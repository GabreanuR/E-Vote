#include "../../include/Utils/DataManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

const std::string DataManager::data_directory = "data/";

DataManager &DataManager::getInstance() {
    static DataManager instance;
    return instance;
}

bool DataManager::saveData(const std::string &key, const json &data) {
    try {
        const std::string filePath = data_directory + key + ".json";
        return saveToFile(filePath, data);
    } catch (const std::exception &e) {
        std::cerr << "Error saving data: " << e.what() << std::endl;
        return false;
    }
}

json DataManager::loadData(const std::string &key) {
    try {
        const std::string filePath = data_directory + key + ".json";
        return loadFromFile(filePath);
    } catch (const std::exception &e) {
        std::cerr << "Error loading data: " << e.what() << std::endl;
        return {};
    }
}

DataManager::DataManager() = default;

DataManager::~DataManager() = default;

bool DataManager::saveToFile(const std::string &filename, const json &data) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file for writing: " + filename);
        }
        file << data.dump(4);
        return true;
    } catch (const std::exception &e) {
        std::cerr << "Error saving to file: " << e.what() << std::endl;
        return false;
    }
}

json DataManager::loadFromFile(const std::string &filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file for reading: " + filename);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return json::parse(buffer.str());
    } catch (const std::exception &e) {
        std::cerr << "Error loading from file: " << e.what() << std::endl;
        return {};
    }
}
