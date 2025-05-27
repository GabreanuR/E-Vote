#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <nlohmann/json.hpp>
#include "../Utils/Types.h"
#include "../Utils/DataManager.h"
#include <stdexcept>

using json = nlohmann::json;

class Election;

class User {
    static const std::string countersFile;
    
    int id;
    std::string username;
    std::string password;
    UserType type;
    bool disabled;

    std::map<ElectionLevel, std::set<int>> restrictedAccess;
    
    static int generateId();

    void initializeAccessLevels() {
        if (type == UserType::admin) {
            json locations = DataManager::getInstance().loadData("data/locations.json");

            restrictedAccess = {
                {ElectionLevel::national, {}},
                {ElectionLevel::regional, {}},
                {ElectionLevel::municipal, {}},
                {ElectionLevel::local, {}},
                {ElectionLevel::nonGovernment, {}}
            };

            if (locations.contains("regions")) {
                for (const auto& [id, _] : locations["regions"].items()) {
                    restrictedAccess[ElectionLevel::regional].insert(std::stoi(id));
                }
            }
            
            if (locations.contains("municipalities")) {
                for (const auto& [id, _] : locations["municipalities"].items()) {
                    restrictedAccess[ElectionLevel::municipal].insert(std::stoi(id));
                }
            }
            
            if (locations.contains("localities")) {
                for (const auto& [id, _] : locations["localities"].items()) {
                    restrictedAccess[ElectionLevel::local].insert(std::stoi(id));
                }
            }
            
            if (locations.contains("nonGovernment")) {
                for (const auto& [id, _] : locations["nonGovernment"].items()) {
                    restrictedAccess[ElectionLevel::nonGovernment].insert(std::stoi(id));
                }
            }
        } else {
            restrictedAccess = {
                {ElectionLevel::national, {}}
            };
        }
    }

    static bool isUsernameValid(const std::string& username) {
        return !username.empty() && username.length() >= 3;
    }

    static bool isPasswordValid(const std::string& password) {
        return !password.empty() && password.length() >= 6;
    }

    [[nodiscard]] static bool doesUsernameExist(const std::string& username) {
        json users = DataManager::getInstance().loadData("users");
        for (const auto& user : users) {
            if (user["username"] == username) {
                return true;
            }
        }
        return false;
    }
    
public:
    class UserCreationError : public std::runtime_error {
    public:
        explicit UserCreationError(const std::string& message) : std::runtime_error(message) {}
    };

    static void updateAdminAccess(const ElectionLevel level, int entityId);

    User(std::string username, std::string password, const UserType type) 
        : id(generateId()),
          username(std::move(username)),
          password(std::move(password)),
          type(type),
          disabled(false)
    {
        if (!isUsernameValid(this->username)) {
            throw UserCreationError("Username must be at least 3 characters long");
        }

        if (doesUsernameExist(this->username)) {
            throw UserCreationError("Username already exists");
        }

        if (!isPasswordValid(this->password)) {
            throw UserCreationError("Password must be at least 6 characters long");
        }

        initializeAccessLevels();
    }
    
    explicit User(const json& data);

    [[nodiscard]] int getId() const { return id; }
    [[nodiscard]] const std::string& getUsername() const { return username; }
    [[nodiscard]] UserType getType() const { return type; }
    [[nodiscard]] bool isDisabled() const { return disabled; }
    [[nodiscard]] const std::map<ElectionLevel, std::set<int>>& getRestrictedAccess() const { return restrictedAccess; }

    void setDisabled(const bool status) { disabled = status; }

    [[nodiscard]] bool canVoteIn(const Election& election) const;
    bool grantAccess(ElectionLevel level, int entityId);
    [[nodiscard]] auto revokeAccess(ElectionLevel level, int entityId) -> bool;
    [[nodiscard]] bool hasAccessTo(ElectionLevel level, int entityId) const;

    [[nodiscard]] static bool updateElectionResults(int electionId, int candidateId, bool isIncrement = true);

    [[nodiscard]] static bool deleteUser(int userId);
    [[nodiscard]] bool removeVotes() const;

    [[nodiscard]] json toJson() const;
    static User fromJson(const json& data);
};

#endif
