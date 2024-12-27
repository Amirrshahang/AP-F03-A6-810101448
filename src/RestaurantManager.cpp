#include "RestaurantManager.hpp"
#include "Restaurant.hpp"
#include <memory>
#include<set>

using namespace std;

void RestaurantManager::loadRestaurantsFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Could not open the file!");
    }

    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        string name, district, foods, opening_time, closing_time, number_of_tables;

        if (!getline(ss, name, ',') || 
            !getline(ss, district, ',') || 
            !getline(ss, foods, ',') || 
            !getline(ss, opening_time, ',') || 
            !getline(ss, closing_time, ',') || 
            !getline(ss, number_of_tables, ',')) {
            cerr << "Invalid line format: " << line << endl;
            continue;
        }

        auto newRestaurant = std::make_unique<Restaurant>(name, district, foods, opening_time, closing_time, number_of_tables);
        restaurants.push_back(std::move(newRestaurant));
    }

    file.close();
}

void RestaurantManager::getRestaurantsByProximity(const string& userDistrict) {
    if (restaurants.empty()) {
        cout << "No restaurants loaded." << endl;
        return;
    }

    queue<string> districts;
    districts.push(userDistrict);

    set<string> visitedDistricts;
    visitedDistricts.insert(userDistrict);

    while (!districts.empty()) {
        string currentDistrict = districts.front();
        districts.pop();

        vector<Restaurant> filteredRestaurants;

        for (auto& restaurant : restaurants) {
            if (restaurant && restaurant->getDistrict() == currentDistrict) {
                filteredRestaurants.push_back(*restaurant);  // دقت کنید که از *restaurant استفاده کنید
            }
        }

        sort(filteredRestaurants.begin(), filteredRestaurants.end(), [](const Restaurant& a, const Restaurant& b) {
            return a.getName() < b.getName();
        });

        for (auto& restaurant : filteredRestaurants) {
            cout << restaurant.getName() << " (" << restaurant.getDistrict() << ")" << endl;
        }

        vector<string> nextDistricts = districtManager.getNeighbors(currentDistrict);

        for (auto& nextDistrict : nextDistricts) {
            if (visitedDistricts.find(nextDistrict) == visitedDistricts.end()) {
                districts.push(nextDistrict);
                visitedDistricts.insert(nextDistrict);
            }
        }
    }
}


void RestaurantManager::displayRestaurantsByDistrict(const string& userDistrict) {
    if (restaurants.empty()) {
        cout << "No restaurants loaded." << endl;
        return;
    }

    vector<string> districts = { userDistrict };

    while (!districts.empty()) {
        vector<std::unique_ptr<Restaurant>> filteredRestaurants;

        for (auto& restaurant : restaurants) {
            if (find(districts.begin(), districts.end(), restaurant->getDistrict()) != districts.end()) {
                filteredRestaurants.push_back(std::move(restaurant));
            }
        }

        sort(filteredRestaurants.begin(), filteredRestaurants.end(), [](const std::unique_ptr<Restaurant>& a, const std::unique_ptr<Restaurant>& b) {
            return a->getName() < b->getName();
        });

        for (auto& restaurant : filteredRestaurants) {
            cout << restaurant->getName() << endl;
        }

        string nextDistrict;
        bool foundNext = false;

        for (auto& restaurant : filteredRestaurants) {
            if (find(districts.begin(), districts.end(), restaurant->getDistrict()) == districts.end()) {
                nextDistrict = restaurant->getDistrict();
                foundNext = true;
                break;
            }
        }

        if (foundNext) {
            districts.push_back(nextDistrict);
        } else {
            break;
        }
    }
}

Restaurant* RestaurantManager::getRestaurantByName(const string& name) {
    for (auto& restaurant : restaurants) {
        if (restaurant->getName() == name) {
            return restaurant.get();  // استفاده از get() برای دسترسی به Restaurant
        }
    }
    return nullptr;
}

void RestaurantManager::listAllRestaurants() const {
    for (const auto& restaurant : restaurants) {
        cout << "Name: " << restaurant->getName() << ", District: " << restaurant->getDistrict() << endl;
    }
}
