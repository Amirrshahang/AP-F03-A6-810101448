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
// void Restaurant::printReservations() const {
//     cout << "Reservations for " << name << ":\n";

//     if (reservations.empty()) {
//         cout << "No reservations found.\n";
//         return;
//     }

//     for (const auto& [reserveId, details] : reservations) {
//         const string& username = get<0>(details);
//         int tableId = get<1>(details);
//         int startTime = get<2>(details);
//         int endTime = get<3>(details);

//         cout << "Reservation ID: " << reserveId
//              << ", User: " << username
//              << ", Table: " << tableId
//              << ", Time: " << startTime << "-" << endTime
//              << endl;
//     }
// }

// void Restaurant::printTables() const {
//     cout << "Tables and Reservations for " << name << ":\n";

//     for (const auto& [tableId, reservations] : tables) {
//         cout << "Table " << tableId << ": ";

//         if (reservations.empty()) {
//             cout << "No reservations.";
//         } else {
//             for (const auto& [start, end] : reservations) {
//                 cout << "(" << start << "-" << end << ") ";
//             }
//         }

//         cout << endl;
//     }
// }

map<int, tuple<int, int, int, vector<pair<string, int>>>> Restaurant::getUserReservations(const string& username) const {
    map<int, tuple<int, int, int, vector<pair<string, int>>>> userReservations;

    for (const auto& [id, details] : reservations) {
        if (get<0>(details) == username) { 
            userReservations[id] = {get<1>(details), get<2>(details), get<3>(details), get<4>(details)};
        }
    }

    return userReservations;
}

string Restaurant::getName() const {return name;}

string Restaurant::getDistrict() const {return district;}

map<string, string> Restaurant::getFoods() const {return foods;}

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
        cout << tableNumber << ": ";
        for (size_t i = 0; i < reservations.size(); ++i) {
            cout << "(" << reservations[i].first << "-" << reservations[i].second << ")";
            if (i != reservations.size() - 1) {
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

int Restaurant::addReservation(int tableId, int startTime, int endTime, const string& username) {
    static int nextReserveId = 1;
    tables[tableId].emplace_back(startTime, endTime);
    reservations[nextReserveId++] = {username, tableId, startTime, endTime};
    return nextReserveId - 1;
}


