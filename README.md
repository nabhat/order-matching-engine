# Order Matching Engine (C++)

![CI](https://github.com/nabhat/order-matching-engine/actions/workflows/ci.yml/badge.svg)

## Overview
A single-instrument limit order book and matching engine implemented in modern C++.
The project focuses on correctness, determinism, and low-latency design principles,
and is intended as a systems-level portfolio project for quantitative development roles.

## Scope
- Limit orders only (initial phase)
- Price–time priority matching
- Single-threaded core engine

## Non-Goals
- Networking
- Persistence
- Multi-instrument support
- Concurrency and parallel matching

## Status
In active development. Design-first approach.

## Build

### Requirements
- CMake ≥ 3.15
- C++20-compatible compiler (GCC / Clang)

### Build Instructions
```bash
cmake -B build
cmake --build build
```

## Design Philosophy
Correctness → determinism → performance.
