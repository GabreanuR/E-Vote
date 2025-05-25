#include "../include/Models/User.h"
#include "../include/Utils/DataManager.h"
#include "../include/Utils/Types.h"
#include "../include/Models/Election.h"
#include <algorithm>

// Static members initialization
const std::string User::countersFile = "counters";

// Private static methods
int User::generateId() {
    // Load current counter from file
    json data = DataManager::getInstance().loadData(countersFile);
    int counter = 1;  // Default to 1 if file doesn't exist
    
    if (!data.empty() && data.contains("userId")) {
        counter = data["userId"].get<int>();
        if (counter < 1) counter = 1;  // Ensure counter is at least 1
    }
    
    // Increment counter and save it back
    data["userId"] = counter + 1;
    if (!DataManager::getInstance().saveData(countersFile, data)) {
        throw std::runtime_error("Failed to update user ID counter");
    }
    
    return counter;
}

// Constructors
User::User(const json& data) {
    id = data["id"].get<int>();
    username = data["username"].get<std::string>();
    password = data["password"].get<std::string>();
    type = data["type"].get<std::string>() == "admin" ? UserType::admin : UserType::voter;
    disabled = data["disabled"].get<bool>();
    
    // Load access levels
    restrictedAccess.clear();
    const auto& access = data["access"];
    
    // Handle national access
    if (access.contains("national") && access["national"].get<bool>()) {
        restrictedAccess[ElectionLevel::national] = {};
    }
    
    // Handle regional access
    if (access.contains("regions")) {
        std::set<int> regions;
        for (const auto& region : access["regions"]) {
            regions.insert(region.get<int>());
        }
        restrictedAccess[ElectionLevel::regional] = regions;
    }
    
    // Handle municipal access
    if (access.contains("municipalities")) {
        std::set<int> municipalities;
        for (const auto& municipality : access["municipalities"]) {
            municipalities.insert(municipality.get<int>());
        }
        restrictedAccess[ElectionLevel::municipal] = municipalities;
    }
    
    // Handle local access
    if (access.contains("localities")) {
        std::set<int> localities;
        for (const auto& locality : access["localities"]) {
            localities.insert(locality.get<int>());
        }
        restrictedAccess[ElectionLevel::local] = localities;
    }
    
    // Handle non-government access
    if (access.contains("nonGovernment")) {
        std::set<int> nonGov;
        for (const auto& entity : access["nonGovernment"]) {
            nonGov.insert(entity.get<int>());
        }
        restrictedAccess[ElectionLevel::nonGovernment] = nonGov;
    }
    
    // Load voting history
    votingHistory.clear();
    if (data.contains("votingHistory")) {
        for (const auto& vote : data["votingHistory"]) {
            int electionId = vote["electionId"].get<int>();
            votingHistory[electionId] = {
                {"timestamp", vote["timestamp"].get<std::string>()},
                {"type", vote["voteType"].get<std::string>()}
            };
        }
    }
}

// Methods
bool User::canVoteIn(const Election& election) const {
    if (disabled || type != UserType::voter) return false;
    // Implementation will be added when Election class is implemented
    return true;
}

bool User::grantAccess(const ElectionLevel level, const int entityId) {
    if (type == UserType::admin) return false;  // Admins have full access
    restrictedAccess[level].insert(entityId);
    return true;
}

bool User::revokeAccess(const ElectionLevel level, const int entityId) {
    if (type == UserType::admin) return false;  // Admins have full access
    restrictedAccess[level].erase(entityId);
    return true;
}

bool User::hasAccessTo(const ElectionLevel level, const int entityId) const {
    if (type == UserType::admin) return true;  // Admins have full access
    const auto it = restrictedAccess.find(level);
    if (it == restrictedAccess.end()) return false;
    return it->second.empty() || it->second.contains(entityId);
}

bool User::hasVotedIn(const int electionId) const {
    return votingHistory.contains(electionId);
}

// Voting methods
bool User::castVote(const int electionId, int candidateId) {
    if (hasVotedIn(electionId)) return false;
    
    // Update voting history
    votingHistory[electionId] = {
        {"candidateId", candidateId},
        {"timestamp", std::time(nullptr)},
        {"type", "single"}
    };
    
    // Update vote log and election results
    return updateVoteLog(electionId, candidateId, true) && 
           updateElectionResults(electionId, candidateId, true);
}

bool User::changeVote(const int electionId, const int oldCandidateId, int newCandidateId) {
    if (!hasVotedIn(electionId)) return false;
    
    // Update voting history
    votingHistory[electionId] = {
        {"candidateId", newCandidateId},
        {"timestamp", std::time(nullptr)},
        {"type", "single"}
    };
    
    // Update vote log and election results
    return updateVoteLog(electionId, newCandidateId, true) && 
           updateElectionResults(electionId, oldCandidateId, false) &&  // Decrement old vote
           updateElectionResults(electionId, newCandidateId, true);     // Increment new vote
}

