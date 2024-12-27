#pragma once

#include "global.hpp"
#include <map>

class DistrictManager {
private:
    map<string, vector<string>> districts;

public:
    void loadFromCSV(const string& filePath);
    vector<string> getNeighbors(const string& districtName) const;
    void printAllDistricts() const;
    void printNeighbors(const string& districtName) const ;
    bool isDistrictExists(const string& districtName) const;

};
