#pragma once

#include "global.hpp"

struct Discount {
    optional<tuple<string, int, int>> totalPriceDiscount; 
    optional<pair<string, int>> firstOrderDiscount;
    map<string, pair<string, int>> foodDiscounts;
};