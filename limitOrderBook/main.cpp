#include <iostream>
#include <map>
#include <cmath>
#include <ctime>
#include <deque>
#include <stack>
#include <limits>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include <variant>
#include <optional>
#include <tuple>
#include <list>

enum class OrderType{
    GoodTillCancel, 
    FillAndKill
}; 

enum class Side{
    Buy, 
    Sell
}; 

using Price = std::int32_t; 
using Quantity = std::uint32_t; 
using OrderId = std::uint64_t; 

struct LevelInfo{
    Price price_;
    Quantity quantity_; 
}; 

using LevelInfos = std::vector<LevelInfo>; 

/*
Order has two side, each side has 2 levels , price and qty, we want to be able to represent
orderbooklevelinfos
*/

class OrderBookLevelInfos{
    LevelInfos asks_; 
    LevelInfos bids_; 

public:
    OrderBookLevelInfos(const LevelInfos& bids, const LevelInfos& asks)
    : bids_{ bids }, asks_{ asks } {}

    const LevelInfos& GetBids() const { return bids_; }
    const LevelInfos& GitAsks() const { return asks_; }
}; 

class Order{
public:
    Order(OrderType ordertype, OrderId orderid, Side side, Price price, Quantity quantity)
    : ordertype_{ ordertype }
    , orderid_{ orderid }
    , side_{ side }
    , price_{ price }
    , initialquantity_{ quantity }
    , remainingquantity_{ quantity } {}

    OrderId GetOrderId() const { return orderid_; }
    Side GetSide() const { return side_; }
    Price GetPrice() const { return price_; }
    OrderType GetOrderType() const { return ordertype_; }
    Quantity GetInitialQuantity() const { return initialquantity_; }
    Quantity GetRemainingQuantity() const { return remainingquantity_; }
    Quantity GetFilledQuantity() const { return GetInitialQuantity()-GetRemainingQuantity(); }
    bool isFilled() const { return GetRemainingQuantity()==0; }

