# Nasdaq TotalView-ITCH 5.0 – Key Concepts

## Overview

Nasdaq TotalView-ITCH 5.0 is a binary outbound market data feed that publishes a full-depth view of the Nasdaq limit order book, including all visible orders, executions, and system events.
It is *not* an order entry protocol; it only disseminates events so downstream systems can reconstruct the exact state and evolution of the exchange book in real time.

## Message stream and architecture

The ITCH feed is a sequenced stream of variable-length messages; each message begins with a one-byte “message type” that determines its format and length.
Messages are typically transported over higher-level protocols like SoupBinTCP, which handle delivery guarantees and sequencing, while the ITCH payload itself focuses purely on market data content.
Each payload message contains fields such as a stock locate code, an internal tracking number, and a nanosecond-resolution timestamp from midnight, enabling deterministic replay.

## Core ITCH message types (order-book related)

- **System Event (`S`)** – Marks key feed and market lifecycle events such as start of day, end of day, and trading halts or resumptions.
- **Stock Directory (`R`)** – Static reference data per symbol (e.g., symbol, listing information) needed to interpret subsequent messages.
- **Stock Trading Action (`H`)** – Announces per-symbol trading status changes (e.g., halted, paused, or trading).
- **Add Order (`A`) / Add Order with MPID (`F`)** – Introduce new visible limit orders to the book with an order ID, side, price, and quantity.
- **Order Executed (`E`) / Order Executed With Price (`C`)** – Indicate that an existing resting order (by order ID) has been executed, partially or fully, at a given quantity and price.
- **Order Cancel (`X`)** – Reduce the remaining quantity of an existing order without removing it entirely, modeling a partial cancellation.
- **Order Delete (`D`)** – Remove an order from the book entirely, typically representing full cancellation or an administrative removal.
- **Order Replace (`U`)** – Replace an existing order ID with a new order ID and potentially new price/size, logically modeling cancel-plus-new in one message.
- **Trade (`P`) / Cross Trade (`Q`)** – Represent on-book trades (e.g., a visible resting limit matched by an aggressor) and special cross transactions, which impact volume but not always resting depth in the same way.


## Price–time priority

Most modern limit order markets, including Nasdaq, implement **price–time priority** for visible orders.
Under price–time priority, orders are first ranked by price (best bid = highest buy price, best ask = lowest sell price), and among orders at the same price, earlier arrival times get execution priority.
This effectively turns each price level into a FIFO queue: when an incoming contra-side order matches at that price, fills are allocated to older resting orders before newer ones.

## Bid, ask, and spread

At any instant, the **bid side** of the book contains all buy limit orders, and the best bid is the highest-priced buy order currently resting.
The **ask side** contains all sell limit orders, and the best ask (or offer) is the lowest-priced sell order currently resting.
The **spread** is defined as best ask minus best bid; a tighter spread generally indicates higher liquidity and lower transaction costs for immediate execution.

## Order states and lifecycle (conceptual)

Although ITCH is an event feed and does not explicitly label “states,” the following conceptual lifecycle can be reconstructed from its messages:

1. **New / Resting** – An order appears on the book when an Add Order (`A` / `F`) message is received, creating an active, visible limit order with an initial quantity.
2. **Partially Filled** – One or more Order Executed (`E` / `C`) messages reduce the remaining quantity, but a positive quantity still rests in the book at its price level.
3. **Fully Executed** – An execution event or series of executions reduces remaining quantity to zero; at this point the order is logically removed from the book.
4. **Cancelled (partial)** – An Order Cancel (`X`) message reduces the remaining quantity without executing it, but some quantity may still remain resting.
5. **Cancelled (full) / Deleted** – An Order Delete (`D`) message removes the order entirely, regardless of prior fills or cancellations.
6. **Replaced** – An Order Replace (`U`) message moves the remaining quantity to a new order ID and potentially a new price, effectively closing the original order and opening a new one.

For your matching engine, these events define transitions on an internal `Order` object: from `NEW` to `PARTIALLY_FILLED` to `FILLED`, or from `NEW`/`PARTIALLY_FILLED` to `CANCELLED`/`REPLACED` depending on which ITCH messages arrive.

## Mapping ITCH messages to engine actions

A practical way to model ITCH inside your engine is to treat each message type as a pure event that mutates your in-memory book:


| ITCH message | Engine operation | Typical state transition |
| :-- | :-- | :-- |
| `A` / `F` Add Order | Insert new limit order into price–time queue | `NONE` → `NEW` (resting) |
| `E` / `C` Execute | Match against identified resting order, reduce its quantity | `NEW`/`PARTIALLY_FILLED` → same or `FILLED` |
| `X` Cancel | Reduce remaining quantity without trade | `NEW`/`PARTIALLY_FILLED` → same or `CANCELLED` |
| `D` Delete | Remove order from book regardless of remaining quantity | `NEW`/`PARTIALLY_FILLED` → `CANCELLED`/`DELETED` |
| `U` Replace | Cancel original ID, insert modified order under new ID | original: `ACTIVE` → `REPLACED`; new: `NEW` |
| `P` Trade | Record trade; may correspond to execution of one or more orders | Depends on whether it references resting orders |

This mapping is implied by the ITCH 5.0 specification and is how open-source parsers reconstruct order books from the feed.

## Price–time priority in the engine

To implement price–time priority consistent with ITCH semantics:

- Maintain two separate books: one for bids and one for asks; each side is keyed by price, then by arrival time (e.g., an insertion-order queue per price level).
- For an incoming aggressive order (e.g., a market order or a limit order priced to cross the spread), always match against the *best* opposing price first, and within that price, match in queue order (earliest resting first).
- When multiple executions happen against the same resting order, record them as multiple execution events in sequence, mirroring how ITCH can show an order being executed in pieces over time.

This gives you deterministic queue behavior and makes it straightforward to replay ITCH logs to validate that your matching logic produces the same trades and book states.

## Edge cases to handle

1. **Partial fills followed by cancel**
    - An order is partially executed by several `E`/`C` messages and then cancelled via `X` or `D`, leaving non-zero historical quantity but zero current resting quantity.
2. **Multiple executions against one order**
    - A single resting order is executed in many small trades over time (e.g., one large sell order filled by many small buyers), so you must track cumulative executed quantity and remaining quantity accurately.
3. **Replace on a partially filled order**
    - An order is partially filled and then replaced (`U`), changing its price and order ID; the new order appears at a new price level and at the back of the queue for that price, while the old ID stops receiving further events.
4. **Trades against non-displayed or special orders**
    - Some markets allow non-displayed (hidden) or special order types; ITCH may report a trade message even when the contra-side order was not visible in the book, so depth changes without a prior visible Add.[^10]
5. **Book resets and halts**
    - System Event (`S`) or Stock Trading Action (`H`) messages can imply halts, resumptions, or resets to the book, requiring you to clear or freeze book state in a controlled manner.
6. **Out-of-order or duplicate events at replay level**
    - While the exchange guarantees sequencing on the feed, your storage or replay layer might encounter out-of-order records or duplicates, so your engine logic should be idempotent and validate that state transitions are legal given current state.
7. **Zero-quantity after combined exec + cancel**
    - Edge case where an execution and a cancel together exactly consume the order quantity, and you must avoid leaving a ghost resting order with zero quantity.



***
