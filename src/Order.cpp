#include "ome/Order.h"

namespace ome {

Order::Order(OrderId order_id_,
             ClientId client_id_,
             Price price_,
             Quantity quantity_,
             Side side_,
             std::chrono::steady_clock::time_point timestamp_)
    : order_id(order_id_)
    , client_id(client_id_)
    , price(price_)
    , quantity(quantity_)
    , remaining_qty(quantity_)
    , side(side_)
    , timestamp(timestamp_)
    , status(Status::NEW)
{
}

} // namespace ome