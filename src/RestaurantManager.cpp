#include "RestaurantManager.hpp"
#include "Restaurant.hpp"
#include <fstream>
#include <sstream>

void RestaurantManager::loadRestaurantsFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Could not open the file!");
    }

    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        string name, district, foods, opening_time, closing_time, number_of_tables;

        getline(ss, name, ',');
        getline(ss, district, ',');
        getline(ss, foods, ',');
        getline(ss, opening_time, ',');
        getline(ss, closing_time, ',');
        getline(ss, number_of_tables, ',');

        Restaurant restaurant(name, district, foods, opening_time, closing_time, number_of_tables);
        addRestaurant(restaurant);

        cout << "Loaded restaurant: " << name << " in district " << district << endl;
    }
}



void RestaurantManager::addRestaurant(const Restaurant& restaurant) {
    restaurants.push_back(restaurant);
}

Restaurant* RestaurantManager::getRestaurantByName(const string& name) {
    for (auto& restaurant : restaurants) {
        if (restaurant.getName() == name) {
            return &restaurant;
        }
    }
    return nullptr;
}

void RestaurantManager::listAllRestaurants() const {
    for (const auto& restaurant : restaurants) {
        cout << "Name: " << restaurant.getName() << ", District: " << restaurant.getDistrict() << endl;
    }
}

