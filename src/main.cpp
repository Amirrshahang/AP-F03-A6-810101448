#include "global.hpp"
#include "UserManager.hpp"
#include "DistrictManager.hpp"
#include "RestaurantManager.hpp"

void processCommand(const string& command, UserManager& userManager, DistrictManager& districtManager, RestaurantManager& restaurantManager) {

    const string validMethods[] = {"GET", "POST", "PUT", "DELETE"};
    bool isValid = false;

    for (const auto& method : validMethods) {
        if (command.find(method) == 0) {
            isValid = true;
            break;
        }
    }
    if (!isValid) {
        throw runtime_error("Bad Request");
    }

    regex signupPattern(R"(^POST\s+signup\s+\?\s+username\s+\"([a-zA-Z0-9_]+)\"\s+password\s+\"([a-zA-Z0-9_]+)\"$)");
    regex loginPattern(R"(^POST\s+login\s+\?\s+username\s+\"([a-zA-Z0-9_]+)\"\s+password\s+\"([a-zA-Z0-9_]+)\"$)");
    regex logoutPattern(R"(^POST\s+logout\s+\?$)");
    regex districtsPattern(R"(^GET\s+districts\s+\?$)");
    regex districtDetailPattern(R"(^GET\s+districts\s+\?\s+district\s+\"([a-zA-Z0-9_ ]+)\"$)");
    regex putDistrictPattern(R"(^PUT\s+my_district\s+\?\s+district\s+\"([a-zA-Z0-9_ ]+)\"$)");
    regex getRestaurantsPattern(R"(^GET\s+restaurants\s+\?$)");
    regex getRestaurantsByFoodPattern(R"(^GET\s+restaurants\s+\?\s+food_name\s+\"([a-zA-Z0-9_ ]+)\"$)");
    regex restaurantDetailPattern(R"(^GET\s+restaurant_detail\s+\?\s+restaurant_name\s+\"([a-zA-Z0-9_ ]+)\"$)");
    regex reservePatternWithFoods(R"(^POST\s+reserve\s+\?\s+restaurant_name\s+\"([a-zA-Z0-9_ ]+)\"\s+table_id\s+\"([0-9]+)\"\s+start_time\s+\"([0-9]+)\"\s+end_time\s+\"([0-9]+)\"\s*(?:foods\s+\"([a-zA-Z0-9_, ]*)\")?$)");
    regex reservesPattern(R"(^GET\s+reserves\s*(?:\?\s+restaurant_name\s+\"([^\"]+)\"\s*(?:reserve_id\s+\"([^\"]+)\")?)?$)");




    smatch match;

    if (regex_match(command, match, signupPattern)) {
        string username = match[1];
        string password = match[2];

        try {
            string result = userManager.signup(username, password, districtManager);
            cout << result << endl;
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
        }
    } else if (regex_match(command, match, loginPattern)) {
        string username = match[1];
        string password = match[2];

        try {
            string result = userManager.login(username, password);
            cout << result << endl;
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
        }
    } else if (regex_match(command, match, logoutPattern)) {
        try {
            string result = userManager.logout();
            cout << result << endl;
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
        }
    } else if (regex_match(command, match, districtsPattern)) {
        try {
            string username = userManager.getLoggedInUsername();
            if (!username.empty()) {
                districtManager.printAllDistricts();
            }
            else {
                throw runtime_error("Permission Denied");
            }
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
        }
    } else if (regex_match(command, match, districtDetailPattern)) {
        string districtName = match[1];

        try {
            string username = userManager.getLoggedInUsername();
            if (!username.empty()) {
                districtManager.printNeighbors(districtName);
            }
            else {
                throw runtime_error("Permission Denied");
            }
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
        }
    } else if (regex_match(command, match, putDistrictPattern)) {
        string districtName = match[1];
        try {
            string username = userManager.getLoggedInUsername();
            if (!username.empty()) {
                userManager.assignDistrictToUser(username, districtName);  
                cout << "OK" << endl;
            } else {
                throw runtime_error("Permission Denied");
            }
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
        }
    } else if (regex_match(command, match, getRestaurantsPattern)) {
        try {
            string username = userManager.getLoggedInUsername();
            if (!username.empty()) {
                string userDistrict = userManager.getUserDistrict(username);
                if (userDistrict.empty()) { 
                    throw runtime_error("Not Found");
                }
                restaurantManager.getRestaurantsByProximity(userDistrict);
            } else {
               throw runtime_error("Permission Denied");
            }
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
        }
    } else if (regex_match(command, match, getRestaurantsByFoodPattern)) {
        string foodName = match[1];
        try {
            string username = userManager.getLoggedInUsername();
            if (!username.empty()) {
                string userDistrict = userManager.getUserDistrict(username);
                if (userDistrict.empty()) { 
                    throw runtime_error("Not Found");
                }
                restaurantManager.getRestaurantsByFood(foodName);
            } else {
                throw runtime_error("Permission Denied");
            }
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
        }
    } else if (regex_match(command, match, restaurantDetailPattern)) {
        string restaurantName = match[1];
        try {
            string username = userManager.getLoggedInUsername();
            if (!username.empty()) {
                try {
                    Restaurant* restaurant = restaurantManager.findRestaurantByName(restaurantName);
                    restaurant->printRestaurantDetails();
                } catch (const runtime_error& e) {
                    cerr << e.what() << endl;
                }       
            } else {
                throw runtime_error("Permission Denied");
            }
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
            }
    } else if (regex_match(command, match, reservePatternWithFoods)) {
        try {
            string username = userManager.getLoggedInUsername();
            if (!username.empty()) {
                    string restaurantName = match[1];
                    int tableId = stoi(match[2]);
                    int startTime = stoi(match[3]);
                    int endTime = stoi(match[4]);
                    vector<string> orderedFoods;
                    if (match[5].matched) {
                        string foodsStr = match[5];
                        istringstream ss(foodsStr);
                        string food;
                        while (getline(ss, food, ',')) {
                            orderedFoods.push_back(food);
                            }
                    }
                try {
                    restaurantManager.reserveTable(restaurantName, tableId, startTime, endTime, username, orderedFoods);
                } catch (const runtime_error& e) {
                    cerr << e.what() <<endl;
                }
            }else {
                throw runtime_error("Permission Denied");
            }  
        }catch (const runtime_error& e) {
        cerr << e.what() << endl;
        }
    } else if (regex_match(command, match, reservesPattern)) {
        try {
            string username = userManager.getLoggedInUsername();
            if (!username.empty()) {
                    string restaurantName = match[1];
                    string reserveId = match[2].matched ? match[2] : "";
                try {
                    restaurantManager.showUserReservations(username, restaurantName, reserveId);
                } catch (const runtime_error& e) {
                    cerr << e.what() <<endl;
                    }
            }else {
                throw runtime_error("Permission Denied");
            }  
        }catch (const runtime_error& e) {
        cerr << e.what() << endl;
        }
    }else{
        cout << "Invalid command format" << endl;
        }
}

