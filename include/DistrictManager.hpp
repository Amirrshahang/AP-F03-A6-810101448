#pragma once

#include "global.hpp"
#include <unordered_map>

class DistrictManager {
private:
    unordered_map<string, vector<string>> districts;

public:
    void loadFromCSV(const string& filePath);
    vector<string> getNeighbors(const string& districtName) const;
    void printAllDistricts() const;
};
