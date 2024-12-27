#include "Restaurant.hpp"
#include <sstream>
#include <iostream>

Restaurant::Restaurant(const string& name, const string& district, const string& foods_str, 
                       const string& opening_time, const string& closing_time, const string& number_of_tables)
    : name(name), district(district), opening_time(opening_time), closing_time(closing_time), number_of_tables(number_of_tables) {
    istringstream ss(foods_str);
    string food;
    while (getline(ss, food, ';')) {
        size_t pos = food.find(':');
        if (pos != string::npos) {
            string food_name = food.substr(0, pos);
            string price = food.substr(pos + 1);
            foods[food_name] = price;
        }
    }
}

string Restaurant::getName() const {return name;}

string Restaurant::getDistrict() const {return district;}

map<string, string> Restaurant::getFoods() const {return foods;}

void Restaurant::printRestaurantDetails() const {
    cout << "Restaurant: " << name << "\nDistrict: " << district << "\nFoods: ";
    for (const auto& food : foods) {
        cout << food.first << ": " << food.second << " "; 
    }
    cout << "\nOpening Time: " << opening_time << " Closing Time: " << closing_time
         << " Number of Tables: " << number_of_tables << endl;
}
