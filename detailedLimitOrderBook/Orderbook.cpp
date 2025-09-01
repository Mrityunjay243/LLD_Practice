#include "Orderbook.h"

#include <numeric>
#include <chrono>
#include <ctime>
#include <optional>

void OrderBook::PruneGoodForDayOrders(){
    using namespace std::chrono; 

    const auto end = hours(16); 

    while (true){
        const auto now = system_clock::now(); 
        const auto now_c = system_clock::to_time_t(now); 
        std::tm now_parts; 
        localtime_r(&now_c, &now_parts);

        if (now_parts.tm_hour >= end.count()){
            now_parts.tm_mday += 1; 
        }

        now_parts.tm_hour = end.count(); 
        now_parts.tm_min = 0; 
        now_parts.tm_sec = 0; 

        auto next = system_clock::from_time_t(mktime(&now_parts)); 
        auto till = next - now + milliseconds(100);

        {
            std::unique_lock orderLock{ ordersmutex_ }; 

            if (shutdown_.load(std::memory_order_acquire) || 
                shutdownCV_.wait_for(orderLock, till) == std::cv_status::no_timeout){
                    return; 
                }
        }

        OrderIds orderids; 

        {
            std::scoped_lock orderLock{ ordersmutex_ }; 

            for (const auto& [_, entry]: orders_){
                const auto& [order, _] = entry; 

                if (order->GetOrderType() != OrderType::GoodForDay){
                    continue;
                }

                orderids.push_back(order->GetOrderId());
            } 
        }

        CancelOrders(orderids);
    }   
}

void OrderBook::CancelOrders(OrderIds orderids){
    std::scoped_lock orderslock{ ordersmutex_ }; 
    
    for (const auto& orderid: orderids){
        CancelOrderInternal(orderid);
    }
}

void OrderBook::CancelOrderInternal(OrderId orderid){
    if (orders_.find(orderid)==orders_.end()){
        return; 
    }

    const auto [order, iterator] = orders_.at(orderid); 
    orders_.erase(orderid); 

    if (order->GetSide()==Side::Sell){
        auto price = order->GetPrice(); 
        auto& orders = asks_.at(price);
        orders.erase(iterator);
        if (orders.empty()){
            asks_.erase(price); 
        } 
    } else {
        auto price = order->GetPrice(); 
        auto& orders = bids_.at(price);
        orders.erase(iterator); 
        if (orders.empty()){
            bids_.erase(price);
        }
    }

    OnOrderCancelled(order); 
}

void OrderBook::OnOrderCancelled(OrderPointer order){
    UpdateLevelData(order->GetPrice(), order->GetRemainingQuantity(), LevelData::Action::Remove); 
}

void OrderBook::OnOrderAdded(OrderPointer order){
    UpdateLevelData(order->GetPrice(), order->GetInitialQuantity(), LevelData::Action::Add);
}

void OrderBook::UpdateLevelData(Price price, Quantity quantity, LevelData::Action action){
    auto& data = data_[price]; 

    data.count_ += action==LevelData::Action::Remove ? -1 : action==LevelData::Action::Add ? 1 : 0; 

    if (action==LevelData::Action::Add || action==LevelData::Action::Match){
        data.quantity_ -= quantity; 
    } else {
        data.quantity_ += quantity; 
    }

    if (data.count_==0){
        data_.erase(price);
    }
}

bool OrderBook::CanFullyFill(Side side, Price price, Quantity quantity) const{

    if (!CanMatch(side, price)){
        return false; 
    }

    std::optional<Price> threashold; 

    if (side == Side::Buy){
        const auto [askPrice, _] = *asks_.begin();
        threashold = askPrice; 
    } else {
        const auto [bidPrice, _] = *bids_.begin();
        threashold = bidPrice;
    }

    for (const auto& [levelprice, leveldata]: data_){
        if (threashold.has_value() && (
            (side==Side::Buy && threashold.value()>levelprice) ||
            (side==Side::Sell && threashold.value()<levelprice)
        )) continue;

        if ((side==Side::Buy && levelprice>price) || 
            (side==Side::Sell && levelprice<price)) continue;

        if (quantity<=leveldata.quantity_) return true;

        quantity -= leveldata.quantity_; 
    }

    return false;
}

bool OrderBook::CanMatch(Side side, Price price) const {
    if (side == Side::Buy){
        if (asks_.empty()){
            return false; 
        }

        const auto& [bestAsk, _] = *asks_.begin();
        return price>=bestAsk;  
    } else {
        if (bids_.empty()){
            return false;
        }

        const auto& [bestBid, _] = *bids_.begin();
        return price<=bestBid;
    }
}

