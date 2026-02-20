#include "ome/PriceLevel.h"

namespace ome {

PriceLevel::PriceLevel(Price price)
    : price_(price)
    , total_quantity_(0)
{
}

Price PriceLevel::getPrice() const {
    return price_;
}

void PriceLevel::addOrder(const Order& order) {
    orders_.push_back(order);
    total_quantity_ += order.remaining_qty;
}

bool PriceLevel::removeOrder(OrderId order_id) {
    for (auto it = orders_.begin(); it != orders_.end(); ++it) {
        if (it->order_id == order_id) {
            total_quantity_ -= it->remaining_qty;
            orders_.erase(it);
            return true;
        }
    }
    return false;
}

Quantity PriceLevel::getTotalQuantity() const {
    return total_quantity_;
}

bool PriceLevel::isEmpty() const {
    return orders_.empty();
}

const PriceLevel::OrderList& PriceLevel::getOrders() const {
    return orders_;
}

} // namespace ome