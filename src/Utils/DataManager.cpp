#include "../../include/Utils/DataManager.h"
#include "../../include/Models/User.h"
#include "../../include/Models/Election.h"
#include "../../include/Models/Candidate.h"
#include "../../include/Models/Location.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

DataManager* DataManager::instance = nullptr;

DataManager& DataManager::getInstance() {
    if (instance == nullptr) {
        instance = new DataManager();
    }
    return *instance;
}

bool DataManager::saveToFile(const std::string& filename, const json& data) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << "\n";
        return false;
    }
    file << data.dump(4);
    file.close();
    return true;
}

json DataManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for reading: " << filename << "\n";
        return json::object();
    }
    json data;
    file >> data;
    file.close();
    return data;
}

bool DataManager::saveData(const std::string& key, const json& data) {
    return saveToFile(key, data);
}

json DataManager::loadData(const std::string& key) {
    return loadFromFile(key);
}

void DataManager::loadAllData() {
    loadUsers();
    loadElections();
    loadCandidates();
    loadLocations();
}

void DataManager::saveAllData() {
    saveUsers();
    saveElections();
    saveCandidates();
    saveLocations();
}

void DataManager::loadUsers() {
    json data = loadFromFile(users_file);
    users.clear();
    for (const auto& userData : data) {
        users.emplace_back(userData);
    }
}

void DataManager::loadElections() {
    json data = loadFromFile(elections_file);
    elections.clear();
    for (const auto& electionData : data) {
        elections.push_back(Election::fromJson(electionData));
    }
}

void DataManager::loadCandidates() {
    json data = loadFromFile(candidates_file);
    candidates.clear();
    for (const auto& candidateData : data) {
        candidates.emplace_back(candidateData);
    }
}

void DataManager::loadLocations() {
    json data = loadFromFile(locations_file);
    locations.clear();

    if (data.contains("regions")) {
        for (const auto& [id, regionData] : data["regions"].items()) {
            locations.emplace_back(std::make_shared<Region>(regionData));
        }
    }

    if (data.contains("municipalities")) {
        for (const auto& [id, municipalityData] : data["municipalities"].items()) {
            locations.emplace_back(std::make_shared<Municipality>(municipalityData));
        }
    }

    if (data.contains("localities")) {
        for (const auto& [id, localityData] : data["localities"].items()) {
            locations.emplace_back(std::make_shared<Locality>(localityData));
        }
    }

    if (data.contains("nonGovernment")) {
        for (const auto& [id, entityData] : data["nonGovernment"].items()) {
            locations.emplace_back(std::make_shared<NonGovernment>(entityData));
        }
    }
}


void DataManager::saveUsers() {
    json data = json::array();
    for (const auto& user : users) {
        data.push_back(user.toJson());
    }
    saveToFile(users_file, data);
}

void DataManager::saveElections() {
    json data = json::array();
    for (const auto& election : elections) {
        data.push_back(election.toJson());
    }
    saveToFile(elections_file, data);
}

void DataManager::saveCandidates() {
    json data = json::array();
    for (const auto& candidate : candidates) {
        data.push_back(candidate.toJson());
    }
    saveToFile(candidates_file, data);
}

void DataManager::saveLocations() {
    json data = json::array();
    for (const auto& location : locations) {
        data.push_back(location->toJson());
    }
    saveToFile(locations_file, data);
}



void DataManager::addUser(const User& user) {
    users.push_back(user);
}

void DataManager::updateUser(const User& user) {
    for (auto& u : users) {
        if (u.getId() == user.getId()) {
            u = user;
            break;
        }
    }
}

void DataManager::removeUser(int userId) {
    users.erase(
        std::remove_if(users.begin(), users.end(),
            [userId](const User& user) { return user.getId() == userId; }),
        users.end()
    );
}

void DataManager::addCandidate(const Candidate& candidate) {
    candidates.push_back(candidate);
}

void DataManager::addLocation(const std::shared_ptr<Location>& location) {
    locations.push_back(location);
}



User* DataManager::findUser(int userId) {
    for (auto& user : users) {
        if (user.getId() == userId) {
            return &user;
        }
    }
    return nullptr;
}

Election* DataManager::findElection(int electionId) {
    for (auto& election : elections) {
        if (election.getId() == electionId) {
            return &election;
        }
    }
    return nullptr;
}

Candidate* DataManager::findCandidate(int candidateId) {
    for (auto& candidate : candidates) {
        if (candidate.getId() == candidateId) {
            return &candidate;
        }
    }
    return nullptr;
}

std::shared_ptr<Location> DataManager::findLocation(int locationId) {
    for (const auto& location_ptr : locations) {
        if (location_ptr->getId() == locationId) {
            return location_ptr;
        }
    }
    return nullptr;
}