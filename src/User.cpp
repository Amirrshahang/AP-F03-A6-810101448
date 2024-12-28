#include "User.hpp"


User::User() 
    : username(""), password(""), is_logged_in(false), current_district(""), districtManager(nullptr) {}

User::User(string uname, string pwd, DistrictManager* dm) 
    : username(uname), password(pwd), is_logged_in(false), districtManager(dm) {}

bool User::login(string uname, string pwd) {
    if (uname == username && pwd == password) {
        is_logged_in = true;
        return true;
    }
    return false;
}

void User::logout() {
    is_logged_in = false;
}

void User::addReservation(string reservation) {
    reservations.push_back(reservation);
}

void User::showReservations() {
    if (reservations.empty()) {
        cout << "Not Found" << endl;
    } else {
        for (const auto& res : reservations) {
            cout << res << endl;
        }
    }
}

void User::updateDistrict(const string& district) {
    if (districtManager->isDistrictExists(district)) {
        current_district = district;
    } else {
        throw runtime_error("Not Found");
    }
}

string User::getUsername() const { return username; }

bool User::isLoggedIn() const { return is_logged_in; }

string User::getDistrict() const { return current_district; }
