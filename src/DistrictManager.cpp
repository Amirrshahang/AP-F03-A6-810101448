#include "DistrictManager.hpp"
#include <sstream>
#include <fstream>

void DistrictManager::loadFromCSV(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        throw runtime_error("Could not open file");
    }

    string line;
    while (getline(file, line)) {
        istringstream lineStream(line);
        string district;
        getline(lineStream, district, ',');

        vector<string> neighbors;
        string neighbor;
        while (getline(lineStream, neighbor, ',')) {
            neighbors.push_back(neighbor);
        }

        districts[district] = neighbors;
    }

    file.close();
}

vector<string> DistrictManager::getNeighbors(const string& districtName) const {
    auto it = districts.find(districtName);
    if (it != districts.end()) {
        return it->second;
    }
    return {};
}

void DistrictManager::printAllDistricts() const {
    for (auto it = districts.begin(); it != districts.end(); ++it) {
        cout << it->first << ": ";
        for (auto neighborIt = it->second.begin(); neighborIt != it->second.end(); ++neighborIt) {
            cout << *neighborIt << " ";
        }
        cout << endl;
    }
}

