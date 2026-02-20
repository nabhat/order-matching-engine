#include <gtest/gtest.h>
#include "ome/Order.h"

using namespace ome;

TEST(OrderTest, CreateOrderInitializesFields) {
    auto now = std::chrono::steady_clock::now();

    Order order(
        1,        // order_id
        42,       // client_id
        100,      // price
        10,       // quantity
        Side::BUY,
        now
    );

    EXPECT_EQ(order.order_id, 1);
    EXPECT_EQ(order.client_id, 42);
    EXPECT_EQ(order.price, 100);
    EXPECT_EQ(order.quantity, 10);
    EXPECT_EQ(order.remaining_qty, 10);
    EXPECT_EQ(order.side, Side::BUY);
    EXPECT_EQ(order.status, Status::NEW);
}

TEST(OrderTest, CopyAndMoveSemanticsWork) {
    auto now = std::chrono::steady_clock::now();

    Order original(1, 42, 100, 10, Side::SELL, now);

    Order copied = original;
    EXPECT_EQ(copied.order_id, original.order_id);
    EXPECT_EQ(copied.remaining_qty, original.remaining_qty);

    Order moved = std::move(original);
    EXPECT_EQ(moved.order_id, 1);
    EXPECT_EQ(moved.remaining_qty, 10);
}