#pragma once

#include "global.hpp"

class DistrictManager {
private:
    map<string, vector<string>> districts;

public:
    string printAllDistricts() const;
    void loadFromCSV(const string& filePath);
    void printNeighbors(const string& districtName) const ;
    bool isDistrictExists(const string& districtName) const;
    vector<string> getNeighbors(const string& districtName) const;
};
