#include "../include/Utils/DataManager.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

DataManager &DataManager::getInstance() {
    static DataManager instance;
    return instance;
}

bool DataManager::saveToFile(const std::string &filename, const json &data) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << "\n";
        return false;
    }
    try {
        file << data.dump(4);
    } catch (const json::exception &e) {
        std::cerr << "JSON serialization error during saveToFile (" << filename << "): " << e.what() << std::endl;
        file.close();
        return false;
    }
    file.close();
    return !file.fail();
}

json DataManager::loadFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for reading: " << filename << "\n";
        return json::object();
    }
    json data;
    try {
        file >> data;
    } catch (const json::parse_error &e) {
        std::cerr << "JSON parse error in file " << filename << ": " << e.what() << std::endl;
        file.close();
        return json::object();
    }
    file.close();
    return data;
}

bool DataManager::saveData(const std::string &filePath, const json &data) {
    return saveToFile(filePath, data);
}

json DataManager::loadData(const std::string &filePath) {
    return loadFromFile(filePath);
}
