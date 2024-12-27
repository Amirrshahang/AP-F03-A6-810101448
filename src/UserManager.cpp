#include "UserManager.hpp"

string UserManager::signup(const string& username, const string& password, DistrictManager& districtManager) {
    if (loggedInUser.has_value()) {
        throw runtime_error("Permission Denied");
    }

    if (userExists(username)) {
        throw runtime_error("Bad Request");
    }

users.emplace(username, User(username, password, &districtManager));
    loggedInUser = username;
    return "OK";
}

string UserManager::login(const string& username, const string& password) {
    if (!userExists(username)) {
        throw runtime_error("Not Found");
    }

    if (loggedInUser.has_value()) {
        throw runtime_error("Permission Denied");
    }

    if (users[username].login(username, password)) {
        loggedInUser = username;
        return "OK";
    }

    throw runtime_error("Permission Denied");
}

string UserManager::logout() {
    if (!loggedInUser.has_value()) {
        throw runtime_error("Permission Denied");
    }

    users[loggedInUser.value()].logout();
    loggedInUser.reset();
    return "OK";
}

bool UserManager::isLoggedIn() const {
    return loggedInUser.has_value();
}

bool UserManager::userExists(const string& username) const {
    return users.find(username) != users.end();
}

void UserManager::assignDistrictToUser(const string& username, const string& district) {
    if (userExists(username)) {
        users[username].updateDistrict(district);
    } else {
        throw runtime_error("Not Found");
    }
}

string UserManager::getUserDistrict(const string& username) const {
    if (userExists(username)) {
        return users.at(username).getDistrict();
    }
    throw runtime_error("Not Found");
}

string UserManager::getLoggedInUsername() const {
    if (loggedInUser.has_value()) {
        return loggedInUser.value();
    }
    return "";
}

void UserManager::printAllUsers() const {
    cout << "Registered Users:" << endl;
    for (const auto& pair : users) {
        cout << " - " << pair.first << endl;
    }
}
