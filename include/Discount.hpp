#pragma once

#include <string>
#include <map>
#include <optional>
#include <vector>

using namespace std;

struct Discount {
    optional<tuple<string, int, int>> totalPriceDiscount; 
    optional<pair<string, int>> firstOrderDiscount;
    map<string, pair<string, int>> foodDiscounts;
};