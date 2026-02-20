#include "ome/OrderBook.h"
#include <stdexcept>

namespace ome {

void OrderBook::addOrder(const Order& order) {
    if (order.side != Side::BUY) {
        throw std::invalid_argument("OrderBook (bid side) accepts BUY orders only");
    }

    auto [it, inserted] = bids_.try_emplace(order.price, order.price);
    it->second.addOrder(order);

    order_index_[order.order_id] = order.price;
}

bool OrderBook::cancelOrder(OrderId order_id) {
    auto idx_it = order_index_.find(order_id);
    if (idx_it == order_index_.end()) {
        return false;
    }

    Price price = idx_it->second;
    auto level_it = bids_.find(price);
    if (level_it == bids_.end()) {
        return false;
    }

    bool removed = level_it->second.removeOrder(order_id);
    if (!removed) {
        return false;
    }

    if (level_it->second.isEmpty()) {
        bids_.erase(level_it);
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

std::size_t OrderBook::getOrderCount() const {
    return order_index_.size();
}

} // namespace ome