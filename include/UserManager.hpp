#pragma once

#include <unordered_map>
#include <string>
#include <optional>
#include "User.hpp"
#include "DistrictManager.hpp" // اضافه کردن مرجع به DistrictManager

class UserManager {
private:
    unordered_map<string, User> users; 
    optional<string> loggedInUser;    

public:
    string signup(const string& username, const string& password, DistrictManager& districtManager); // تغییر در ورودی
    string login(const string& username, const string& password);
    string logout();
    bool isLoggedIn() const;
    bool userExists(const string& username) const;
    string getUserDistrict(const string& username) const;
    void assignDistrictToUser(const string& username, const string& district); // تغییر در ورودی
    string getLoggedInUsername() const;
    void printAllUsers() const;
};
