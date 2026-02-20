#include <gtest/gtest.h>
#include "ome/OrderBook.h"

using namespace ome;

static Order makeOrder(
    OrderId id,
    Side side,
    Price price,
    Quantity qty = 1
) {
    return Order(
        id,
        1,
        price,
        qty,
        side,
        std::chrono::steady_clock::now()
    );
}

TEST(OrderBookTwoSidedTest, AddOrdersOnBothSides) {
    OrderBook book;

    book.addOrder(makeOrder(1, Side::BUY, 100));
    book.addOrder(makeOrder(2, Side::SELL, 105));

    ASSERT_NE(book.getBestBid(), nullptr);
    ASSERT_NE(book.getBestAsk(), nullptr);

    EXPECT_EQ(book.getBestBid()->getPrice(), 100);
    EXPECT_EQ(book.getBestAsk()->getPrice(), 105);
}

TEST(OrderBookTwoSidedTest, BestBidAndAskCorrect) {
    OrderBook book;

    book.addOrder(makeOrder(1, Side::BUY, 99));
    book.addOrder(makeOrder(2, Side::BUY, 101));
    book.addOrder(makeOrder(3, Side::SELL, 105));
    book.addOrder(makeOrder(4, Side::SELL, 103));

    EXPECT_EQ(book.getBestBid()->getPrice(), 101);
    EXPECT_EQ(book.getBestAsk()->getPrice(), 103);
}

TEST(OrderBookTwoSidedTest, SpreadCalculatedCorrectly) {
    OrderBook book;

    book.addOrder(makeOrder(1, Side::BUY, 100));
    book.addOrder(makeOrder(2, Side::SELL, 110));

    auto spread = book.getBidAskSpread();
    ASSERT_TRUE(spread.has_value());
    EXPECT_EQ(spread.value(), 10);
}

TEST(OrderBookTwoSidedTest, SpreadUnavailableIfOneSideEmpty) {
    OrderBook book;

    book.addOrder(makeOrder(1, Side::BUY, 100));
    EXPECT_FALSE(book.getBidAskSpread().has_value());
}

TEST(OrderBookTwoSidedTest, CancelAskOrder) {
    OrderBook book;

    book.addOrder(makeOrder(1, Side::SELL, 105));
    EXPECT_TRUE(book.cancelOrder(1));
    EXPECT_EQ(book.getBestAsk(), nullptr);
}