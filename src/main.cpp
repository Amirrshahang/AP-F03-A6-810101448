#include "RunHandler.hpp"

int main(int argc, char* argv[]) {
 
    run(argv[1], argv[2], argv[3]);
    return 0;
}



/*

POST signup ? username "low_mist" password "meow"
POST increase_budget ? amount 108600
POST increase_budget ? amount -15000

PUT my_district ? district "Tajrish"


POST increase_budget ? amount fj

GET show_budget ?

GET restaurant_detail ? restaurant_name "Nofel Loshato"



POST signup ? username "low_mist" password "meow"

GET restaurant_detail ? restaurant_name "Nofel Loshato"
POST signup ? username "low_mist" password "meow"
POST increase_budget ? amount 108600
POST reserve ? restaurant_name "Nofel Loshato" table_id "1" start_time "22" end_time "23" foods "Kabab Barg,Kabab Barg,sezar"

POST reserve ? restaurant_name "Nofel Loshato" table_id "2" start_time "15" end_time "17" 



*
POST signup ? username "low_mist" password "meoow"
POST login ? username "low_mist" password "meoow"
POST signup ? username "amir" password "amiramir"
POST login ? username "amir" password "amiramir"
POST signup ? username "mmd" password "mmmli"
POST login ? username "mmd" password "mmmli"
POST logout ?
GET districts ? district "Saadat Abad"
GET districts ? district "Omid Town"
GET districts ?
PUT my_district ? district "Tajrish"
PUT my_district ? district "Omid Town"
GET restaurants ?
GET restaurants ? food_name "Kir"
GET restaurant_detail ? restaurant_name "Nofel Loshato"
GET restaurant_detail ? restaurant_name "Na Koja Abad"
GET restaurant_detail ? restaurant_name "Apadana"
GET restaurants ? food_name "Sushi"
POST reserve ? restaurant_name "Chicken Family" table_id "1"
start_time "15" end_time "17" foods "Sib Zamini,Sokhari"
POST reserve ? restaurant_name "Nofel Loshato" table_id "1" start_time "22" end_time "23" 
foods "sezar"
POST reserve ? restaurant_name "Nofel Loshato" table_id "1" start_time "13" end_time "14" foods "sezar,Kabab Barg,Kabab Barg"
POST reserve ? restaurant_name "Nofel Loshato" table_id "3" start_time "20" end_time "22" foods "Kabab Barg,Kabab Barg"
POST reserve ? restaurant_name "Apadana" table_id "1" start_time "12" end_time "13" foods "Noodles,Pizza"
GET reserves ?
GET reserves ? restaurant_name "Ocean Wave"
GET reserves ? restaurant_name "Nofel Loshato" reserve_id "1"
DELETE reserve ? restaurant_name "Nofel Loshato" reserve_id "2" 

*/










