#include "global.hpp"
#include "UserManager.hpp"
#include "DistrictManager.hpp"
#include "SignupException.hpp"
#include "RestaurantManager.hpp"

class BadRequestException : public runtime_error {
public:
    BadRequestException(const string& message) : runtime_error(message) {}
};
void processCommand(const string& command, UserManager& userManager, DistrictManager& districtManager) {

    const string validMethods[] = {"GET", "POST", "PUT", "DELETE"};
    bool isValid = false;

    for (const auto& method : validMethods) {
        if (command.find(method) == 0) {
            isValid = true;
            break;
        }
    }
    if (!isValid) {
        throw BadRequestException("Bad Request");
    }

    regex signupPattern(R"(^POST\s+signup\s+\?\s+username\s+\"([a-zA-Z0-9_]+)\"\s+password\s+\"([a-zA-Z0-9_]+)\"$)");
    regex loginPattern(R"(^POST\s+login\s+\?\s+username\s+\"([a-zA-Z0-9_]+)\"\s+password\s+\"([a-zA-Z0-9_]+)\"$)");
    regex logoutPattern(R"(^POST\s+logout\s+\?$)");
    regex districtsPattern(R"(^GET\s+districts\s+\?$)");
    regex districtDetailPattern(R"(^GET\s+districts\s+\?\s+district\s+\"([a-zA-Z0-9_ ]+)\"$)");
    regex putDistrictPattern(R"(^PUT\s+my_district\s+\?\s+district\s+\"([a-zA-Z0-9_ ]+)\"$)");
    smatch match;

    if (regex_match(command, match, signupPattern)) {
        string username = match[1];
        string password = match[2];

        try {
            string result = userManager.signup(username, password,districtManager);
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
            districtManager.printAllDistricts();
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
        }
    } else if (regex_match(command, match, districtDetailPattern)) {
        string districtName = match[1];

        try {
            districtManager.printNeighbors(districtName);
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
                cerr << "Permission Denied" << endl;
            }
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
        }
    } else {
        cout << "Invalid command format" << endl;
    }
}


int main() {
    string command;
    UserManager userManager;
    DistrictManager districtManager;
    RestaurantManager restaurantManager;

    districtManager.loadFromCSV(CSV_PATH + "districts.csv");
    restaurantManager.loadRestaurantsFromCSV(CSV_PATH + "restaurants.csv");

    //districtManager.printAllDistricts();

    while (getline(cin, command)) {
        try {
            processCommand(command, userManager, districtManager);

        } catch (const BadRequestException& e) {
            cerr << e.what() << endl;
        }
    }

    return 0;
}

// try {
//         districtManager.loadFromCSV(CSV_PATH +"districts.csv");
//         districtManager.printAllDistricts();
        
//         if (districtManager.isDistrictExists("Tajrish")) {
//             cout << "District 'Tajrish' exists." << endl;
//         } else {
//             cout << "District 'Tajrish' does not exist." << endl;
//         }
//     } catch (const exception& e) {
//         cerr << "Error: " << e.what() << endl;
//     }
/*
int main() {
    UserManager userManager;

    // افزودن کاربران به `userManager`
    User user1;
    userManager.addUser("john", user1);

    try {
        // تنظیم ناحیه برای کاربر
        userManager.setDistrictUser("john", "Tajrish");
        cout << "District set successfully." << endl;
    } catch (const exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}

POST signup ? username "low_mist" password "meoow"
POST login ? username "low_mist" password "meoow"

POST signup ? username "amir" password "amiramir"
POST login ? username "amir" password "amiramir"

POST signup ? username "mmd" password "mmmli"
POST login ? username "mmd" password "mmmli"

POST logout ?


GET districts ? district "Saadat Abad"
GET districts ?
PUT my_district ? district "Tajrish"
PUT my_district ? district "Merrikh"

*/
