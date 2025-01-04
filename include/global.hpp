#pragma once

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <optional>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <utility>
#include <random>
#include <string>
#include <vector>
#include <memory>
#include <math.h>
#include <tuple>
#include <queue>
#include <ctime>
#include <regex>
#include <set>
#include <map>

using namespace std;

inline const map<string, regex> regexPatterns = {
    {"signup", regex(R"(^POST\s+signup\s+\?\s+username\s+\"([a-zA-Z0-9_]+)\"\s+password\s+\"([a-zA-Z0-9_]+)\"(?:\s+.*)?$)")},
    {"login", regex(R"(^POST\s+login\s+\?\s+username\s+\"([a-zA-Z0-9_]+)\"\s+password\s+\"([a-zA-Z0-9_]+)\"(?:\s+.*)?$)")},
    {"logout", regex(R"(^POST\s+logout\s+\?(?:\s+.*)?$)")},
    {"districts", regex(R"(^GET\s+districts\s+\?(?!.*district).*$)")},
    {"districtDetail", regex(R"(^GET\s+districts\s+\?\s+district\s+\"([a-zA-Z0-9_ ]+)\"(?:\s+.*)?$)")},
    {"putDistrict", regex(R"(^PUT\s+my_district\s+\?\s+district\s+\"([a-zA-Z0-9_ ]+)\"(?:\s+.*)?$)")},
    {"getRestaurants", regex(R"(^GET\s+restaurants\s+\?(?!.*\bfood_name\b).*$)")},
    {"getRestaurantsByFood", regex(R"(^GET\s+restaurants\s+\?\s+food_name\s+\"([a-zA-Z0-9_ ]+)\"(?:\s+.*)?$)")},
    {"showrestaurantDetail", regex(R"(^GET\s+restaurant_detail\s+\?\s+restaurant_name\s+\"([a-zA-Z0-9_ ]+)\"(?:\s+.*)?$)")},
    {"reserveWithFoods", regex(R"(^POST\s+reserve\s+\?\s+restaurant_name\s+\"([a-zA-Z0-9_ ]+)\"\s+table_id\s+\"([0-9]+)\"\s+start_time\s+\"([0-9]+)\"\s+end_time\s+\"([0-9]+)\"\s*(?:foods\s+\"([a-zA-Z0-9_, ]*)\")?(?:\s+.*)?$)")},
    {"showreserves", regex(R"(^GET\s+reserves\s*\?(?!.*\b(restaurant_name|reserve_id)\b).*$)")},
	{"showreservesWithDetail", regex(R"(^GET\s+reserves\s+\?\s*restaurant_name\s+\"([^\"]+)\"\s*(?:reserve_id\s+\"([^\"]+)\")?(?:\s+.*)?$)")},
    {"deleteReserve", regex(R"(^DELETE\s+reserve\s+\?\s+restaurant_name\s+\"([^\"]+)\"\s+reserve_id\s+\"([^\"]+)\"(?:\s+.*)?$)")},
    {"BadRequest", regex(R"(^GET\s+reserves\s+\?\s+reserve_id\s+\"([0-9]+)\"$)")},
    {"increaseBudget", regex(R"(^POST\s+increase_budget\s+\?\s+amount\s+.+$)")},
    {"showBudget", regex(R"(^GET\s+show_budget\s+\?$)")}
};

static const regex numericPattern(R"(^-?\d+(\.\d+)?$)");
const string CSV_PATH = "/Users/amir/Desktop/AP/project/A6/File/";
const size_t FOOD_NAME_LENGTH = 11;
constexpr int AMOUNT_PREFIX_LENGTH = 7;
