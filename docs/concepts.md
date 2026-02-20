# Market Microstructure & Order Book Concepts

This document captures the core market microstructure concepts required to design
and reason about a limit order matching engine. The focus is on correctness,
determinism, and price–time priority — not trading strategies.

---

## 1. Limit Order Book Overview

A **limit order book (LOB)** is a data structure that stores buy and sell limit orders
for a financial instrument and matches them according to predefined rules.

Each order specifies:
- Side: Buy (Bid) or Sell (Ask)
- Price
- Quantity
- Time (arrival order)

The book is divided into two sides:
- **Bid side**: Buy orders, sorted by descending price
- **Ask side**: Sell orders, sorted by ascending price

---

## 2. Bid, Ask, and Spread

- **Best Bid**: Highest price a buyer is willing to pay
- **Best Ask**: Lowest price a seller is willing to accept
- **Spread**: `Best Ask - Best Bid`

A smaller spread generally indicates higher liquidity.


## Order Book Snapshot

| Ask Side (Sell) | Quantity | Bid Side (Buy) | Quantity |
|-----------------|----------|----------------|----------|
| 101.00          |  50      | 99.00          | 40       |
| 102.00          | 100      | 98.50          | 60       |


**Spread**: 101.00 − 99.00 = 2.00

---

## 3. Price–Time Priority

Price–time priority is the fundamental matching rule used by most modern exchanges.

### Price Priority
- Better prices execute first
- Higher bid prices beat lower bids
- Lower ask prices beat higher asks

### Time Priority
- For orders at the same price, earlier orders execute first (FIFO)

This ensures:
- Fairness
- Determinism
- Predictable execution behavior

---

## 4. Order Lifecycle and States

An order progresses through well-defined states:

1. **New**
   - Order accepted and validated
2. **Partially Filled**
   - Some quantity executed, remainder still active
3. **Filled**
   - Entire quantity executed
4. **Cancelled**
   - Removed by user before full execution
5. **Rejected**
   - Invalid order (never enters the book)

State transitions must be explicit and auditable.

---

## 5. Aggressive vs Passive Orders

- **Passive order**
  - Does not immediately match
  - Rests on the book providing liquidity
- **Aggressive order**
  - Crosses the spread and executes immediately
  - Consumes liquidity

Example:
- Buy limit at 101 when best ask is 100 → aggressive
- Buy limit at 99 when best ask is 100 → passive

---

## 6. Matching Mechanics (High Level)

When an aggressive order arrives:

1. Identify the best opposing price level
2. Check if prices cross
3. Match against resting orders in FIFO order
4. Generate trade(s)
5. Update remaining quantities
6. Remove fully filled orders
7. Stop when order is filled or no more matches exist

Partial fills are allowed and expected.

---

## 7. Order Book Invariants

The engine must always maintain these invariants:

- No order executes at a worse price than its limit
- Price–time priority is never violated
- Total executed quantity ≤ original order quantity
- No crossed book after matching completes
- Order IDs are unique and immutable

Violating any invariant is a correctness bug.

---

## 8. Common Edge Cases to Handle

1. **Partial fills across multiple price levels**
2. **Cancel order that is partially filled**
3. **Cancel order not present in the book**
4. **Market order when book is empty**
5. **Multiple orders with same price and timestamp**
6. **Self-trade prevention (same participant on both sides)**
7. **Zero or negative price/quantity (must reject)**

These cases must be explicitly tested.

---

## 9. Determinism and Reproducibility

Given the same sequence of inputs:
- The same trades must occur
- In the same order
- At the same prices and quantities

Determinism is critical for:
- Debugging
- Replay
- Auditability
- Regulatory compliance

---

## 10. Why This Matters for Implementation

Understanding these concepts guides:
- Data structure choice
- Matching algorithm design
- Test coverage
- Performance trade-offs

The matching engine is **infrastructure**, not a trading strategy.
Correctness and predictability are more important than raw speed early on.

---

## References

- NASDAQ TotalView-ITCH Specification
- “Trading and Exchanges” — Larry Harris
- Exchange matching engine documentation (CME, LSE)