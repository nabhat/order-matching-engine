#include <gtest/gtest.h>
#include <unordered_set>
#include "ome/OrderIDGenerator.h"

using namespace ome;

TEST(OrderIDGeneratorTest, GeneratesUniqueIds) {
    OrderIDGenerator gen;
    std::unordered_set<OrderIDGenerator::OrderId> ids;

    constexpr int N = 10'000;
    for (int i = 0; i < N; ++i) {
        auto id = gen.next();
        ids.insert(id);
    }

    EXPECT_EQ(ids.size(), N);
}

TEST(OrderIDGeneratorTest, GeneratesMonotonicallyIncreasingIds) {
    OrderIDGenerator gen;

    auto prev = gen.next();
    for (int i = 0; i < 9999; ++i) {
        auto current = gen.next();
        EXPECT_GT(current, prev);
        prev = current;
    }
}

TEST(OrderIDGeneratorTest, ResetWorksForTesting) {
    OrderIDGenerator gen;

    auto id1 = gen.next();
    gen.reset(0);
    auto id2 = gen.next();

    EXPECT_EQ(id1, id2);
}