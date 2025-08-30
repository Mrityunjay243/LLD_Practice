#pragma once

#include "Order.h"

class OrderModify{
public:
    OrderModify(OrderId orderid, Side side, Price price, Quantity quantity)
    : orderid_{ orderid }
    , price_{ price }
    , side_{ side }
    , quantity_{ quantity }
    {}

    OrderId GetOrderId() const { return orderid_; }
    Price GetPrice() const { return price_; }
    Side GetSide() const { return side_; }
    Quantity GetQuantity() const { return quantity_; }

    OrderPointer ToOrderPointer(OrderType type) const {
        return std::make_shared<Order>(type, GetOrderId(), GetSide(), GetPrice(), GetQuantity()); 
    }

private:
    OrderId orderid_; 
    Price price_; 
    Side side_; 
    Quantity quantity_; 
};