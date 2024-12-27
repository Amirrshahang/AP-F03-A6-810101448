#pragma once
#include "global.hpp"

#include <string>
#include <map>

class Restaurant {
private:
    string name;
    string district;
    map<string, int> foods; 
    string opening_time;  
    string closing_time; 
    string number_of_tables;

public:
    Restaurant(const string& name, const string& district, const string& foods, const string& opening_time, const string& closing_time, const string& number_of_tables);
    
    string getName() const;
    string getDistrict() const;

    void printRestaurantDetails() const;
};
