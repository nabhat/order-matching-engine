#include "ome/Trade.h"

namespace ome {

Trade::Trade(TradeId trade_id_,
             OrderId buyer_order_id_,
             OrderId seller_order_id_,
             Price price_,
             Quantity quantity_,
             std::chrono::steady_clock::time_point timestamp_)
    : trade_id(trade_id_),
      buyer_order_id(buyer_order_id_),
      seller_order_id(seller_order_id_),
      price(price_),
      quantity(quantity_),
      timestamp(timestamp_) {}

} // namespace ome
