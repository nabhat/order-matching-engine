#pragma once

#include <atomic>
#include <cstdint>

namespace ome {

class OrderIDGenerator {
public:
    using OrderId = std::uint64_t;

    OrderIDGenerator();

    // Generate next unique, monotonically increasing ID
    OrderId next();

    // Reset counter (intended for testing only)
    void reset(OrderId start = 0);

private:
    std::atomic<OrderId> counter_;
};

} // namespace ome