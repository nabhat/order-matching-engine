#include <gtest/gtest.h>
#include "ome/Trade.h"

using namespace ome;

TEST(TradeTest, CreateTradeInitializesFields) {
    auto now = std::chrono::steady_clock::now();

    Trade trade(
        1,    // trade_id
        42,   // buyer_order_id
        43,   // seller_order_id
        100,  // price
        10,   // quantity
        now   // timestamp
    );

    EXPECT_EQ(trade.trade_id, 1);
    EXPECT_EQ(trade.buyer_order_id, 42);
    EXPECT_EQ(trade.seller_order_id, 43);
    EXPECT_EQ(trade.price, 100);
    EXPECT_EQ(trade.quantity, 10);
    EXPECT_EQ(trade.timestamp, now);
}

TEST(TradeTest, CopyAndMoveSemanticsWork) {
    auto now = std::chrono::steady_clock::now();

    Trade original(1, 42, 43, 100, 10, now);

    // Test copy semantics
    Trade copied = original;
    EXPECT_EQ(copied.trade_id, original.trade_id);
    EXPECT_EQ(copied.buyer_order_id, original.buyer_order_id);
    EXPECT_EQ(copied.seller_order_id, original.seller_order_id);
    EXPECT_EQ(copied.price, original.price);
    EXPECT_EQ(copied.quantity, original.quantity);

    // Test move semantics
    Trade moved = std::move(copied);
    EXPECT_EQ(moved.trade_id, 1);
    EXPECT_EQ(moved.buyer_order_id, 42);
    EXPECT_EQ(moved.seller_order_id, 43);
    EXPECT_EQ(moved.price, 100);
    EXPECT_EQ(moved.quantity, 10);
}

TEST(TradeTest, MultipleTradesCanBeCreated) {
    auto now1 = std::chrono::steady_clock::now();
    auto now2 = std::chrono::steady_clock::now();

    Trade trade1(1, 10, 11, 100, 5, now1);
    Trade trade2(2, 12, 13, 105, 8, now2);

    EXPECT_EQ(trade1.trade_id, 1);
    EXPECT_EQ(trade2.trade_id, 2);
    EXPECT_NE(trade1.buyer_order_id, trade2.buyer_order_id);
}
