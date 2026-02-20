#pragma once

#include <map>
#include <unordered_map>
#include <functional>
#include "ome/PriceLevel.h"

namespace ome {

class OrderBook {
public:
    OrderBook() = default;

    // Bid-side only (BUY orders)
    void addOrder(const Order& order);
    bool cancelOrder(OrderId order_id);

    // Returns nullptr if no bids exist
    const PriceLevel* getBestBid() const;

    std::size_t getOrderCount() const;

private:
    using BidMap = std::map<Price, PriceLevel, std::greater<Price>>;

    BidMap bids_;

    // OrderId -> price level
    std::unordered_map<OrderId, Price> order_index_;
};

} // namespace ome