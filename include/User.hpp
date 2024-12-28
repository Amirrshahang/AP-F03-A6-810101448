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
    void logout();
    void showReservations();
    bool isLoggedIn() const;
    string getDistrict() const;
    string getUsername() const;
    bool login(string uname, string pwd);
    void addReservation(string reservation);
    void updateDistrict(const string& district);
};
