# Order Matching Engine — Data Structure Design

This document describes the core data structures used in the order matching engine
and the rationale behind each design decision. The emphasis is on correctness,
determinism, and clarity before performance optimizations.

---

## 1. Design Goals

The primary goals guiding data structure selection are:

1. **Correctness**
   - Strict enforcement of price–time priority
   - Explicit order lifecycle handling

2. **Determinism**
   - Identical input sequences produce identical outcomes
   - No reliance on undefined iteration order

3. **Clarity**
   - Design must be understandable to future maintainers
   - Trade-offs must be explicit

4. **Performance (Secondary)**
   - Efficient enough for realistic workloads
   - Optimizations introduced only when justified

---

## 2. High-Level Order Book Structure

The order book is split into two independent sides:

- **Bid side** (buy orders)
- **Ask side** (sell orders)

Each side is modeled as:
- A price-ordered container of **price levels**
- Each price level contains a FIFO queue of orders

This separation simplifies reasoning and matching logic.

---

## 3. Price Level Storage

### Chosen Structure
- `std::map<Price, PriceLevel>`

Where:
- Bid side uses descending price order
- Ask side uses ascending price order

### Justification

- `std::map` maintains strict ordering guarantees
- Deterministic iteration order is critical for correctness
- Logarithmic insertion and lookup are acceptable at this stage
- Clear semantic meaning: “best price” is always `begin()`

### Trade-offs

**Pros**
- Simple and explicit
- Easy to reason about
- Naturally enforces price priority

**Cons**
- Higher constant factors than flat or array-based structures
- Not cache-optimal for ultra-low latency systems

Future optimizations (e.g., flat maps, custom trees) can replace this once correctness is locked in.

---

## 4. Order Queue at Each Price Level

### Chosen Structure
- `std::deque<Order>` (or equivalent linked structure)

### Justification

- FIFO behavior is required for time priority
- O(1) insertion at the back
- O(1) removal from the front
- Stable ordering under partial fills

### Trade-offs

**Pros**
- Explicit FIFO semantics
- Simple implementation
- Works naturally with partial fills

**Cons**
- Removal of arbitrary elements (e.g., cancel by ID) is not O(1)
- Requires coordination with order ID lookup

The simplicity and clarity outweigh performance concerns at this stage.

---

## 5. Order ID Lookup

### Chosen Structure
- `std::unordered_map<OrderId, OrderHandle>`

Where `OrderHandle` references:
- Side (bid / ask)
- Price level
- Position within the order queue

### Justification

- Enables O(1) average-time lookup for cancel/modify
- Decouples external order identity from internal storage
- Avoids scanning the book to find an order

### Trade-offs

**Pros**
- Fast cancellation
- Clean separation of concerns
- Scales with number of active orders

**Cons**
- Requires careful lifetime management
- Introduces indirection

Correctness is prioritized over micro-optimizations.

---

## 6. Matching Algorithm Implications

This data structure layout supports a straightforward matching loop:

1. Identify best opposing price level
2. Match against front of FIFO queue
3. Generate trades
4. Update quantities and remove filled orders
5. Clean up empty price levels

Each step maps directly to the underlying structures, minimizing hidden complexity.

---

## 7. Invariants Enforced by Design

The following invariants are naturally enforced:

- Price–time priority cannot be violated
- Orders execute strictly in FIFO order at a price
- Best price lookup is unambiguous
- Deterministic traversal order

Any violation indicates a bug in logic, not ambiguity in data structures.

---

## 8. Alternatives Considered

- Flat arrays / vectors indexed by price  
  → Rejected due to sparse price space and complexity

- Skip lists or custom trees  
  → Deferred until performance profiling justifies complexity

- Intrusive linked lists  
  → Adds implementation risk early on

The chosen design favors correctness and maintainability.

---

## 9. Future Extensions

Potential future improvements include:
- Custom allocators for order objects
- Cache-friendly price level containers
- Lock-free or concurrent structures (out of scope for now)

These can be evaluated once baseline behavior is stable and tested.

---

## 10. Summary

The selected data structures provide a clear, deterministic, and correct foundation
for the matching engine. Performance optimizations are intentionally deferred to
avoid premature complexity and correctness risk.