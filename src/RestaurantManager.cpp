#include "RestaurantManager.hpp"
#include "DiscountPolicies.hpp"
#include "Restaurant.hpp"
#include "UserManager.hpp"


using namespace std;

void RestaurantManager::loadDiscountsFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Failed to open discount file");
    }

    string line;
    getline(file, line);
    while (getline(file, line)) {
        istringstream ss(line);
        string restaurantName, totalDiscount, firstOrderDiscount, foodDiscounts;

        getline(ss, restaurantName, ',');
        getline(ss, totalDiscount, ',');
        getline(ss, firstOrderDiscount, ',');
        getline(ss, foodDiscounts, ',');

        Restaurant* restaurant = findRestaurantByName(restaurantName);
        if (!restaurant) {
            continue;
        }

        Discount discount;

        if (totalDiscount != "none") {
            size_t firstDelimiter = totalDiscount.find(';');
            size_t secondDelimiter = totalDiscount.rfind(';');
            if (firstDelimiter != string::npos && secondDelimiter != string::npos && firstDelimiter != secondDelimiter) {
                string type = totalDiscount.substr(0, firstDelimiter);
                int minimum = stoi(totalDiscount.substr(firstDelimiter + 1, secondDelimiter - firstDelimiter - 1));
                int value = stoi(totalDiscount.substr(secondDelimiter + 1));
                discount.totalPriceDiscount = make_tuple(type, minimum, value);
            }
        }
        if (firstOrderDiscount != "none") {
            size_t delimiter = firstOrderDiscount.find(';');
            if (delimiter != string::npos) {
                string type = firstOrderDiscount.substr(0, delimiter);
                int value = stoi(firstOrderDiscount.substr(delimiter + 1));
                discount.firstOrderDiscount = make_pair(type, value);
            }
        }
        if (foodDiscounts != "none") {
            istringstream foodStream(foodDiscounts);
            string foodEntry;
            while (getline(foodStream, foodEntry, '|')) {
                size_t firstDelimiter = foodEntry.find(';');
                size_t secondDelimiter = foodEntry.find(':', firstDelimiter + 1);

                if (firstDelimiter != string::npos && secondDelimiter != string::npos) {
                    string type = foodEntry.substr(0, firstDelimiter);
                    string foodName = foodEntry.substr(firstDelimiter + 1, secondDelimiter - firstDelimiter - 1);
                    int value = stoi(foodEntry.substr(secondDelimiter + 1));
                    discount.foodDiscounts[foodName] = make_pair(type, value);
                }
            }
        }

        restaurant->setDiscount(discount);
    }

    file.close();
}

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
     return nullptr;
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

