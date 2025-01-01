
#include "global.hpp"

class DiscountBase {
public:
    virtual int calculateDiscount(int originalPrice) const = 0;
    virtual string getDescription() const = 0;
    virtual ~DiscountBase() = default;
};




class TotalPriceDiscount : public DiscountBase {
private:
    string type;
    int minimum;
    int value;

public:
    TotalPriceDiscount(string type, int minimum, int value)
        : type(type), minimum(minimum), value(value) {}

    int calculateDiscount(int originalPrice) const override {
        if (originalPrice <= minimum) return 0;
        return (type == "percent") ? (originalPrice * value) / 100 : value;
    }

    string getDescription() const override {
        return string(type == "percent" ? "percentage" : "amount") +
               ", Minimum: " + to_string(minimum) +
               ", Value: " + to_string(value);
    }
};



class FirstOrderDiscount : public DiscountBase {
private:
    string type;
    int value;

public:
    FirstOrderDiscount(string type, int value) : type(type), value(value) {}

    int calculateDiscount(int originalPrice) const override {
        return (type == "percent") ? (originalPrice * value) / 100 : value;
    }

    string getDescription() const override {
        return string(type == "percent" ? "percentage" : "amount") +
               ", Value: " + to_string(value);
    }
};



class ItemSpecificDiscount : public DiscountBase {
private:
    map<string, pair<string, int>> itemDiscounts;
    vector<pair<string, int>> orderedFoods;

public:
    ItemSpecificDiscount(const map<string, pair<string, int>>& itemDiscounts, const vector<pair<string, int>>& orderedFoods)
        : itemDiscounts(itemDiscounts), orderedFoods(orderedFoods) {}
    
    int calculateValue(const string& type, int baseValue, int factor) const {
        return (type == "percent") ? (baseValue * factor) / 100 : factor;
    }  

    int calculateDiscount(int originalPrice) const override {
        int totalDiscount = 0;
        for (const auto& [foodName, count] : orderedFoods) {
            auto it = itemDiscounts.find(foodName);
            if (it != itemDiscounts.end()) {
                const auto& [type, value] = it->second;
                totalDiscount += calculateValue(type, originalPrice, value * count);
            }
        }
        return totalDiscount;
    }
    string getDescription() const override {
        string description = "Item Specific Discounts: ";
        for (const auto& [foodName, discount] : itemDiscounts) {
            description += foodName + "(" + discount.first + ": " + to_string(discount.second) + "), ";
        }
        return description;
    }
};
