#pragma once 

#include <list>
#include <exception>
#include <stdexcept>
#include <memory>
//#include <format>

#include "OrderType.h"
#include "Side.h"
#include "Usings.h"
#include "Constants.h"

class Order{
public:
    Order(OrderType type, OrderId orderid, Side side, Price price, Quantity quantity)
    : ordertype_{ type }, 
    orderid_{ orderid }, 
    side_{ side },
    price_{ price }, 
    initialquantity_{ quantity },
    remainingquantity_{ quantity }
    {}

    Order(OrderId orderid, Side side, Quantity quantity)
    : Order(OrderType::Market, orderid, side, Constants::InvalidPrice, quantity)
    {}

    OrderId GetOrderId() const { return orderid_; }
    Side GetSide() const { return side_; }
    Price GetPrice() const { return price_; }
    OrderType GetOrderType() const { return ordertype_; }
    Quantity GetInitialQuantity() const { return initialquantity_; }
    Quantity GetRemainingQuantity() const { return remainingquantity_; }
    Quantity GetFilledQuantity() const { return GetInitialQuantity()-GetRemainingQuantity(); }
    bool IsFilled() const { return GetRemainingQuantity()==0; }

    void Fill(Quantity quantity){
        if (quantity>GetRemainingQuantity()){
            throw std::logic_error("Order cannot be filled for more than its remaining quantity"); 
        }

        remainingquantity_ -= quantity; 
    }

private:
    OrderType ordertype_; 
    OrderId orderid_; 
    Side side_; 
    Price price_; 
    Quantity initialquantity_; 
    Quantity remainingquantity_; 
};

using OrderPointer = std::shared_ptr<Order>; 
using OrderPointers = std::vector<OrderPointer>; 