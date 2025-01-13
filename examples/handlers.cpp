#include "handlers.hpp"
#include "global.hpp"
#include <cstdlib>
#include <iostream>
#include "UserManager.hpp"
#include "RunHandler.hpp"

Response* SignupHandler::callback(Request* req) {
    string username = req->getBodyParam("username");
    string password = req->getBodyParam("password");
    Response* res = new Response();
    try {
            string result = userManager->signup(username, password, *districtManager);
            if (result == "OK") {
                token_->username = username;
                token_->password = password;
                token_->valid = true;
                Response* res = Response::redirect("/Homepage");
                return res;
            }
    } catch (const runtime_error& e) { 
        cerr << "Signup error: " << e.what() << endl;

        res->setHeader("Content-Type", "text/html");
        string body = "<html>"
                      "<head>"
                      "<script>"
                      "alert('Error: " + string(e.what()) + "');"
                      "window.location.href = '/Homepage';"
                      "</script>"
                      "</head>"
                      "<body></body>"
                      "</html>";
        res->setBody(body);
        return res;
    }

}

Response* LoginHandler::callback(Request* req) {
    string username = req->getBodyParam("username");
    string password = req->getBodyParam("password");
    
    Response* res = new Response();
    try {
        string result = userManager->login(username, password);
        if (result == "OK") {
            // token_->username = username;
            // token_->password = password;
            // token_->valid = true;
            Response* res = Response::redirect("/Homepage");
            return res;
        }
        
    } catch (const runtime_error& e) {
        cerr << "login error: " << e.what() << endl;

        res->setHeader("Content-Type", "text/html");
        string body = "<html>"
                      "<head>"
                      "<script>"
                      "alert('Error: " + string(e.what()) + "');"
                      "window.location.href = '/';"
                      "</script>"
                      "</head>"
                      "<body></body>"
                      "</html>";
        res->setBody(body);
        return res;
    }
}

Response* LogoutHandler::callback(Request* req) {
    Response* res = new Response();

    try {
        string result = userManager->logout();
        if (result == "OK") {
            Response* res = Response::redirect("/");
            return res;
        }
    } catch (const runtime_error& e) {
        cerr << "logout error: " << e.what() << endl;

        res->setHeader("Content-Type", "text/html");
        string body = "<html>"
                      "<head>"
                      "<script>"
                      "alert('Error: " + string(e.what()) + "');"
                      "window.location.href = '/Homepage';"
                      "</script>"
                      "</head>"
                      "<body></body>"
                      "</html>";
        res->setBody(body);
        return res;
    }
}

Response* IncreaseWalletHandler::callback(Request* req)  {
    string username = userManager->getLoggedInUsername();
    int amount = stoi(req->getBodyParam("amount"));  
    userManager->increaseWallet(username, amount);

    Response* res = Response::redirect("/Homepage");  
    return res;
}

Response* AssignDistrictToUser::callback(Request* req) {
    Response* res = new Response();
    try {
        string username = userManager->getLoggedInUsername();
        if (!username.empty()) {
            string districtName = req->getBodyParam("district"); 
            string result = userManager->assignDistrictToUser(username, districtName);
            if (result == "OK") {
                return Response::redirect("/Homepage");
            }
        } else {
            throw runtime_error("Permission Denied");
        }
    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
        string body = R"(
            <html>
            <head>
            <script>
            alert('Error: )" + string(e.what()) + R"(');
            window.location.href = '/Homepage';
            </script>
            </head>
            <body></body>
            </html>
        )";

        res->setHeader("Content-Type", "text/html");
        res->setBody(body);
        return res;
    }
}


HomePageHandler::HomePageHandler(const string& filePath,Token* token)
    : TemplateHandler(filePath),
      token_(token) {}

map<string, string> HomePageHandler::handle(Request* req) {
    string name = token_->username;
    map<string, string> context;
    context["user"] = name  ;
    if(token_->valid) context["valid"] = "true";
    else context["valid"] = "false";
    return context;
}


