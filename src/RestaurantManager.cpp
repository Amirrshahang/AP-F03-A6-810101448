#include "RestaurantManager.hpp"
#include "Restaurant.hpp"
#include "UserManager.hpp"


using namespace std;

void RestaurantManager::loadRestaurantsFromCSV(const string& filename) {
    ifstream file(filename);

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
        cout << "Empty" << endl;
        return;
    }

    queue<string> districts;
    districts.push(userDistrict);

    set<string> visitedDistricts;
    visitedDistricts.insert(userDistrict);

    vector<Restaurant> allRestaurants;
    vector<Restaurant> unvisitedRestaurants;

    while (!districts.empty()) {
        string currentDistrict = districts.front();
        districts.pop();

        vector<Restaurant> filteredRestaurants;
        for (auto& restaurant : restaurants) {
            if (restaurant && restaurant->getDistrict() == currentDistrict) {
                filteredRestaurants.push_back(*restaurant);
            }
        }
        sort(filteredRestaurants.begin(), filteredRestaurants.end(), [](const Restaurant& a, const Restaurant& b) {
            return a.getName() < b.getName();
        });

        allRestaurants.insert(allRestaurants.end(), filteredRestaurants.begin(), filteredRestaurants.end());

        vector<string> nextDistricts = districtManager.getNeighbors(currentDistrict);
        for (auto& nextDistrict : nextDistricts) {
            if (visitedDistricts.find(nextDistrict) == visitedDistricts.end()) {
                districts.push(nextDistrict);
                visitedDistricts.insert(nextDistrict);
            }
        }
    }

    for (auto& restaurant : restaurants) {
        if (visitedDistricts.find(restaurant->getDistrict()) == visitedDistricts.end()) {
            unvisitedRestaurants.push_back(*restaurant);
        }
    }

    if (allRestaurants.empty()) {
    } else {
        for (auto& restaurant : allRestaurants) {
            cout << restaurant.getName() << " (" << restaurant.getDistrict() << ")" << endl;
        }
    }

    if (!unvisitedRestaurants.empty()) {
        sort(unvisitedRestaurants.begin(), unvisitedRestaurants.end(), [](const Restaurant& a, const Restaurant& b) {
            return a.getName() < b.getName();
        });

        for (auto& restaurant : unvisitedRestaurants) {
            cout << restaurant.getName() << " (" << restaurant.getDistrict() << ")" << endl;
        }
    }
}

