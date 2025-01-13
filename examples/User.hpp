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
    double wallet;


public:
    User();
    User(string uname, string pwd, DistrictManager* dm); 

    
    void logout();
    void showReservations();
    bool isLoggedIn() const;
    double getWallet() const;
    string getDistrict() const;
    string getUsername() const;
    void addToWallet(int amount);
    void backToWallet(int amount);
    bool decreaseWallet(int amount);
    bool login(string uname, string pwd);
    void deductFromWallet(double amount);
    void addReservation(string reservation);
    void updateDistrict(const string& district);
};
