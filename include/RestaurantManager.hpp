#pragma once

#include "Restaurant.hpp"
#include "global.hpp"

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>

class RestaurantManager {
private:
    vector<Restaurant> restaurants;

public:
    void addRestaurant(const Restaurant& restaurant);
    Restaurant* getRestaurantByName(const string& name);
    void listAllRestaurants() const;
    void loadRestaurantsFromCSV(const string& filename);
};
