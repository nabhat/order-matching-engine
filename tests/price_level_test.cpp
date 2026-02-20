#include <gtest/gtest.h>
#include "ome/PriceLevel.h"

using namespace ome;

static Order makeOrder(OrderId id, Quantity qty) {
    return Order(
        id,
        1,              // client_id
        100,            // price
        qty,
        Side::BUY,
        std::chrono::steady_clock::now()
    );
}

TEST(PriceLevelTest, AddOrdersMaintainsFIFO) {
    PriceLevel level(100);

    for (int i = 1; i <= 100; ++i) {
        level.addOrder(makeOrder(i, 1));
    }

    OrderId expected = 1;
    for (const auto& order : level.getOrders()) {
        EXPECT_EQ(order.order_id, expected++);
    }
}

TEST(PriceLevelTest, RemoveOrderFromMiddle) {
    PriceLevel level(100);

    level.addOrder(makeOrder(1, 10));
    level.addOrder(makeOrder(2, 20));
    level.addOrder(makeOrder(3, 30));

    EXPECT_TRUE(level.removeOrder(2));
    EXPECT_EQ(level.getOrders().size(), 2);

    auto it = level.getOrders().begin();
    EXPECT_EQ(it->order_id, 1);
    ++it;
    EXPECT_EQ(it->order_id, 3);
}

TEST(PriceLevelTest, RemoveNonExistentOrderReturnsFalse) {
    PriceLevel level(100);

    level.addOrder(makeOrder(1, 10));
    EXPECT_FALSE(level.removeOrder(99));
}

TEST(PriceLevelTest, TotalQuantityCorrect) {
    PriceLevel level(100);

    level.addOrder(makeOrder(1, 10));
    level.addOrder(makeOrder(2, 20));
    level.addOrder(makeOrder(3, 30));

    EXPECT_EQ(level.getTotalQuantity(), 60);

    level.removeOrder(2);
    EXPECT_EQ(level.getTotalQuantity(), 40);
}

TEST(PriceLevelTest, EmptyStateCorrect) {
    PriceLevel level(100);

    EXPECT_TRUE(level.isEmpty());

    level.addOrder(makeOrder(1, 5));
    EXPECT_FALSE(level.isEmpty());

    level.removeOrder(1);
    EXPECT_TRUE(level.isEmpty());
}