#include "ome/OrderIDGenerator.h"

namespace ome {

OrderIDGenerator::OrderIDGenerator()
    : counter_(0)
{
}

OrderIDGenerator::OrderId OrderIDGenerator::next() {
    // Pre-increment ensures IDs start from 1 by default
    return ++counter_;
}

void OrderIDGenerator::reset(OrderId start) {
    counter_.store(start);
}

} // namespace ome