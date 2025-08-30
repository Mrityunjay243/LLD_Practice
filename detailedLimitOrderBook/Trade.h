#pragma once

#include "TradeInfo.h"

class Trade{
public:
    Trade(const TradeInfo& bidtrade, const TradeInfo& asktrade)
    : bidtrade_{ bidtrade }
    , asktrade_{ asktrade }
    {}

    const TradeInfo& GetBidInfo() const { return bidtrade_; }
    const TradeInfo& GetAskInfo() const { return asktrade_; }

private:
    TradeInfo bidtrade_; 
    TradeInfo asktrade_; 
};

using Trades = std::vector<Trade>; 