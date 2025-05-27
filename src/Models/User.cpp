#include "../include/Models/User.h"
#include "../include/Utils/DataManager.h"
#include "../include/Utils/Types.h"
#include "../include/Models/Election.h"
#include <algorithm>

const std::string User::countersFile = "data/counters.json";

int User::generateId() {
    json data = DataManager::getInstance().loadData(countersFile);
    int counter = 1;
    
    if (!data.empty() && data.contains("userId")) {
        counter = data["userId"].get<int>();
        if (counter < 1) counter = 1;
    }

    data["userId"] = counter + 1;
    if (!DataManager::getInstance().saveData(countersFile, data)) {
        throw std::runtime_error("Failed to update user ID counter");
    }
    
    return counter;
}

User::User(const json& data) {
    id = data["id"].get<int>();
    username = data["username"].get<std::string>();
    password = data["password"].get<std::string>();
    type = data["type"].get<std::string>() == "admin" ? UserType::admin : UserType::voter;
    disabled = data["disabled"].get<bool>();

    restrictedAccess.clear();
    const auto& access = data["access"];

    if (access.contains("national") && access["national"].get<bool>()) {
        restrictedAccess[ElectionLevel::national] = {};
    }

    if (access.contains("regions")) {
        std::set<int> regions;
        for (const auto& region : access["regions"]) {
            regions.insert(region.get<int>());
        }
        restrictedAccess[ElectionLevel::regional] = regions;
    }

    if (access.contains("municipalities")) {
        std::set<int> municipalities;
        for (const auto& municipality : access["municipalities"]) {
            municipalities.insert(municipality.get<int>());
        }
        restrictedAccess[ElectionLevel::municipal] = municipalities;
    }

    if (access.contains("localities")) {
        std::set<int> localities;
        for (const auto& locality : access["localities"]) {
            localities.insert(locality.get<int>());
        }
        restrictedAccess[ElectionLevel::local] = localities;
    }

    if (access.contains("nonGovernment")) {
        std::set<int> nonGov;
        for (const auto& entity : access["nonGovernment"]) {
            nonGov.insert(entity.get<int>());
        }
        restrictedAccess[ElectionLevel::nonGovernment] = nonGov;
    }
}

bool User::canVoteIn(const Election&) const {
    if (disabled || type != UserType::voter) return false;

    return true;
}

bool User::grantAccess(const ElectionLevel level, const int entityId) {

    if (entityId == -1) {
        restrictedAccess[level].clear();
    }

    else if (entityId == 0) {
        restrictedAccess[level].clear();

        switch (level) {
            case ElectionLevel::national:
                restrictedAccess[ElectionLevel::regional].clear();
                restrictedAccess[ElectionLevel::municipal].clear();
                restrictedAccess[ElectionLevel::local].clear();
                break;
            case ElectionLevel::regional:
                restrictedAccess[ElectionLevel::municipal].clear();
                restrictedAccess[ElectionLevel::local].clear();
                break;
            case ElectionLevel::municipal:
                restrictedAccess[ElectionLevel::local].clear();
                break;
            default:
                break;
        }
    }

    else {
        if (type != UserType::admin &&
            (level == ElectionLevel::regional || 
             level == ElectionLevel::municipal || 
             level == ElectionLevel::local)) {
            restrictedAccess[level].clear();
        }
        restrictedAccess[level].insert(entityId);
    }
    return true;
}

bool User::revokeAccess(const ElectionLevel level, const int entityId) {
    if (type == UserType::admin) return false;
    restrictedAccess[level].erase(entityId);
    return true;
}

bool User::hasAccessTo(const ElectionLevel level, const int entityId) const {
    if (type == UserType::admin) return true;

    const auto it = restrictedAccess.find(level);
    if (it == restrictedAccess.end()) return false;

    if (it->second.empty()) return true;

    return it->second.contains(entityId);
}

bool User::updateElectionResults(const int electionId, const int candidateId, const bool isIncrement) {
    json elections = DataManager::getInstance().loadData("elections");
    for (auto& election : elections) {
        if (election["id"] == electionId) {
            for (auto& candidate : election["candidates"]) {
                if (candidate["id"] == candidateId) {
                    const int votes = candidate["votes"].get<int>();
                    candidate["votes"] = isIncrement ? votes + 1 : votes - 1;
                    return DataManager::getInstance().saveData("elections", elections);
                }
            }
        }
    }
    return false;
}

bool User::deleteUser(int userId) {
    json users = DataManager::getInstance().loadData("users");
    const auto it = std::ranges::find_if(users,
                                   [userId](const json& user) { return user["id"] == userId; });
    
    if (it == users.end()) return false;

    users.erase(it);

    json elections = DataManager::getInstance().loadData("elections");
    for (auto& election : elections) {
        for (auto& candidate : election["candidates"]) {
            const int votes = candidate["votes"].get<int>();
            candidate["votes"] = votes - 1;
        }
    }
    
    return DataManager::getInstance().saveData("users", users) &&
           DataManager::getInstance().saveData("elections", elections);
}

bool User::removeVotes() const {
    json elections = DataManager::getInstance().loadData("elections");
    for (auto& election : elections) {
        for (auto& candidate : election["candidates"]) {
            const int votes = candidate["votes"].get<int>();
            candidate["votes"] = votes - 1;  // Decrement vote count
        }
    }
    
    return DataManager::getInstance().saveData("elections", elections);
}

json User::toJson() const {
    json data = {
        {"id", id},
        {"username", username},
        {"password", password},
        {"type", type == UserType::admin ? "admin" : "voter"},
        {"disabled", disabled}
    };

    json access_json_output;


    auto it_national = restrictedAccess.find(ElectionLevel::national);
    if (it_national != restrictedAccess.end() && it_national->second.empty()) {
        access_json_output["national"] = true;
    } else {
        access_json_output["national"] = false;
    }

    json regions_array = json::array();
    auto it_regions = restrictedAccess.find(ElectionLevel::regional);
    if (it_regions != restrictedAccess.end()) {
        for (const int id : it_regions->second) {
            regions_array.push_back(id);
        }
    }
    access_json_output["regions"] = regions_array;

    json municipalities_array = json::array();
    auto it_municipalities = restrictedAccess.find(ElectionLevel::municipal);
    if (it_municipalities != restrictedAccess.end()) {
        for (const int id : it_municipalities->second) {
            municipalities_array.push_back(id);
        }
    }
    access_json_output["municipalities"] = municipalities_array;

    json localities_array = json::array();
    auto it_localities = restrictedAccess.find(ElectionLevel::local);
    if (it_localities != restrictedAccess.end()) {
        for (const int id : it_localities->second) {
            localities_array.push_back(id);
        }
    }
    access_json_output["localities"] = localities_array;

    json non_gov_array = json::array();
    auto it_non_gov = restrictedAccess.find(ElectionLevel::nonGovernment);
    if (it_non_gov != restrictedAccess.end()) {
        for (const int id : it_non_gov->second) {
            non_gov_array.push_back(id);
        }
    }
    access_json_output["nonGovernment"] = non_gov_array;
    
    data["access"] = access_json_output;
    
    return data;
}

User User::fromJson(const json& data) {
    return User(data);
}

void User::updateAdminAccess(const ElectionLevel level, int entityId) {
    json usersData = DataManager::getInstance().loadData("data/users.json");

    auto it = std::ranges::find_if(usersData, [](const json& user) { return user["id"] == 1; });
    
    if (it != usersData.end()) {
        User admin(*it);
        admin.grantAccess(level, entityId);
        (*it) = admin.toJson();
        DataManager::getInstance().saveData("data/users.json", usersData);
    }
}
