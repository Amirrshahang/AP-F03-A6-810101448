#include "global.hpp"
#include "global.hpp"
#include "RunHandler.hpp"
#include "UserManager.hpp"
#include "DistrictManager.hpp"
#include "RestaurantManager.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <thread>


#include "../server/server.hpp"
#include "handlers.hpp"


void mapServerPaths(Server& server, UserManager& userManager, DistrictManager& districtManager,RestaurantManager& restaurantManager, Token& current_token) {
    server.setNotFoundErrPage("static/404.html");
    server.get("/", new ShowPage("static/home.html"));
    server.get("/Homepage", new HomePageHandler("static/Homepage.html", &current_token));
    server.get("/home.png", new ShowImage("static/home.png"));
    server.get("/signup", new ShowPage("static/signupcss.html"));
    server.post("/signup", new SignupHandler(&userManager, &districtManager, &current_token));
    server.get("/login", new ShowPage("static/logincss.html"));
    server.post("/login", new LoginHandler(&userManager));  
    server.get("/logout", new ShowPage("static/home.html"));
    server.post("/logout", new LogoutHandler(&userManager));
    server.post("/Putmy_district", new AssignDistrictToUser(&userManager));
    server.post("/increase-wallet", new IncreaseWalletHandler(&userManager));
    server.get("/resturantPage", new PrintAllRestaurant("template/resturantPage.html", &restaurantManager, &userManager));
    server.get("/RestaurantDetails", new PrintRestaurantDetails("template/resturantPage.html", &userManager, &restaurantManager));
    server.get("/reserve", new ReserveTable("template/resturantPage.html", &userManager, &restaurantManager));
    server.get("/ShowReserve", new showAllReservations("template/reserve.html", &userManager, &restaurantManager));

}



int main(int argc, char** argv) {
    Token current_token;
    current_token.valid = false;
    UserManager userManager;
    DistrictManager districtManager;
    RestaurantManager restaurantManager(districtManager);


    string restaurantsFile = CSV_PATH + argv[2]; 
    string districtsFile = CSV_PATH + argv[3];  
    string discountsFile = CSV_PATH + argv[4]; 

    restaurantManager.loadRestaurantsFromCSV(restaurantsFile);
    districtManager.loadFromCSV(districtsFile);
    restaurantManager.loadDiscountsFromCSV(discountsFile); 
 
    int port = argc > 1 ? std::stoi(argv[1]) : 5000;
    Server server(port);
    mapServerPaths(server, userManager, districtManager,restaurantManager ,current_token);
    std::cout << "Server running on port: " << port << std::endl;
    server.run(); 


    return 0;
}
