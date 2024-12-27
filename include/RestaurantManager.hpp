#pragma once

#include "DistrictManager.hpp"
#include "Restaurant.hpp"
#include "global.hpp"

class RestaurantManager {
private:
    vector<std::unique_ptr<Restaurant>> restaurants;
    DistrictManager& districtManager;

public:
    RestaurantManager(DistrictManager& manager) : districtManager(manager) {} 
    Restaurant* getRestaurantByName(const string& name);
    void getRestaurantsByProximity(const string& userDistrict);
    void listAllRestaurants() const;
    void loadRestaurantsFromCSV(const string& filename);
    void displayRestaurantsByDistrict(const string& userDistrict);

};