PrintAllRestaurant::PrintAllRestaurant(const string& filePath, RestaurantManager* re, UserManager* um)
    : TemplateHandler(filePath), restaurantManager(re), userManager(um) {}

map<string, string> PrintAllRestaurant::handle(Request* req) {
    map<string, string> context;
    try {
        string username = userManager->getLoggedInUsername();
        if (!username.empty()) {
            string userDistrict = userManager->getUserDistrict(username);

            if (userDistrict.empty()) { 
                    throw runtime_error("Not Found");
                }
            string result = restaurantManager->getRestaurantsByProximity(userDistrict);

            context["result"] = result;
        } else {
            throw runtime_error("Permission Denied");
        }
    } catch (const runtime_error& e) {
        context["result"] = "Error: " + string(e.what());
    }
    return context;
}


PrintRestaurantDetails::PrintRestaurantDetails(const string& filePath, UserManager* um, RestaurantManager* re)
    : TemplateHandler(filePath), userManager(um), restaurantManager(re) {}

map<string, string> PrintRestaurantDetails::handle(Request* req) {
    map<string, string> context;
    try {
        string username = userManager->getLoggedInUsername();
        if (!username.empty()) {
            string restaurantName = req->getQueryParam("restaurantName");
            Restaurant* restaurant = restaurantManager->findRestaurantByName(restaurantName);
            if(restaurant != nullptr){
                string result = restaurant->printRestaurantDetails();
                context["result"] = result;

            }else{
                throw runtime_error("Not Found");
            }
        } else {
            throw runtime_error("Permission Denied");
        }
    } catch (const runtime_error& e) {
        context["result"] = "Error: " + string(e.what());
    }
    return context;
}


ReserveTable::ReserveTable(const string& filePath, UserManager* um, RestaurantManager* re)
    : TemplateHandler(filePath), userManager(um), restaurantManager(re) {}

map<string, string> ReserveTable::handle(Request* req) {
    map<string, string> context;
    string restaurantName = req->getQueryParam("restaurantName");

    string tableIdStr = req->getQueryParam("table_id");
    string startTimeStr = req->getQueryParam("start_time");
    string endTimeStr = req->getQueryParam("end_time");
    string foodname;
    if(req->getQueryParam("foodname")!=""){
        foodname = req->getQueryParam("foodname");
    }

    if(foodname == "Empty") foodname = "";
    int tableId = stoi(tableIdStr);
    int startTime = stoi(startTimeStr);
    int  endTime = stoi(endTimeStr);

    vector<pair<string, int>> orderedFoods;
    if (!foodname.empty()) {

        map<string, int> foodCounts;
        istringstream ss(foodname);
        string food;
        while (getline(ss, food, ',')) {
            foodCounts[food]++;
        }
        
        for (const auto& [foodName, count] : foodCounts) {
            orderedFoods.emplace_back(foodName, count);
        }
    }

    try {
        string username = userManager->getLoggedInUsername();

        if (!username.empty()) {
            string result = restaurantManager->reserveTable(restaurantName, tableId, startTime, endTime, username, orderedFoods, *userManager);
            context["result"] = result;
        } else {
            throw runtime_error("Permission Denied");
        }
    } catch (const runtime_error& e) {
        context["result"] = "Error: " + string(e.what());
    }

    return context;
}


showAllReservations::showAllReservations(const string& filePath, UserManager* um, RestaurantManager* re)
    : TemplateHandler(filePath), userManager(um), restaurantManager(re) {}

map<string, string> showAllReservations::handle(Request* req) {
    map<string, string> context;
    try {
            
        string username = userManager->getLoggedInUsername();
        if (!username.empty()) {
            string result = restaurantManager->showAllUserReservations(username);
            context["result"] = result;
        }else{
            throw runtime_error("Permission Denied");
        }
    }catch (const runtime_error& e) {
        context["result"] = "Error: " + string(e.what());
    }  

    return context;
}
