#include <gtest/gtest.h>
#include "ome/OrderBook.h"

using namespace ome;

static Order makeBuy(OrderId id, Price price, Quantity qty = 1) {
    return Order(
        id,
        1,
        price,
        qty,
        Side::BUY,
        std::chrono::steady_clock::now()
    );
}

TEST(OrderBookBidTest, AddOrdersMultiplePriceLevels) {
    OrderBook book;

    book.addOrder(makeBuy(1, 100));
    book.addOrder(makeBuy(2, 101));
    book.addOrder(makeBuy(3, 99));

    ASSERT_NE(book.getBestBid(), nullptr);
    EXPECT_EQ(book.getBestBid()->getPrice(), 101);
}

TEST(OrderBookBidTest, FIFOWithinSamePrice) {
    OrderBook book;

    book.addOrder(makeBuy(1, 100));
    book.addOrder(makeBuy(2, 100));
    book.addOrder(makeBuy(3, 100));

    const auto& orders = book.getBestBid()->getOrders();

    auto it = orders.begin();
    EXPECT_EQ(it++->order_id, 1);
    EXPECT_EQ(it++->order_id, 2);
    EXPECT_EQ(it++->order_id, 3);
}

TEST(OrderBookBidTest, CancelOrderUpdatesBestBid) {
    OrderBook book;

    book.addOrder(makeBuy(1, 100));
    book.addOrder(makeBuy(2, 101));

    EXPECT_EQ(book.getBestBid()->getPrice(), 101);

    EXPECT_TRUE(book.cancelOrder(2));
    EXPECT_EQ(book.getBestBid()->getPrice(), 100);
}

TEST(OrderBookBidTest, CancelNonExistentOrder) {
    OrderBook book;
    EXPECT_FALSE(book.cancelOrder(999));
}

TEST(OrderBookBidTest, LargeNumberOfOrders) {
    OrderBook book;

    constexpr int N = 1000;
    for (int i = 0; i < N; ++i) {
        book.addOrder(makeBuy(i + 1, 100 + (i % 10)));
    }

    EXPECT_EQ(book.getOrderCount(), N);
    ASSERT_NE(book.getBestBid(), nullptr);
}