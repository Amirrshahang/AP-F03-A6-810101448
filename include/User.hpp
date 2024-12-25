#pragma once

#include "global.hpp"

class User {
private:
    string username;
    string password;
    bool is_logged_in;
    string current_district;
    vector<string> reservations;

public:
    User();
    User(string uname, string pwd);
    bool login(string uname, string pwd);
    void logout();
    void setDistrict(string district);
    void addReservation(string reservation);
    void showReservations();
    string getUsername() const;
    bool isLoggedIn() const;
    string getDistrict() const;
};