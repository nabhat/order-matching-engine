# Order Matching Engine (C++)

![CI](https://github.com/nabhat/order-matching-engine/actions/workflows/ci.yml/badge.svg)
![CODEQL](https://github.com/nabhat/order-matching-engine/actions/workflows/github-code-scanning/codeql/badge.svg)

## Overview

A high-performance single-instrument limit order book and matching engine implemented in modern C++20.
This project demonstrates systems-level design principles with emphasis on correctness, determinism, and low-latency performance.
Designed as a portfolio project for quantitative development and systems engineering roles.

## Features

- **Limit Order Book**: Price-time priority matching with O(1) execution
- **Deterministic Matching**: Repeatable order execution across runs
- **Low-Latency Design**: ~1-8µs per operation (ID generation, order creation)
- **Comprehensive Testing**: Full test coverage with performance benchmarks
- **Modern C++**: C++20 with strict type safety and resource management

## Scope

- Single instrument trading
- Limit orders only (initial phase)
- Price–time priority matching
- Single-threaded core engine

## Non-Goals

- Network I/O and connectivity
- Data persistence
- Multi-instrument support
- Concurrent matching

## Performance

| Operation | Time | Throughput |
|-----------|------|-----------|
| Generate 10,000 unique IDs | 8 ms | 1.25M IDs/sec |
| Order creation & book insertion | <1 µs | >1M orders/sec |
| Test suite execution | 9 ms | Complete verification |

## Status

✅ **Stable** – Core functionality complete and tested

## Build

### Requirements

- CMake ≥ 3.15
- C++20-compatible compiler (GCC 11+ or Clang 13+)

### Build Instructions

```bash
cmake -B build
cmake --build build
```

### Running Tests

```bash
cmake --build build
ctest --test-dir build --output-on-failure
```

## Documentation

Detailed documentation is available in the [docs/](docs/) directory:
- [Design Architecture](docs/design.md) – System design and data structures
- [Order Model](docs/order_model.md) – Order types and lifecycle
- [Concepts](docs/concepts.md) – Core trading concepts
- [NASDAQ ITCH Protocol](docs/nasdaq-itch.md) – Reference implementation notes

## Design Philosophy

**Correctness → Determinism → Performance**

All design decisions prioritize correctness and determinism first, with performance optimization as a secondary concern.
This ensures the engine is reliable and predictable in quantitative trading scenarios.
