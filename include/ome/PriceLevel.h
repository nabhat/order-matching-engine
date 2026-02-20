#pragma once

#include <list>
#include <cstdint>
#include "ome/Order.h"

namespace ome {

class PriceLevel {
public:
    using OrderList = std::list<Order>;

    explicit PriceLevel(Price price);

    Price getPrice() const;

    void addOrder(const Order& order);
    bool removeOrder(OrderId order_id);

    Quantity getTotalQuantity() const;
    bool isEmpty() const;

    const OrderList& getOrders() const;

private:
    Price price_;
    OrderList orders_;
    Quantity total_quantity_;
};

} // namespace ome