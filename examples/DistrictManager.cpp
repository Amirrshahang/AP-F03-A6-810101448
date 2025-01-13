#include "DistrictManager.hpp"

using namespace std;

void DistrictManager::loadFromCSV(const string& filePath) {
    ifstream file(filePath);
   
    string line;
    getline(file, line);
    while (getline(file, line)) {
        istringstream lineStream(line);
        string district;
        getline(lineStream, district, ',');

        vector<string> neighbors;
        string neighbor;
        while (getline(lineStream, neighbor, ';')) {
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

string DistrictManager::printAllDistricts() const {
    if (districts.empty()) {
        throw runtime_error("Empty");
    }

    string result;

    for (auto it = districts.begin(); it != districts.end(); ++it) {
        result += it->first + ": ";
        for (auto neighborIt = it->second.begin(); neighborIt != it->second.end(); ++neighborIt) {
            result += *neighborIt;
            if (neighborIt + 1 != it->second.end()) {
                result += ", ";
            }
        }
        result += "<br>";
    }

    return result;
}

void DistrictManager::printNeighbors(const string& districtName) const {
    auto it = districts.find(districtName);
    if (it != districts.end()) {
        cout << districtName << ": ";
        for (const auto& neighbor : it->second) {
            cout << neighbor << " ";
        }
        cout << endl;
    } else {
        throw runtime_error("Not Found");
    }
}

bool DistrictManager::isDistrictExists(const string& districtName) const {
    return districts.find(districtName) != districts.end();
}