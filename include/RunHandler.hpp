#ifndef RUN_HANDLER_HPP
#define RUN_HANDLER_HPP

#include "global.hpp"
#include "UserManager.hpp"
#include "DistrictManager.hpp"
#include "RestaurantManager.hpp"


void loadFils(const string& restaurantsFilePath, const string& districtsFilePath, const string& discountsFilePath, DistrictManager& districtManager, RestaurantManager& restaurantManager);
void commandHandler(const string& command,UserManager& userManager,DistrictManager& districtManager, RestaurantManager& restaurantManager);
void processCommand(const string& command, UserManager& userManager, DistrictManager& districtManager, RestaurantManager& restaurantManager);
void run(const string& restaurantsFilePath,const string& districtsFilePath,const string& discountsFilePath);


#endif
