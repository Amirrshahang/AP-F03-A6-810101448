#pragma once

#include <unordered_map>
#include <string>
#include <optional>
#include "User.hpp"

class UserManager {
private:
    unordered_map<string, User> users; 
    optional<string> loggedInUser;    

public:
    string signup(const string& username, const string& password);
    string login(const string& username, const string& password);
    void logout();
    bool isLoggedIn() const;
    bool userExists(const string& username) const;
    string getLoggedInUsername() const;
    void printAllUsers() const;
};
