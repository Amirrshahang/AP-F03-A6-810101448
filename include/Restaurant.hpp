#pragma once
#include "global.hpp"

#include <string>
#include <map>

class Restaurant {
private:
    string name;
    string district;
    string opening_time;  
    string closing_time; 
    string number_of_tables;
    map<string, string> foods; 
    map<int, vector<pair<int, int>>> tables;
    map<int, tuple<string, int, int, int>> reservations;

public:
    Restaurant(const string& name, const string& district, const string& foods, const string& opening_time, const string& closing_time, const string& number_of_tables);
     map<string, string> getFoods() const;

    string getName() const;
    string getDistrict() const;
    map<int, vector<pair<int, int>>> getAllReservations() const;
    vector<string> getNextDistricts() const;
    void printTables() const;
    void printReservations() const;
    void printRestaurantDetails() const;
    bool isTableAvailable(int tableId) const;
    bool isTimeSlotAvailable(int tableId, int startTime, int endTime) const;
    int  addReservation(int tableId, int startTime, int endTime, const string& username);
    map<int, tuple<int, int, int, vector<pair<string, int>>>> getUserReservations(const string& username) const;




};
