#include "UserManager.hpp"
#include <iostream>
#include <stdexcept>

std::string UserManager::signup(const std::string& username, const std::string& password) {
    if (loggedInUser.has_value()) {
        throw std::runtime_error("Permission Denied: User already logged in.");
    }

    if (userExists(username)) {
        throw std::runtime_error("Bad Request: Username already exists.");
    }

    users[username] = User(username, password);
    return "OK";
}

std::string UserManager::login(const std::string& username, const std::string& password) {
    if (!userExists(username)) {
        throw std::runtime_error("Bad Request: Username does not exist.");
    }

    if (loggedInUser.has_value()) {
        throw std::runtime_error("Permission Denied: Another user is already logged in.");
    }

    if (users[username].login(username, password)) {
        loggedInUser = username;
        return "OK: Login successful";
    }

    throw std::runtime_error("Bad Request: Invalid password.");
}

void UserManager::logout() {
    if (!loggedInUser.has_value()) {
        throw std::runtime_error("Bad Request: No user is currently logged in.");
    }

    users[loggedInUser.value()].logout();
    loggedInUser.reset();
    std::cout << "OK: Logout successful." << std::endl;
}

bool UserManager::isLoggedIn() const {
    return loggedInUser.has_value();
}

bool UserManager::userExists(const std::string& username) const {
    return users.find(username) != users.end();
}

std::string UserManager::getLoggedInUsername() const {
    if (loggedInUser.has_value()) {
        return loggedInUser.value();
    }
    return "";
}

void UserManager::printAllUsers() const {
    std::cout << "Registered Users:" << std::endl;
    for (const auto& pair : users) {
        std::cout << " - " << pair.first << std::endl;
    }
}
