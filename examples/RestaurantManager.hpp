#pragma once

#include "DistrictManager.hpp"
#include "UserManager.hpp"
#include "Restaurant.hpp"
#include "Discount.hpp"
#include "global.hpp"

class RestaurantManager {
private:
    vector<std::unique_ptr<Restaurant>> restaurants;
    DistrictManager& districtManager;

public:
    RestaurantManager(DistrictManager& manager) : districtManager(manager) {} 
    void loadDiscountsFromCSV(const string& filename);
    Restaurant* getRestaurantByName(const string& name);
    Restaurant* findRestaurantByName(const string& name);
    void loadRestaurantsFromCSV(const string& filename);
    string showAllUserReservations(const string& username);
    string getRestaurantsByProximity(const string& userDistrict);
    void getRestaurantsByFood(const string& foodName, const string& userDistrict);
    void showUserReservations(const string& username, const string& restaurantName);
    bool hasUserTimeConflict(const string& username, int startTime, int endTime) const;
    bool isUserReservationConflict(const string& username, int startTime, int endTime) const;
    void showUserReservationById(const string& username, const string& restaurantName, const string& reserveId);
    void deleteReservation(const string& username, const string& restaurantName, int reserveId, UserManager& userManager);
    string reserveTable(const string& restaurantName, int tableId, int startTime, int endTime, const string& username, const vector<pair<string, int>>& orderedFoods, UserManager& userManager);

};