int RestaurantManager::reserveTable(const string& restaurantName, int tableId, int startTime, int endTime, const string& username, const vector<pair<string, int>>& orderedFoods, UserManager& userManager) {
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

    const Discount& discount = restaurant->getDiscount();
    int totalDiscount = 0;
    ItemSpecificDiscount itemDisc(discount.foodDiscounts, validFoods, restaurant->getFoods());
    int itemDiscount = itemDisc.calculateDiscount(totalPrice);
    if (itemDiscount > 0) {
        totalDiscount += itemDiscount;
    }
    int priceAfterItemDiscount = totalPrice - itemDiscount;
    bool isFirstReservation = restaurant->isFirstReservationForUser(username); 

    if (discount.firstOrderDiscount.has_value() && isFirstReservation) {
        const auto& [type, value] = discount.firstOrderDiscount.value();
        FirstOrderDiscount firstOrderDisc(type, value);
        int discountValue = firstOrderDisc.calculateDiscount(priceAfterItemDiscount);
        if (discountValue > 0) {
            totalDiscount += discountValue;
            priceAfterItemDiscount -= discountValue;
        }
    }

    if (discount.totalPriceDiscount.has_value()) {
        const auto& [type, minimum, value] = discount.totalPriceDiscount.value();
        TotalPriceDiscount totalPriceDisc(type, minimum, value);
        int discountValue = totalPriceDisc.calculateDiscount(priceAfterItemDiscount);
        if (priceAfterItemDiscount >= minimum && discountValue > 0) {
            totalDiscount += discountValue;
            priceAfterItemDiscount -= discountValue;
        }
    }

    int finalPrice = max(0, priceAfterItemDiscount);
    int reserveId;
    if (userManager.decreaseWallet(username, finalPrice)) {
        reserveId = restaurant->addReservation(tableId, startTime, endTime, username, validFoods,totalPrice,finalPrice);
        cout << "Reserve ID: " << reserveId << endl;
        cout << "Table " << tableId << " for " << startTime << " to " << endTime << " in " << restaurantName << endl;
        cout << "Original Price: " << totalPrice << endl;
        if(totalPrice != 0){
            if (itemDiscount > 0) {
                cout << "Total Item Specific Discount: " << itemDiscount << endl;
            }

            if (discount.totalPriceDiscount.has_value()) {
                const auto& [type, minimum, value] = discount.totalPriceDiscount.value();
                TotalPriceDiscount totalPriceDisc(type, minimum, value);
                int totalPriceDiscountValue = totalPriceDisc.calculateDiscount(priceAfterItemDiscount + totalDiscount);
                if (totalPriceDiscountValue > 0) {
                    cout << "Order Amount Discount: " << totalPriceDiscountValue << endl;
                }
            }

            if (discount.firstOrderDiscount.has_value() && totalDiscount > itemDiscount && isFirstReservation) {
                const auto& [type, value] = discount.firstOrderDiscount.value();
                FirstOrderDiscount firstOrderDisc(type, value);
                int firstOrderDiscountValue = firstOrderDisc.calculateDiscount(priceAfterItemDiscount + totalDiscount - itemDiscount);
                cout << "First Order Discount: " << firstOrderDiscountValue << endl;
            }

            cout<<"Total Discount: "<< totalPrice - finalPrice <<endl;
            cout << "Total Price: " << finalPrice << endl;
        }
    }else{
        throw runtime_error("Bad Request");
    }

    return reserveId;
}

void RestaurantManager::showAllUserReservations(const string& username) {
    vector<tuple<int, string, int, int, int, vector<pair<string, int>>, int, int>> reservations;

    for (const auto& restaurant : restaurants) {
        const auto& userReservations = restaurant->getUserReservations(username);

        for (const auto& [id, details] : userReservations) {

            int tableId = get<0>(details);
            int startTime = get<1>(details);
            int endTime = get<2>(details);
            const auto& foods = get<3>(details);
            int totalPrice = get<4>(details); 
            int finalPrice = get<5>(details);
            reservations.emplace_back(id, restaurant->getName(), tableId, startTime, endTime, foods, totalPrice, finalPrice);
        }
    }

    if (reservations.empty()) {
        throw runtime_error("Empty");
    }

    sort(reservations.begin(), reservations.end(), [](const auto& a, const auto& b) {
        return get<0>(a) < get<0>(b);
    });

    for (const auto& [id, restaurant, tableId, startTime, endTime, foods, totalPrice, finalPrice] : reservations) {
        cout << id << ": " << restaurant << " " << tableId << " " << startTime << "-" << endTime;

        if (!foods.empty()) {
            for (const auto& [foodName, count] : foods) {
                cout << " " << foodName << "(" << count << ")";
            }
        }

        cout << " " << totalPrice << " " << finalPrice << endl;
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
        int totalPrice = get<4>(details); 
        int finalPrice = get<5>(details);
       
        cout << id << ": " << restaurantName << " " << tableId << " " << startTime << "-" << endTime;

        if (!foods.empty()) {
            for (const auto& [foodName, count] : foods) {
                cout << " " << foodName << "(" << count << ")";
            }
        }
        cout << " " << totalPrice << " " << finalPrice << endl;
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
    int totalPrice = get<4>(details);
    int finalPrice = get<5>(details);

    cout << reservationId << ": " << restaurantName << " " << tableId << " " << startTime << "-" << endTime;

    if (!foods.empty()) {
        for (const auto& [foodName, count] : foods) {
            cout << " " << foodName << "(" << count << ")";
        }
    }

    cout << " " << totalPrice << " " << finalPrice << endl;
}

void RestaurantManager::deleteReservation(const string& username, const string& restaurantName, int reserveId, UserManager& userManager) {
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

    int finalPrice = 0;
    restaurant->removeReservation(reserveId, finalPrice);
    int refundAmount = static_cast<int>(finalPrice * 0.6);
    userManager.backToWallet(username, refundAmount);
    cout<<"OK"<<endl;
}