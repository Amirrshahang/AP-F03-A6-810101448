#pragma once

#include <unordered_map>
#include <string>
#include <optional>
#include "User.hpp"
#include "DistrictManager.hpp"

class UserManager {
private:
    unordered_map<string, User> users; 
    optional<string> loggedInUser;    

public:
    string login(const string& username, const string& password);
    string logout();
    bool isLoggedIn() const;
    void printAllUsers() const;
    string getLoggedInUsername() const;
    bool userExists(const string& username) const;
    double getUserWallet(const string& username) const;
    string getUserDistrict(const string& username) const;
    void increaseWallet(const string& username, int amount);
    bool decreaseWallet(const string& username, int amount);
    void assignDistrictToUser(const string& username, const string& district);
    string signup(const string& username, const string& password, DistrictManager& districtManager);

};
