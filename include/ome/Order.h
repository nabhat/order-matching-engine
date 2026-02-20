#pragma once

#include <cstdint>
#include <chrono>

namespace ome {

using OrderId  = std::uint64_t;
using ClientId = std::uint64_t;
using Price    = std::int64_t;
using Quantity = std::int64_t;

enum class Side {
    BUY,
    SELL
};

enum class Status {
    NEW,
    PARTIALLY_FILLED,
    FILLED,
    CANCELLED,
    REJECTED
};

struct Order {
    OrderId  order_id;
    ClientId client_id;
    Price    price;
    Quantity quantity;
    Quantity remaining_qty;
    Side     side;
    std::chrono::steady_clock::time_point timestamp;
    Status   status;

    Order(OrderId order_id_,
          ClientId client_id_,
          Price price_,
          Quantity quantity_,
          Side side_,
          std::chrono::steady_clock::time_point timestamp_);

    // Rule of zero is fine, but we state intent explicitly
    Order(const Order&)            = default;
    Order& operator=(const Order&) = default;
    Order(Order&&)                 = default;
    Order& operator=(Order&&)      = default;
};

} // namespace ome