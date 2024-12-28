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
    throw runtime_error("Not Found: Restaurant does not exist");
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
        throw runtime_error("Not Found: Restaurant does not exist");
    }

    if (!restaurant->isTableAvailable(tableId)) {
        throw runtime_error("Not Found: Table does not exist");
    }

    if (!restaurant->isTimeSlotAvailable(tableId, startTime, endTime)) {
        throw runtime_error("Permission Denied: Time slot not available");
    }

    if (isUserReservationConflict(username, startTime, endTime)) {
        throw runtime_error("Permission Denied: User has a conflicting reservation");
    }

    int totalPrice = 0;
    vector<pair<string, int>> validFoods;

    for (const auto& [foodName, count] : orderedFoods) {
        const auto& foods = restaurant->getFoods();
        auto it = foods.find(foodName);
        if (it == foods.end()) {
            throw runtime_error("Not Found: Food '" + foodName + "' not available in the restaurant");
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
            const auto& foods = get<3>(details); // غذاها و تعدادشان

            reservations.emplace_back(
                id,                         // int: شناسه رزرو
                restaurant->getName(),      // string: نام رستوران
                tableId,                    // int: شماره میز
                startTime,                  // int: زمان شروع
                endTime,                    // int: زمان پایان
                foods                       // vector<pair<string, int>>: غذاها و تعداد
            );
        }
    }

    if (reservations.empty()) {
        throw runtime_error("Not Found: No reservations found for this user");
    }

    // مرتب‌سازی رزروها بر اساس زمان شروع
    sort(reservations.begin(), reservations.end(), [](const auto& a, const auto& b) {
        return get<3>(a) < get<3>(b); // مقایسه زمان شروع
    });

    // نمایش رزروها
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
        throw runtime_error("Not Found: Restaurant does not exist");
    }

    // دریافت رزروهای کاربر برای این رستوران
    const auto& userReservations = restaurant->getUserReservations(username);
    if (userReservations.empty()) {
        throw runtime_error("Not Found: No reservations found for this user in the restaurant");
    }

    for (const auto& [id, details] : userReservations) {
        int tableId = get<0>(details);
        int startTime = get<1>(details);
        int endTime = get<2>(details);
        const auto& foods = get<3>(details);

        // چاپ اطلاعات رزرو
        cout << id << ": " << restaurantName << " " << tableId << " " << startTime << "-" << endTime;

        // چاپ غذاهای سفارش داده‌شده (در صورت وجود)
        if (!foods.empty()) {
            for (const auto& [foodName, count] : foods) {
                cout << " " << foodName << "(" << count << ")";
            }
        }

        cout << endl;
    }
}
void RestaurantManager::showUserReservationById(const string& username, const string& restaurantName, const string& reserveId) {
    // یافتن رستوران
    Restaurant* restaurant = findRestaurantByName(restaurantName);
    if (!restaurant) {
        throw runtime_error("Not Found: Restaurant does not exist");
    }

    // تبدیل شناسه رزرو از string به int
    int reservationId = stoi(reserveId);

    // دریافت رزرو کاربر
    const auto& userReservations = restaurant->getUserReservations(username);
    auto it = userReservations.find(reservationId);
    if (it == userReservations.end()) {
        throw runtime_error("Not Found: Reservation ID not found for this user in the specified restaurant");
    }

    // استخراج اطلاعات رزرو
    const auto& details = it->second;
    int tableId = get<0>(details);
    int startTime = get<1>(details);
    int endTime = get<2>(details);
    const auto& foods = get<3>(details);

    // نمایش اطلاعات رزرو
    cout << reservationId << ": " << restaurantName << " " << tableId << " " << startTime << "-" << endTime;

    // نمایش غذاهای سفارش داده‌شده
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
        throw runtime_error("Not Found: Restaurant does not exist");
    }

    if (!restaurant->isReservationExists(reserveId)) {
        throw runtime_error("Not Found: Reservation does not exist");
    }

    if (!restaurant->isReservationOwnedByUser(reserveId, username)) {
        throw runtime_error("Permission Denied: Reservation does not belong to the user");
    }

    restaurant->removeReservation(reserveId);

    cout << "Reservation " << reserveId << " in " << restaurantName << " deleted successfully." << endl;
}
















