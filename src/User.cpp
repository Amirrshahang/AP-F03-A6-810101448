#include "User.hpp"

User::User()
    : username(""), password(""), is_logged_in(false), current_district("") {}

User::User(string uname, string pwd)
    : username(uname), password(pwd), is_logged_in(false), current_district("") {}

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

void User::setDistrict(string district) {
    current_district = district;
}

void User::addReservation(string reservation) {
    reservations.push_back(reservation);
}

void User::showReservations() {
    if (reservations.empty()) {
        cout << "No reservations found." << endl;
    } else {
        for (const auto& res : reservations) {
            cout << res << endl;
        }
    }
}

string User::getUsername() const { return username; }

bool User::isLoggedIn() const { return is_logged_in; }

string User::getDistrict() const { return current_district; }
