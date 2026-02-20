#pragma once

#include <map>
#include <unordered_map>
#include <functional>
#include <optional>
#include "ome/PriceLevel.h"

namespace ome {

class OrderBook {
public:
    OrderBook() = default;

    void addOrder(const Order& order);
    bool cancelOrder(OrderId order_id);

    // Returns nullptr if no bids exist
    const PriceLevel* getBestBid() const;
    const PriceLevel* getBestAsk() const;

    std::optional<Price> getBidAskSpread() const;

    std::size_t getOrderCount() const;

private:
    using BidMap = std::map<Price, PriceLevel, std::greater<Price>>;
    using AskMap = std::map<Price, PriceLevel>;

    BidMap bids_;
    AskMap asks_;

    // OrderId -> (side, price)
    struct OrderIndex {
        Side side;
        Price price;
    };

    std::unordered_map<OrderId, OrderIndex> order_index_;
};

} // namespace ome