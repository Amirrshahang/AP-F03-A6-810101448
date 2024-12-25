#include "global.hpp"
#include "UserManager.hpp"
#include "DistrictManager.hpp"
#include "SignupException.hpp"

class BadRequestException : public exception {
public:
    const char* what() const noexcept override {
        return "Bad Request";
    }
};

void processCommand(const string& command) {
    const string validMethods[] = {"GET", "POST", "PUT", "DELETE"};
    bool isValid = false;

    for (const auto& method : validMethods) {
        if (command.find(method) == 0) {
            isValid = true;
            break;
        }
    }

    if (!isValid) {
        throw BadRequestException();
    }

}

int main() {
    string command;
    UserManager userManager;
    DistrictManager districtManager;

    districtManager.loadFromCSV(CSV_PATH + "districts.csv");

    //districtManager.printAllDistricts();
    while (true) {
        getline(cin, command);

        try {
            processCommand(command);
               
        } catch (const BadRequestException& e) {
            cerr << e.what() << endl;
        }
    }

    return 0;
}
