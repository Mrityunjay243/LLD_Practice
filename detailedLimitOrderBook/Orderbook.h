#pragma once

#include <map>
#include <unordered_map>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>

#include "Usings.h"
#include "Order.h"
#include "OrderModify.h"
#include "OrderBookLevelInfos.h"
#include "Trade.h"

class OrderBook{
private:
    struct OrderEntry{
        OrderPointer order_{ nullptr }; 
        OrderPointers::iterator location_; 
    };

    struct LevelData{
        Quantity quantity_{ }; 
        Quantity count_{ }; 

        enum class Action{
            Add, 
            Remove, 
            Match
        };
    };

    std::unordered_map<Price, LevelData> data_; 
    std::map<Price, OrderPointers, std::greater<Price>> bids_; 
    std::map<Price, OrderPointers> asks_; 
    std::unordered_map<OrderId, OrderEntry> orders_; 
    mutable std::mutex ordersmutex_; 
    std::thread ordersPruneThread_; 
    std::condition_variable shutdownCV_; 
    std::atomic<bool> shutdown_{ false }; 

    void PruneGoodForDayOrders(); 

    void CancelOrders(OrderIds Orderids);
    void CancelOrderInternal(OrderId orderid); 

    void OnOrderCancelled(OrderPointer order); 
    void OnOrderAdded(OrderPointer order); 
    void OnOrderMatched(Price price, Quantity quantity, bool isFullyFilled); 
    void UpdateLevelData(Price price, Quantity quantity, LevelData::Action action); 

    bool CanFullyFill(Side side, Price price, Quantity quantity) const; 
    bool CanMatch(Side side, Price price, Quantity quantity) const; 
    Trades MatchOrders(); 

public:
    OrderBook(); 
    OrderBook(const OrderBook&) = delete; 
    void operator=(const OrderBook&) = delete; 
    OrderBook(OrderBook&&) = delete; 
    void operator=(OrderBook&&) = delete; 
    ~OrderBook(); 

    Trades AddOrder(OrderPointer order);
    void CancelOrder(OrderId orderid); 
    Trades ModifyOrder(OrderModify order); 

    std::size_t Size() const; 
    OrderBookLevelInfos GetOrderInfos() const; 
};