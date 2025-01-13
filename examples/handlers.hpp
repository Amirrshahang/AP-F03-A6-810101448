#ifndef HANDLERS_HPP_INCLUDE
#define HANDLERS_HPP_INCLUDE
#include "UserManager.hpp" 
#include "DistrictManager.hpp" 
#include "RestaurantManager.hpp"



#include <map>
#include <string>

#include "../server/server.hpp"
struct Token{
    string username;
    string password;
    bool valid;
};

class SignupHandler : public RequestHandler {
    Token* token_;
    UserManager* userManager;
    DistrictManager* districtManager;


public:
    SignupHandler(UserManager* um, DistrictManager* dm, Token* token)
        : token_(token), userManager(um), districtManager(dm) {}

    Response* callback(Request*) override;
};

class LoginHandler : public RequestHandler {
private:
    //Token* token_;
    UserManager* userManager;
public:
    LoginHandler(UserManager* um) : userManager(um) {}

    Response* callback(Request*) override;
};

class LogoutHandler : public RequestHandler {
    UserManager* userManager;

public:
    LogoutHandler(UserManager* um) : userManager(um) {}
    Response* callback(Request*) override;
};

class PrintAllRestaurant : public TemplateHandler {
public:
    PrintAllRestaurant(const std::string& filePath,RestaurantManager* re, UserManager* um);
    std::map<std::string, std::string> handle(Request* req) override;

private:
    RestaurantManager* restaurantManager;
    UserManager* userManager;    
};

class PrintRestaurantDetails : public TemplateHandler {
public:
    PrintRestaurantDetails(const std::string& filePath, UserManager* um, RestaurantManager* re);
    std::map<std::string, std::string> handle(Request* req) override;

private:
    UserManager* userManager;
    RestaurantManager* restaurantManager;
};

class ReserveTable : public TemplateHandler {
public:
    ReserveTable(const std::string& filePath, UserManager* um, RestaurantManager* re);
    std::map<std::string, std::string> handle(Request* req) override;

private:
    UserManager* userManager;
    RestaurantManager* restaurantManager;
};

class IncreaseWalletHandler : public RequestHandler {
private:
    UserManager* userManager;
public:
    IncreaseWalletHandler(UserManager* um) : userManager(um) {}
    Response* callback(Request*) override;
};

class AssignDistrictToUser : public RequestHandler {
private:
    UserManager* userManager;
public:
    AssignDistrictToUser(UserManager* um) : userManager(um) {}
    Response* callback(Request*) override;
};

class HomePageHandler : public TemplateHandler {
    Token* token_;
public:
    HomePageHandler (const std::string& filePath,Token* token = nullptr);
    std::map<std::string, std::string> handle(Request* req) override;
};

class showAllReservations : public TemplateHandler {
public:
    showAllReservations(const std::string& filePath, UserManager* um, RestaurantManager* re);
    std::map<std::string, std::string> handle(Request* req) override;

private:
    UserManager* userManager;
    RestaurantManager* restaurantManager;
};

#endif