int main(int argc, char* argv[]) {
    
    string restaurantsFile = CSV_PATH + argv[1]; 
    string districtsFile = CSV_PATH + argv[2];  

    string command;
    UserManager userManager;
    DistrictManager districtManager;
    RestaurantManager restaurantManager(districtManager);

    try {
        restaurantManager.loadRestaurantsFromCSV(restaurantsFile);
        districtManager.loadFromCSV(districtsFile);
    } catch (const exception& e) {
        cerr << "Error loading CSV files: " << e.what() << endl;
        return 1;
    }

    while (getline(cin, command)) {
        try {
            processCommand(command, userManager, districtManager, restaurantManager);
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
        }
    }

    return 0;
}

/*

POST signup ? username "low_mist" password "meoow"
POST login ? username "low_mist" password "meoow"

POST signup ? username "amir" password "amiramir"
POST login ? username "amir" password "amiramir"

POST signup ? username "mmd" password "mmmli"
POST login ? username "mmd" password "mmmli"

POST logout ?


GET districts ? district "Saadat Abad"
GET districts ? district "Omid Town"

GET districts ?
PUT my_district ? district "Tajrish"
PUT my_district ? district "Omid Town"

GET restaurants ?
GET restaurants ? food_name "Kir"

GET restaurant_detail ? restaurant_name "Nofel Loshato"
GET restaurant_detail ? restaurant_name "Na Koja Abad"
GET restaurant_detail ? restaurant_name "Apadana"

GET restaurants ? food_name "Sushi"


POST reserve ? restaurant_name "Chicken Family" table_id "1"
start_time "15" end_time "17" foods "Sib Zamini,Sokhari"

POST reserve ? restaurant_name "Nofel Loshato" table_id "2" start_time "14" end_time "15"
POST reserve ? restaurant_name "Nofel Loshato" table_id "1" start_time "18" end_time "19" foods "brger,szar"


POST reserve ? restaurant_name "Apadana" table_id "1" start_time "10" end_time "11" foods "Noodles,Pizza"


*/