Trades OrderBook::MatchOrders(){
    Trades trades; 

    trades.reserve(orders_.size());

    while (true){
        if (bids_.empty() || asks_.empty()) break; 

        auto& [bidprice, bids] = *bids_.begin();
        auto& [askprice, asks] = *asks_.begin();

        if (bidprice < askprice){
            break;
        }

        while (!bids.empty() && !asks.empty()){
            auto bid = bids.front();
            auto ask = asks.front();
            
            Quantity quantity = std::min(bid->GetRemainingQuantity(), ask->GetRemainingQuantity());
            bid->Fill(quantity);
            ask->Fill(quantity);

            if (bid->IsFilled()){
                bids.pop_front();
                orders_.erase(bid->GetOrderId());
            }

            if (ask->IsFilled()){
                asks.pop_front();
                orders_.erase(ask->GetOrderId());
            }

            trades.push_back( {
                TradeInfo{ bid->GetOrderId(), bid->GetPrice(), quantity}, 
                TradeInfo{ ask->GetOrderId(), ask->GetPrice(), quantity}
            });

            OnOrderMatched(bid->GetPrice(), quantity, bid->IsFilled());
            OnOrderMatched(ask->GetPrice(), quantity, ask->IsFilled());
        }

        if (bids.empty()){
            bids_.erase(bidprice);
            data_.erase(bidprice);
        }

        if (asks.empty()){
            asks_.erase(askprice);
            data_.erase(askprice);
        }
    }

    if (!bids_.empty()){
        auto& [_, bids] = *bids_.begin();
        auto& order = bids.front();
        if (order->GetOrderType()==OrderType::FillAndKill) CancelOrder(order->GetOrderId());
    }

    if (!asks_.empty()){
        auto& [_, asks] = *asks_.begin();
        auto& order = asks.front();
        if (order->GetOrderType()==OrderType::FillAndKill) CancelOrder(order->GetOrderId());
    }

    return trades;
    
}

OrderBook::OrderBook() : ordersPruneThread_{[this] { PruneGoodForDayOrders(); }} {}

OrderBook::~OrderBook(){
    shutdown_.store(true, std::memory_order_release);
    shutdownCV_.notify_one();
    ordersPruneThread_.join();
}

Trades OrderBook::AddOrder(OrderPointer order){
    std::scoped_lock orderslock{ ordersmutex_ }; 

    if (orders_.find(order->GetOrderId())!=orders_.end()){
        return {};
    }

    if (order->GetOrderType() == OrderType::Market){
        if (order->GetSide() == Side::Buy && !asks_.empty()){
            const auto& [worstask, _] = *asks_.rbegin();
            order->ToGoodTillCancel(worstask);
        } else if (order->GetSide() == Side::Sell && !bids_.empty()) {
            const auto& [worstbid, _] = *bids_.rbegin();
            order->ToGoodTillCancel(worstbid);
        }
    } else {
        return {};
    }

    if (order->GetOrderType()==OrderType::FillAndKill && !CanMatch(order->GetSide(), order->GetPrice())) return {};

    OrderPointers::iterator iterator; 

    if (order->GetSide() == Side::Buy){
        auto& orders = bids_[order->GetPrice()];
        orders.push_back(order);
        iterator = std::prev(orders.end());
    } else {
        auto& orders = asks_[order->GetPrice()];
        orders.push_back(order);
        iterator = std::prev(orders.end());
    }

    orders_.insert({order->GetOrderId(), OrderEntry{ order, iterator }}); 

    OnOrderAdded(order);

    return MatchOrders();
}

void OrderBook::CancelOrder(OrderId orderid){
    std::scoped_lock orderlock{ ordersmutex_ }; 
    CancelOrderInternal(orderid);
}

Trades OrderBook::ModifyOrder(OrderModify order){
    OrderType type; 

    {
        std::scoped_lock orderslock{ ordersmutex_ }; 

        if (orders_.find(order.GetOrderId())==orders_.end()){
            return {};
        }

        const auto& [existingorder, _] = orders_.at(order.GetOrderId());
        type = existingorder->GetOrderType(); 
    }

    CancelOrder(order.GetOrderId());
    return AddOrder(order.ToOrderPointer(type));
}

std::size_t OrderBook::Size() const { 
    std::scoped_lock orderslock{ ordersmutex_ }; 
    return orders_.size();
}

OrderBookLevelInfos OrderBook::GetOrderInfos() const {
    LevelInfos bidinfos, askinfos; 

    bidinfos.reserve(orders_.size());
    askinfos.reserve(orders_.size());

    auto CreateLevelInfos = [](Price price, const OrderPointers& orders){
        return LevelInfo{ price, std::accumulate(orders.begin(), orders.end(), (Quantity)0, 
        [](Quantity quantity, const OrderPointer& order){
            return quantity + order->GetRemainingQuantity(); 
        })
        };
    };

    for (const auto& [price, orders]: bids_){
        bidinfos.push_back(CreateLevelInfos(price, orders));
    }

    for (const auto& [price, orders]: asks_){
        askinfos.push_back(CreateLevelInfos(price, orders));
    }

    return OrderBookLevelInfos{ bidinfos, askinfos };
}