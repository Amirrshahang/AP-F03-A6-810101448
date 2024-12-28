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
    int totalTables = stoi(number_of_tables);
    for (int i = 1; i <= totalTables; ++i) {
        tables[i] = {};
    }
}

string Restaurant::getName() const { return name; }

string Restaurant::getDistrict() const { return district; }

map<string, string> Restaurant::getFoods() const { return foods; }

map<int, tuple<int, int, int, vector<pair<string, int>>>> Restaurant::getUserReservations(const string& username) const {
    map<int, tuple<int, int, int, vector<pair<string, int>>>> userReservations;

    for (const auto& [id, details] : reservations) {
        if (get<0>(details) == username) {  
            userReservations[id] = {
                get<1>(details), 
                get<2>(details), 
                get<3>(details), 
                get<4>(details) 
            };
        }
    }

    return userReservations;
}

void Restaurant::printRestaurantDetails() const {
    cout << "Name: " << name << endl;
    cout << "District: " << district << endl;
    cout << "Time: " << opening_time << "-" << closing_time << endl;
    cout << "Menu: ";
    vector<pair<string, string>> sortedFoods(foods.begin(), foods.end());
    sort(sortedFoods.begin(), sortedFoods.end());

    for (size_t i = 0; i < sortedFoods.size(); ++i) {
        cout << sortedFoods[i].first << "(" << sortedFoods[i].second << ")";
        if (i != sortedFoods.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;

    for (const auto& [tableNumber, reservations] : tables) {
        vector<pair<int, int>> sortedReservations = reservations;
        sort(sortedReservations.begin(), sortedReservations.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.first < b.first;
        });

        cout << tableNumber << ": ";
        for (size_t i = 0; i < sortedReservations.size(); ++i) {
            cout << "(" << sortedReservations[i].first << "-" << sortedReservations[i].second << ")";
            if (i != sortedReservations.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

bool Restaurant::isTableAvailable(int tableId) const {
    return tables.find(tableId) != tables.end();
}

map<int, vector<pair<int, int>>> Restaurant::getAllReservations() const {
    return tables;
}

bool Restaurant::isTimeSlotAvailable(int tableId, int startTime, int endTime) const {
    const auto& reservations = tables.at(tableId);
    for (const auto& [reservedStart, reservedEnd] : reservations) {
        if (!(endTime <= reservedStart || startTime >= reservedEnd)) {
            return false;
        }
    }
    return true;
}

int Restaurant::addReservation(int tableId, int startTime, int endTime, const string& username, const vector<pair<string, int>>& orderedFoods) {
    if (!isTableAvailable(tableId)) {
        throw runtime_error("Not Found: Table does not exist");
    }
    if (!isTimeSlotAvailable(tableId, startTime, endTime)) {
        throw runtime_error("Permission Denied: Time slot is not available");
    }

    tables[tableId].emplace_back(startTime, endTime);

    reservations[nextReserveId] = make_tuple(username, tableId, startTime, endTime, orderedFoods);

    return nextReserveId++;
}

bool Restaurant::isReservationExists(int reserveId) const {
    return reservations.find(reserveId) != reservations.end();
}

bool Restaurant::isReservationOwnedByUser(int reserveId, const string& username) const {
    auto it = reservations.find(reserveId);
    if (it == reservations.end()) return false;
    return get<0>(it->second) == username;
}

void Restaurant::removeReservation(int reserveId) {
    auto it = reservations.find(reserveId);
    if (it == reservations.end()) {
        throw runtime_error("Not Found");  
    }

    int tableId = get<1>(it->second);
    int startTime = get<2>(it->second);
    int endTime = get<3>(it->second);

    auto& tableReservations = tables[tableId];
    tableReservations.erase(
        remove(tableReservations.begin(), tableReservations.end(), make_pair(startTime, endTime)),
        tableReservations.end()
    );

    reservations.erase(it);
}