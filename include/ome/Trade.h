#pragma once

#include <cstdint>
#include <chrono>
#include "Order.h"

namespace ome {

using TradeId = std::uint64_t;

struct Trade {
    TradeId trade_id;
    OrderId buyer_order_id;
    OrderId seller_order_id;
    Price   price;
    Quantity quantity;
    std::chrono::steady_clock::time_point timestamp;

    Trade(TradeId trade_id_,
          OrderId buyer_order_id_,
          OrderId seller_order_id_,
          Price price_,
          Quantity quantity_,
          std::chrono::steady_clock::time_point timestamp_);

    // Rule of zero - default semantics are appropriate for this lightweight struct
    Trade(const Trade&)            = default;
    Trade& operator=(const Trade&) = default;
    Trade(Trade&&)                 = default;
    Trade& operator=(Trade&&)      = default;
};

} // namespace ome
