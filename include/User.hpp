#pragma once

#include "global.hpp"
#include "DistrictManager.hpp"

class User {
private:
    string username;
    string password;
    bool is_logged_in;
    string current_district;
    vector<string> reservations;
    DistrictManager* districtManager;

public:
    User();
    User(string uname, string pwd, DistrictManager* dm); 
    bool login(string uname, string pwd);
    void logout();
    void updateDistrict(const string& district);
    void addReservation(string reservation);
    void showReservations();
    string getUsername() const;
    bool isLoggedIn() const;
    string getDistrict() const;
};
