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
    getline(file, line);
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

    // Initialize BFS queue and visited set
    queue<string> districts;
    districts.push(userDistrict);

    set<string> visitedDistricts;
    visitedDistricts.insert(userDistrict);

    vector<Restaurant> allRestaurants;
    vector<Restaurant> unvisitedRestaurants;  // Stores restaurants that are not in any neighborhood

    while (!districts.empty()) {
        string currentDistrict = districts.front();
        districts.pop();

        // Collect restaurants for the current district
        vector<Restaurant> filteredRestaurants;
        for (auto& restaurant : restaurants) {
            if (restaurant && restaurant->getDistrict() == currentDistrict) {
                filteredRestaurants.push_back(*restaurant);  // Deselect restaurant pointer
            }
        }

        // Sort filtered restaurants alphabetically
        sort(filteredRestaurants.begin(), filteredRestaurants.end(), [](const Restaurant& a, const Restaurant& b) {
            return a.getName() < b.getName();
        });

        // Add the filtered restaurants to the result list
        allRestaurants.insert(allRestaurants.end(), filteredRestaurants.begin(), filteredRestaurants.end());

        // Find neighboring districts and push them to the queue
        vector<string> nextDistricts = districtManager.getNeighbors(currentDistrict);
        for (auto& nextDistrict : nextDistricts) {
            if (visitedDistricts.find(nextDistrict) == visitedDistricts.end()) {
                districts.push(nextDistrict);
                visitedDistricts.insert(nextDistrict);
            }
        }
    }

    // Check and collect any restaurants that are not in any of the visited districts
    for (auto& restaurant : restaurants) {
        if (visitedDistricts.find(restaurant->getDistrict()) == visitedDistricts.end()) {
            unvisitedRestaurants.push_back(*restaurant);
        }
    }

    // Display all the collected restaurants
    if (allRestaurants.empty()) {
        cout << "No restaurants found in the nearby districts." << endl;
    } else {
        cout << "Restaurants in the nearby districts:\n";
        for (auto& restaurant : allRestaurants) {
            cout << restaurant.getName() << " (" << restaurant.getDistrict() << ")" << endl;
        }
    }

    // Display the remaining unvisited restaurants (not in any neighbor districts)
    if (!unvisitedRestaurants.empty()) {
        cout << "\nRestaurants not in any nearby districts:\n";
        sort(unvisitedRestaurants.begin(), unvisitedRestaurants.end(), [](const Restaurant& a, const Restaurant& b) {
            return a.getName() < b.getName();
        });

        for (auto& restaurant : unvisitedRestaurants) {
            cout << restaurant.getName() << " (" << restaurant.getDistrict() << ")" << endl;
        }
    }
}

void RestaurantManager::getRestaurantsByFood(const string& foodName) {
    vector<Restaurant> filteredRestaurants;

   for (auto& restaurant : restaurants) {
    map<string, string> foods = restaurant->getFoods();
        for (const auto& food : foods) {
            if (food.first.find(foodName) != string::npos) {
                filteredRestaurants.push_back(*restaurant);   
                break;
            }
        }
    }

    if (filteredRestaurants.empty()) {
        cout << "Empty"<<endl;
        return;
    }

    sort(filteredRestaurants.begin(), filteredRestaurants.end(), [](const Restaurant& a, const Restaurant& b) {
        return a.getName() < b.getName();
    });

    for (auto& restaurant : filteredRestaurants) {
        cout << restaurant.getName() << " (" << restaurant.getDistrict() << ")" << endl;
    }
}

Restaurant* RestaurantManager::findRestaurantByName(const string& name) {
    for (const auto& restaurant : restaurants) {
        if (restaurant && restaurant->getName() == name) {
            return restaurant.get(); 
        }
    }
    throw runtime_error("Not Found");
}

bool RestaurantManager::hasUserTimeConflict(const string& username, int startTime, int endTime) const {
    for (const auto& restaurant : restaurants) {
        if (restaurant) {
            for (const auto& [tableId, times] : restaurant->getAllReservations()) {
                for (const auto& [reservedStart, reservedEnd] : times) {
                    if (!(endTime <= reservedStart || startTime >= reservedEnd)) {
                        return true; // تداخل زمانی
                    }
                }
            }
        }
    }
    return false;
}

int RestaurantManager::reserveTable(const string& restaurantName, int tableId, int startTime, int endTime, const string& username, const vector<string>& orderedFoods) {
    Restaurant* restaurant = findRestaurantByName(restaurantName);
    if (!restaurant) {
        throw runtime_error("Not Found: Restaurant does not exist");
    }

    if (!restaurant->isTableAvailable(tableId)) {
        throw runtime_error("Not Found: Table does not exist");
    }

    if (!restaurant->isTimeSlotAvailable(tableId, startTime, endTime)) {
        throw runtime_error("Permission Denied: Time slot not available");
    }

    int totalPrice = 0;
    if (!orderedFoods.empty()) { 
        for (const auto& food : orderedFoods) {
            const auto& foods = restaurant->getFoods();
            auto it = foods.find(food);
            if (it == foods.end()) {
                throw runtime_error("Not Found: Food '" + food + "' not available in the restaurant");
            }       
            totalPrice += stoi(it->second);
        }
    }

    int reserveId = restaurant->addReservation(tableId, startTime, endTime, username);

    cout << "Reserve ID: " << reserveId << endl;
    cout << "Table " << tableId << " for " << startTime << " to " << endTime << " in " << restaurantName << endl;
    cout << "Price: " << totalPrice << endl;

    return reserveId;
}

void RestaurantManager::showUserReservations(const string& username, const string& restaurantName, const string& reserveId) {
    vector<tuple<int, string, int, int, vector<pair<string, int>>>> reservations;

    Restaurant* restaurant = findRestaurantByName(restaurantName);
    if (!restaurant) {
        throw runtime_error("Not Found: Restaurant does not exist");
    }

    const auto& userReservations = restaurant->getUserReservations(username);

    for (const auto& [id, details] : userReservations) {
        if (!reserveId.empty() && to_string(id) != reserveId) {
            continue;
        }

        int tableId = get<0>(details);
        int startTime = get<1>(details);
        int endTime = get<2>(details);
        const auto& foods = get<3>(details);

        reservations.emplace_back(id, restaurant->getName(), tableId, startTime, endTime, foods);
    }

    if (reservations.empty()) {
        throw runtime_error("Not Found");
    }

    for (const auto& [id, restaurant, tableId, startTime, endTime, foods] : reservations) {
        cout << id << ": " << restaurant << " " << tableId << " " << startTime << "-" << endTime;
        for (const auto& [food, count] : foods) {
            cout << " " << food << "(" << count << ")";
        }
        cout << endl;
    }
}

