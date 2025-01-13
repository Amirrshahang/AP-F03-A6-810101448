#pragma once
#include "global.hpp"
#include "Discount.hpp"

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
    map<int, tuple<string, int, int, int, vector<pair<string, int>>, int, int>> reservations;


    Discount discount;

public:
    Restaurant(const string& name, const string& district, const string& foods, const string& opening_time, const string& closing_time, const string& number_of_tables);

    string getName() const;
    void printTables() const; 
    string getDistrict() const; 
    void printReservations() const;
    const Discount& getDiscount() const;
    string printRestaurantDetails() const;
    map<string, string> getFoods() const;
    bool isTableAvailable(int tableId) const;
    void setDiscount(const Discount& discount);
    bool isReservationExists(int reserveId) const;
    void removeReservation(int reserveId, int& finalPrice);
    map<int, vector<pair<int, int>>> getAllReservations() const;
    bool isFirstReservationForUser(const string& username) const;
    bool isTimeSlotAvailable(int tableId, int startTime, int endTime) const;
    bool isReservationOwnedByUser(int reserveId, const string& username) const;
    map<int, tuple<int, int, int, vector<pair<string, int>>, int, int>> getUserReservations(const string& username) const;
    int addReservation(int tableId, int startTime, int endTime, const string& username, const vector<pair<string, int>>& orderedFoods,int finalPrice,int totalPrice);
};