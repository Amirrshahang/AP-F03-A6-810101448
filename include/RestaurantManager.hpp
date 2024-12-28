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
    Restaurant* findRestaurantByName(const string& name);
    void loadRestaurantsFromCSV(const string& filename);
    void showAllUserReservations(const string& username);
    void getRestaurantsByProximity(const string& userDistrict);
    void getRestaurantsByFood(const string& foodName, const string& userDistrict);
    void showUserReservations(const string& username, const string& restaurantName);
    bool hasUserTimeConflict(const string& username, int startTime, int endTime) const;
    bool isUserReservationConflict(const string& username, int startTime, int endTime) const;
    void deleteReservation(const string& username, const string& restaurantName, int reserveId);
    void showUserReservationById(const string& username, const string& restaurantName, const string& reserveId);
    int reserveTable(const string& restaurantName, int tableId, int startTime, int endTime, const string& username, const vector<pair<string, int>>& orderedFoods);

};