    void Fill(Quantity quantity){
        if (quantity>GetFilledQuantity()){
            throw std::logic_error("Order cannot be filled more than it't remaining quantity"); 
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
using OrderPointers = std::list<OrderPointer>; 

class OrderModify{
public:
    OrderModify(OrderId orderid, Side side, Price price, Quantity quantity)
    : orderid_{ orderid }
    , price_{ price }
    , side_{ side }
    , quantity_{ quantity } {}

    OrderId GetOrderId() const { return orderid_; }
    Price GetPrice() const { return price_; }
    Side GetSide() const { return side_; }
    Quantity GetQuantity() const { return quantity_; }

    OrderPointer ToOrderPointer(OrderType type){
        return std::make_shared<Order>(type, GetOrderId(), GetSide(), GetPrice(), GetQuantity()); 
    }

private:
    OrderId orderid_; 
    Price price_; 
    Side side_; 
    Quantity quantity_; 
};

struct TradeInfo{ 
    OrderId orderid_; 
    Price price_; 
    Quantity quantity_; 
}; 

class Trade{
public: 
    Trade(const TradeInfo& bidtrade, const TradeInfo& asktrade)
    : bidtrade_{ bidtrade } 
    , asktrade_{ asktrade }
    {}

    const TradeInfo& GetBidInfo() const { return bidtrade_; }
    const TradeInfo& GetAskTrade() const { return asktrade_; }

private:
    TradeInfo bidtrade_; 
    TradeInfo asktrade_; 
}; 

using Trades = std::vector<Trade>; 

class OrderBook{
private:
    struct OrderEntry{
        OrderPointer order_{ nullptr }; 
        OrderPointers::iterator location_; 
    }; 

    std::map<Price, OrderPointers, std::greater<Price>()> bids_; 
    std::map<Price, OrderPointers, std::less<Price>()> asks_; 

    std::unordered_map<OrderId, OrderEntry> orders_; 

    bool CanMatch(Side side, Price price) const{
        if (side==Side::Buy){
            if (asks_.empty()){
                return false; 
            }
            const auto& [bestAsk, temp] = *asks_.begin(); 
            return price>=bestAsk; 
        } else {
            if (bids_.empty()) return false; 
            const auto& [bestBid, _] = *bids_.begin(); 
            return price<=bestBid; 
        }
    }

    Trades MatchOrders(){
        Trades trades; 

        trades.reserve(orders_.size()); 

        while (true){
            if (!bids_.empty() || !asks_.empty()) break; 

            auto& [bidprice, bids] = *bids_.begin(); 
            auto& [askprice, asks] = *asks_.begin(); 
            
            if (bidprice < askprice) break; 

            while (bids.size() && asks.size()){
                auto& bid = bids.front(); 
                auto& ask = asks.front(); 

                Quantity quantity = std::min(bid->GetRemainingQuantity(), ask->GetRemainingQuantity()); 
                bid->Fill(quantity); 
                ask->Fill(quantity); 

                if (bid->isFilled()){
                    bids.pop_front(); 
                    orders_.erase(bid->GetOrderId()); 
                }

                if (bids.empty()) bids_.erase(bidprice); 
                if (asks.empty()) asks_.erase(askprice); 

                trades.push_back( Trade {
                TradeInfo{ bid->GetOrderId(), bid->GetPrice(), quantity}, 
                TradeInfo{ ask->GetOrderId(), ask->GetPrice(), quantity}
                });
            }
        }

        if (!bids_.empty()){
            auto& [_, bids] = *bids_.begin(); 
            auto& order = bids.front(); 
            if (order->GetOrderType()==OrderType::FillAndKill){
                CancelOrder(order->GetOrderId()); 
            }
        }

        if (!asks_.empty()){
            auto& [_, asks] = *asks_.begin(); 
            auto& order = asks.front(); 
            if (order->GetOrderType()==OrderType::FillAndKill){
                CancelOrder(order->GetOrderId()); 
            }
        }

        return trades; 
    }

public:
    Trades AddOrder(OrderPointer order){
        if (orders_.find(order->GetOrderId())!=orders_.end()) return {}; 
        if (order->GetOrderType()==OrderType::FillAndKill && !CanMatch(order->GetSide(), order->GetPrice())) return {}; 

        OrderPointers::iterator iterator; 

        if (order->GetSide() == Side::Buy){
            auto& orders = bids_[order->GetPrice()]; 
            orders.push_back(order);
            iterator = std::next(orders.begin(), orders.size()-1);  
        } else {
            auto& orders = asks_[order->GetPrice()]; 
            orders.push_back(order); 
            iterator = std::next(orders.begin(), orders.size()-1); 
        }

        orders_.insert({ order->GetOrderId(), OrderEntry{ order, iterator}}); 
        return MatchOrders(); 
    }

    void CancelOrder(OrderId orderId){
        if (orders_.find(orderId)!=orders_.end()) return; 

        const auto& [order, orderIterator] = orders_.at(orderId); 
        orders_.erase(orderId); 

        if (order->GetSide()==Side::Sell){
            auto price = order->GetPrice(); 
            auto& orders = asks_.at(price); 
            orders.erase(orderIterator); 
            if (orders.empty()) asks_.erase(price); 
        }
    }

    Trades MatchOrder(OrderModify order){
        if (orders_.find(order.GetOrderId())!=orders_.end()){
            return {}; 
        }

        const auto& [existingOrder, _] = orders_.at(order.GetOrderId()); 
        CancelOrder(order.GetOrderId()); 
        return AddOrder(order.ToOrderPointer(existingOrder->GetOrderType())); 
    }

    std::size_t Size() const { return orders_.size(); }

    OrderBookLevelInfos GetOrderInfos() const {
        LevelInfos bidInfos, askInfos; 
        bidInfos.reserve(orders_.size()); 
        askInfos.reserve(orders_.size()); 

        auto CreateLevelInfos = [](Price price, const OrderPointers& orders){
            return LevelInfo{ price, std::accumulate(orders.begin(), orders.end(), (Quantity)0, 
            [](std::size_t runningSum, const OrderPointer& order){
                return runningSum + order->GetRemainingQuantity(); 
            })}; 
        }; 

        for (const auto& [price, orders]: bids_) bidInfos.push_back(CreateLevelInfos(price, orders)); 
        for (const auto& [price, orders]: asks_) askInfos.push_back(CreateLevelInfos(price, orders)); 

        return OrderBookLevelInfos{ bidInfos, askInfos }; 
    }
}; 