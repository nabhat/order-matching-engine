#include "ome/OrderBook.h"
#include <stdexcept>

namespace ome {

void OrderBook::addOrder(const Order& order) {
    if (order.side == Side::BUY) {
        auto [it, inserted] = bids_.try_emplace(order.price, order.price);
        it->second.addOrder(order);
    } else {
        auto [it, inserted] = asks_.try_emplace(order.price, order.price);
        it->second.addOrder(order);
    }

    order_index_[order.order_id] = {order.side, order.price};
}

bool OrderBook::cancelOrder(OrderId order_id) {
    auto idx_it = order_index_.find(order_id);
    if (idx_it == order_index_.end()) {
        return false;
    }

    const auto& [side, price] = idx_it->second;

    if (side == Side::BUY) {
        auto level_it = bids_.find(price);
        if (level_it == bids_.end()) return false;

        if (!level_it->second.removeOrder(order_id)) return false;

        if (level_it->second.isEmpty()) {
            bids_.erase(level_it);
        }
    } else {
        auto level_it = asks_.find(price);
        if (level_it == asks_.end()) return false;

        if (!level_it->second.removeOrder(order_id)) return false;

        if (level_it->second.isEmpty()) {
            asks_.erase(level_it);
        }
    }

    order_index_.erase(idx_it);
    return true;
}

const PriceLevel* OrderBook::getBestBid() const {
    if (bids_.empty()) {
        return nullptr;
    }
    return &bids_.begin()->second;
}

const PriceLevel* OrderBook::getBestAsk() const {
    if (asks_.empty()) {
        return nullptr;
    }
    return &asks_.begin()->second;
}

std::optional<Price> OrderBook::getBidAskSpread() const {
    if (bids_.empty() || asks_.empty()) {
        return std::nullopt;
    }
    return getBestAsk()->getPrice() - getBestBid()->getPrice();
}

std::size_t OrderBook::getOrderCount() const {
    return order_index_.size();
}

} // namespace ome