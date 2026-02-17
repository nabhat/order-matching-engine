# Order Model & Lifecycle

This document defines the conceptual model and lifecycle of an order in the
Order Matching Engine (OME). The focus is on correctness, determinism, and
clarity of behavior rather than implementation details.

---

## 1. Definition of an Order

An order represents a participant’s intent to buy or sell a specified quantity
of a financial instrument under defined constraints.

Within the system, an order:
- Encapsulates intent (buy or sell)
- Specifies constraints (price and quantity)
- Enters the order book and may be matched against opposing orders
- Exists until it reaches a terminal state (filled, canceled, or rejected)

An order is **not** a trade.  
A trade is the result of matching two compatible orders.

---

## 2. Core Attributes

An order must carry sufficient information to support correct matching,
prioritization, and lifecycle management.

### Identity
- Unique order identifier
- Participant or client identifier (logical, not authenticated)

### Intent
- Side: Buy or Sell
- Limit price
- Original quantity

### Execution Tracking
- Remaining quantity
- Executed (filled) quantity

### Priority & Ordering
- Time priority indicator (timestamp or sequence number)
- Used to enforce price–time priority

### State
- Current lifecycle state of the order

Only attributes that directly affect correctness, matching behavior, or
priority are included at this stage.

---

## 3. Order Types (Initial Scope)

### Supported (Initial Implementation)
- Limit Buy Order
- Limit Sell Order

### Explicitly Not Supported (Out of Scope)
- Market orders
- Immediate-or-Cancel (IOC)
- Fill-or-Kill (FOK)
- Stop orders
- Order modification (cancel–replace)
- Iceberg or hidden orders

Restricting scope allows the core matching logic to remain simple, correct,
and verifiable. Additional order types may be introduced incrementally.

---

## 4. Order States

The order lifecycle is modeled as a finite state machine.

### Non-Terminal States (Active)
- **New**  
  Order has been accepted and is eligible for matching.

- **Partially Filled**  
  Order has been matched for some quantity but remains active with remaining quantity.

### Terminal States
- **Filled**  
  Entire quantity has been executed.

- **Canceled**  
  Order was removed by the participant before full execution.

- **Rejected**  
  Order was invalid and never entered the order book.

An order in a terminal state must never transition again.

---

## 5. State Transitions

State transitions occur only in response to well-defined events.

### Valid Transitions
- New → Partially Filled
- New → Filled
- Partially Filled → Filled
- New → Canceled
- Partially Filled → Canceled
- New → Rejected

### Invalid Transitions
- Any transition from a terminal state
- Filled → Canceled
- Rejected → Any other state

The matching engine must enforce these transitions strictly.

---

## 6. Invariants

The following conditions must **always** hold:

- Remaining quantity is never negative
- Executed quantity + remaining quantity = original quantity
- A terminal order is never matched again
- An order cannot be both filled and canceled
- Price–time priority is preserved among all active orders
- Order identifiers are unique within the system

Violating any invariant indicates a correctness bug.

---

## 7. Error Scenarios

The system must define clear behavior for invalid or unexpected inputs.

Examples include:
- Order with zero or negative quantity → Rejected
- Order with invalid price → Rejected
- Cancel request for unknown order → Ignored or rejected
- Cancel request for terminal order → Ignored
- Duplicate order identifier → Rejected

Error handling is policy-driven and deterministic.

---

## 8. Non-Goals

The following concerns are intentionally excluded from the order model:

- Networking and transport protocols
- Persistence and recovery
- Risk checks and credit limits
- Regulatory compliance
- Multi-instrument support
- Concurrency and threading

These aspects may be addressed in higher-level system components but are
outside the scope of the core order model.

---

## Summary

This order model prioritizes:
- Clear semantics
- Strict correctness guarantees
- Minimal but complete scope

A well-defined order lifecycle is foundational to building a correct,
maintainable, and high-performance matching engine.