void RestaurantManager::getRestaurantsByFood(const string& foodName, const string& userDistrict) {
    if (restaurants.empty()) {
        cout << "Empty" << endl;
        return;
    }

    queue<string> districts;
    districts.push(userDistrict);

    set<string> visitedDistricts;
    visitedDistricts.insert(userDistrict);

    vector<Restaurant> allRestaurants;
    vector<Restaurant> unvisitedRestaurants;

    while (!districts.empty()) {
        string currentDistrict = districts.front();
        districts.pop();

        vector<Restaurant> filteredRestaurants;
        for (auto& restaurant : restaurants) {
            if (restaurant && restaurant->getDistrict() == currentDistrict) {
                const auto& foods = restaurant->getFoods();
                if (foods.find(foodName) != foods.end()) {
                    filteredRestaurants.push_back(*restaurant);
                }
            }
        }
        sort(filteredRestaurants.begin(), filteredRestaurants.end(), [](const Restaurant& a, const Restaurant& b) {
            return a.getName() < b.getName();
        });

        allRestaurants.insert(allRestaurants.end(), filteredRestaurants.begin(), filteredRestaurants.end());

        vector<string> nextDistricts = districtManager.getNeighbors(currentDistrict);
        for (auto& nextDistrict : nextDistricts) {
            if (visitedDistricts.find(nextDistrict) == visitedDistricts.end()) {
                districts.push(nextDistrict);
                visitedDistricts.insert(nextDistrict);
            }
        }
    }

    for (auto& restaurant : restaurants) {
        if (visitedDistricts.find(restaurant->getDistrict()) == visitedDistricts.end()) {
            const auto& foods = restaurant->getFoods();
            if (foods.find(foodName) != foods.end()) {
                unvisitedRestaurants.push_back(*restaurant);
            }
        }
    }

    if (allRestaurants.empty() && unvisitedRestaurants.empty()) {
        cout << "Empty" << endl;
        return;
    }

    for (auto& restaurant : allRestaurants) {
        cout << restaurant.getName() << " (" << restaurant.getDistrict() << ")" << endl;
    }

    if (!unvisitedRestaurants.empty()) {
        sort(unvisitedRestaurants.begin(), unvisitedRestaurants.end(), [](const Restaurant& a, const Restaurant& b) {
            return a.getName() < b.getName();
        });

        for (auto& restaurant : unvisitedRestaurants) {
            cout << restaurant.getName() << " (" << restaurant.getDistrict() << ")" << endl;
        }
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
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool RestaurantManager::isUserReservationConflict(const string& username, int startTime, int endTime) const {
    for (const auto& restaurant : restaurants) {
        const auto& userReservations = restaurant->getUserReservations(username);

        for (const auto& [id, details] : userReservations) {
            int reservedStart = get<1>(details);
            int reservedEnd = get<2>(details);

            if (!(endTime <= reservedStart || startTime >= reservedEnd)) {
                return true;
            }
        }
    }
    return false;
}

int RestaurantManager::reserveTable(const string& restaurantName, int tableId, int startTime, int endTime, const string& username, const vector<pair<string, int>>& orderedFoods) {
    Restaurant* restaurant = findRestaurantByName(restaurantName);
    if (!restaurant) {
        throw runtime_error("Not Found");
    }

    if (!restaurant->isTableAvailable(tableId)) {
        throw runtime_error("Not Found");
    }

    if (!restaurant->isTimeSlotAvailable(tableId, startTime, endTime)) {
        throw runtime_error("Permission Denied");
    }

    if (isUserReservationConflict(username, startTime, endTime)) {
        throw runtime_error("Permission Denied");
    }

    int totalPrice = 0;
    vector<pair<string, int>> validFoods;

    for (const auto& [foodName, count] : orderedFoods) {
        const auto& foods = restaurant->getFoods();
        auto it = foods.find(foodName);
        if (it == foods.end()) {
            throw runtime_error("Not Found");
        }
        totalPrice += stoi(it->second) * count;
        validFoods.emplace_back(foodName, count);
    }

    int reserveId = restaurant->addReservation(tableId, startTime, endTime, username, validFoods);

    cout << "Reserve ID: " << reserveId << endl;
    cout << "Table " << tableId << " for " << startTime << " to " << endTime << " in " << restaurantName << endl;
    cout << "Price: " << totalPrice << endl;

    return reserveId;
}

void RestaurantManager::showAllUserReservations(const string& username) {
    vector<tuple<int, string, int, int, int, vector<pair<string, int>>>> reservations;

    for (const auto& restaurant : restaurants) {
        const auto& userReservations = restaurant->getUserReservations(username);

        for (const auto& [id, details] : userReservations) {
            int tableId = get<0>(details);
            int startTime = get<1>(details);
            int endTime = get<2>(details);
            const auto& foods = get<3>(details);

            reservations.emplace_back(
                id,
                restaurant->getName(),
                tableId,
                startTime,
                endTime,
                foods
            );
        }
    }

    if (reservations.empty()) {
        throw runtime_error("Empty");
    }

    sort(reservations.begin(), reservations.end(), [](const auto& a, const auto& b) {
        return get<0>(a) < get<0>(b); 
    });

    for (const auto& [id, restaurant, tableId, startTime, endTime, foods] : reservations) {
        cout << id << ": " << restaurant << " " << tableId << " " << startTime << "-" << endTime;

        if (!foods.empty()) {
            for (const auto& [foodName, count] : foods) {
                cout << " " << foodName << "(" << count << ")";
            }
        }

        cout << endl;
    }
}

void RestaurantManager::showUserReservations(const string& username, const string& restaurantName) {
    Restaurant* restaurant = findRestaurantByName(restaurantName);
    if (!restaurant) {
        throw runtime_error("Not Found");
    }

    const auto& userReservations = restaurant->getUserReservations(username);
    if (userReservations.empty()) {
        throw runtime_error("Empty");
    }

    for (const auto& [id, details] : userReservations) {
        int tableId = get<0>(details);
        int startTime = get<1>(details);
        int endTime = get<2>(details);
        const auto& foods = get<3>(details);

        cout << id << ": " << restaurantName << " " << tableId << " " << startTime << "-" << endTime;

        if (!foods.empty()) {
            for (const auto& [foodName, count] : foods) {
                cout << " " << foodName << "(" << count << ")";
            }
        }

        cout << endl;
    }
}

void RestaurantManager::showUserReservationById(const string& username, const string& restaurantName, const string& reserveId) {
    Restaurant* restaurant = findRestaurantByName(restaurantName);
    if (!restaurant) {
        throw runtime_error("Not Found");
    }

    int reservationId = stoi(reserveId);

    const auto& userReservations = restaurant->getUserReservations(username);
    auto it = userReservations.find(reservationId);
    if (it == userReservations.end()) {
        throw runtime_error("Permission Denied");
    }

    const auto& details = it->second;
    int tableId = get<0>(details);
    int startTime = get<1>(details);
    int endTime = get<2>(details);
    const auto& foods = get<3>(details);

    cout << reservationId << ": " << restaurantName << " " << tableId << " " << startTime << "-" << endTime;

    if (!foods.empty()) {
        for (const auto& [foodName, count] : foods) {
            cout << " " << foodName << "(" << count << ")";
        }
    }

    cout << endl;
}

void RestaurantManager::deleteReservation(const string& username, const string& restaurantName, int reserveId) {
    Restaurant* restaurant = findRestaurantByName(restaurantName);
    if (!restaurant) {
        throw runtime_error("Not Found");
    }

    if (!restaurant->isReservationExists(reserveId)) {
        throw runtime_error("Not Found");
    }

    if (!restaurant->isReservationOwnedByUser(reserveId, username)) {
        throw runtime_error("Permission Denied");
    }

    restaurant->removeReservation(reserveId);

    cout << "OK "<<endl;
}