// File operations
bool User::updateVoteLog(int electionId, int candidateId, bool isNewVote) const {
    json voteLog = DataManager::getInstance().loadData("votes_log");
    if (isNewVote) {
        voteLog.push_back({
            {"voterId", id},
            {"electionId", electionId},
            {"vote", {
                {"candidateId", candidateId},
                {"timestamp", std::time(nullptr)},
                {"type", "single"}
            }}
        });
    } else {
        // Remove old vote
        auto it = std::ranges::find_if(voteLog,
                                       [this, electionId](const json& vote) {
                                           return vote["voterId"] == id && vote["electionId"] == electionId;
                                       });
        if (it != voteLog.end()) {
            voteLog.erase(it);
        }
    }
    return DataManager::getInstance().saveData("votes_log", voteLog);
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

// User management
bool User::deleteUser(int userId) {
    json users = DataManager::getInstance().loadData("users");
    const auto it = std::ranges::find_if(users,
                                   [userId](const json& user) { return user["id"] == userId; });
    
    if (it == users.end()) return false;
    
    // Remove user
    users.erase(it);
    
    // Remove user's votes from vote log
    json voteLog = DataManager::getInstance().loadData("votes_log");
    voteLog.erase(
        std::ranges::remove_if(voteLog,
                               [userId](const json& vote) { return vote["voterId"] == userId; }).begin(),
        voteLog.end()
    );
    
    // Update election results
    json elections = DataManager::getInstance().loadData("elections");
    for (auto& election : elections) {
        for (auto& candidate : election["candidates"]) {
            const int votes = candidate["votes"].get<int>();
            candidate["votes"] = votes - 1;  // Decrement vote count
        }
    }
    
    return DataManager::getInstance().saveData("users", users) &&
           DataManager::getInstance().saveData("votes_log", voteLog) &&
           DataManager::getInstance().saveData("elections", elections);
}

bool User::removeVotes() const {
    json voteLog = DataManager::getInstance().loadData("votes_log");
    voteLog.erase(
        std::ranges::remove_if(voteLog,
                               [this](const json& vote) { return vote["voterId"] == id; }).begin(),
        voteLog.end()
    );
    
    // Update election results
    json elections = DataManager::getInstance().loadData("elections");
    for (auto& election : elections) {
        for (auto& candidate : election["candidates"]) {
            const int votes = candidate["votes"].get<int>();
            candidate["votes"] = votes - 1;  // Decrement vote count
        }
    }
    
    return DataManager::getInstance().saveData("votes_log", voteLog) &&
           DataManager::getInstance().saveData("elections", elections);
}

// Serialization
json User::toJson() const {
    json data = {
        {"id", id},
        {"username", username},
        {"password", password},
        {"type", type == UserType::admin ? "admin" : "voter"},
        {"disabled", disabled}
    };
    
    // Convert restricted access to the expected format
    json access_json_output; // Use a new json object for clarity

    // Handle "national" access
    auto it_national = restrictedAccess.find(ElectionLevel::national);
    if (it_national != restrictedAccess.end() && it_national->second.empty()) {
        access_json_output["national"] = true; // Full national access if key exists and set is empty
    } else {
        access_json_output["national"] = false; // Otherwise, no full national access
    }

    // Handle "regions"
    json regions_array = json::array();
    auto it_regions = restrictedAccess.find(ElectionLevel::regional);
    if (it_regions != restrictedAccess.end()) {
        for (const int id : it_regions->second) {
            regions_array.push_back(id);
        }
    }
    access_json_output["regions"] = regions_array;

    // Handle "municipalities"
    json municipalities_array = json::array();
    auto it_municipalities = restrictedAccess.find(ElectionLevel::municipal);
    if (it_municipalities != restrictedAccess.end()) {
        for (const int id : it_municipalities->second) {
            municipalities_array.push_back(id);
        }
    }
    access_json_output["municipalities"] = municipalities_array;

    // Handle "localities"
    json localities_array = json::array();
    auto it_localities = restrictedAccess.find(ElectionLevel::local);
    if (it_localities != restrictedAccess.end()) {
        for (const int id : it_localities->second) {
            localities_array.push_back(id);
        }
    }
    access_json_output["localities"] = localities_array;

    // Handle "nonGovernment"
    json non_gov_array = json::array();
    auto it_non_gov = restrictedAccess.find(ElectionLevel::nonGovernment);
    if (it_non_gov != restrictedAccess.end()) {
        for (const int id : it_non_gov->second) {
            non_gov_array.push_back(id);
        }
    }
    access_json_output["nonGovernment"] = non_gov_array;
    
    data["access"] = access_json_output;
    
    // Convert voting history to array format
    json history = json::array();
    for (const auto& [electionId, voteData] : votingHistory) {
        history.push_back({
            {"electionId", electionId},
            {"timestamp", voteData["timestamp"].get<std::string>()},
            {"voteType", voteData["type"].get<std::string>()}
        });
    }
    data["votingHistory"] = history;
    
    return data;
}

User User::fromJson(const json& data) {
    return User(data);
}
