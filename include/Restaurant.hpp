#pragma once
#include "global.hpp"

#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <utility>

using namespace std;

class Restaurant {
private:
    string name;
    string district;
    string opening_time;
    string closing_time;
    string number_of_tables;
    int nextReserveId = 1;
    map<string, string> foods;
    map<int, vector<pair<int, int>>> tables;
    map<int, tuple<string, int, int, int, vector<pair<string, int>>>> reservations;

public:
    Restaurant(const string& name, const string& district, const string& foods, const string& opening_time, const string& closing_time, const string& number_of_tables);

    string getName() const;
    string getDistrict() const;
    map<string, string> getFoods() const;
    map<int, vector<pair<int, int>>> getAllReservations() const;
    map<int, tuple<int, int, int, vector<pair<string, int>>>> getUserReservations(const string& username) const;

    void printTables() const;  
    void printReservations() const;
    void printRestaurantDetails() const;
    void removeReservation(int reserveId);
    bool isTableAvailable(int tableId) const;
    bool isReservationExists(int reserveId) const;
    bool isTimeSlotAvailable(int tableId, int startTime, int endTime) const;
    bool isReservationOwnedByUser(int reserveId, const string& username) const;
    int addReservation(int tableId, int startTime, int endTime, const string& username, const vector<pair<string, int>>& orderedFoods);



};
