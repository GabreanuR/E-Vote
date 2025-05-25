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

class Election;  // Forward declaration

class User {
    static const std::string countersFile;  // Will store in counters.json
    
    int id;
    std::string username;
    std::string password;
    UserType type;
    bool disabled;
    
    // List of restricted entities for each level
    // Empty list means full access to that level
    // Non-empty list means access only to those specific entities
    std::map<ElectionLevel, std::set<int>> restrictedAccess;
    std::map<int, json> votingHistory;  // electionId -> vote data
    
    static int generateId();   // Generate next available ID from counters.json
    
    // Helper method to initialize access levels based on user type
    void initializeAccessLevels() {
        if (type == UserType::admin) {
            // Admins have empty restrictedAccess (full access to everything)
            restrictedAccess.clear();
        } else {
            // Voters start with all levels restricted (empty sets)
            restrictedAccess = {
                {ElectionLevel::national, {}},
                {ElectionLevel::regional, {}},
                {ElectionLevel::municipal, {}},
                {ElectionLevel::local, {}},
                {ElectionLevel::nonGovernment, {}}
            };
        }
    }

    // Validation methods
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
    // Custom exception class for user creation errors
    class UserCreationError : public std::runtime_error {
    public:
        explicit UserCreationError(const std::string& message) : std::runtime_error(message) {}
    };

    // Constructors
    User(std::string username, std::string password, const UserType type) 
        : id(generateId()),
          username(std::move(username)),
          password(std::move(password)),
          type(type),
          disabled(false)
    {
        // Validate username
        if (!isUsernameValid(this->username)) {
            throw UserCreationError("Username must be at least 3 characters long");
        }

        // Check if username exists
        if (doesUsernameExist(this->username)) {
            throw UserCreationError("Username already exists");
        }

        // Validate password
        if (!isPasswordValid(this->password)) {
            throw UserCreationError("Password must be at least 6 characters long");
        }

        initializeAccessLevels();
    }
    
    explicit User(const json& data);
    
    // Getters
    [[nodiscard]] int getId() const { return id; }
    [[nodiscard]] const std::string& getUsername() const { return username; }
    [[nodiscard]] UserType getType() const { return type; }
    [[nodiscard]] bool isDisabled() const { return disabled; }
    [[nodiscard]] const std::map<ElectionLevel, std::set<int>>& getRestrictedAccess() const { return restrictedAccess; }
    [[nodiscard]] const std::map<int, json>& getVotingHistory() const { return votingHistory; }
    
    // Setters
    void setDisabled(const bool status) { disabled = status; }
    
    // Methods
    [[nodiscard]] bool canVoteIn(const Election& election) const;
    bool grantAccess(ElectionLevel level, int entityId);     // Grant access to specific entity
    [[nodiscard]] auto revokeAccess(ElectionLevel level, int entityId) -> bool;    // Revoke access to specific entity
    [[nodiscard]] bool hasAccessTo(ElectionLevel level, int entityId) const;  // Check access to specific entity
    [[nodiscard]] bool hasVotedIn(int electionId) const;
    
    // Voting methods
    [[nodiscard]] bool castVote(int electionId, int candidateId);  // Main voting method that updates all files
    [[nodiscard]] bool changeVote(int electionId, int oldCandidateId, int newCandidateId);  // Change vote and update counts
    
    // File operations
    [[nodiscard]] bool updateVoteLog(int electionId, int candidateId, bool isNewVote = true) const;  // Update votes_log.json
    [[nodiscard]] static bool updateElectionResults(int electionId, int candidateId, bool isIncrement = true);  // Update elections.json
    
    // User management
    [[nodiscard]] static bool deleteUser(int userId);  // Delete user and all their data
    [[nodiscard]] bool removeVotes() const;  // Remove all votes cast by this user
    
    // Serialization
    [[nodiscard]] json toJson() const;
    static User fromJson(const json& data);
};

#endif
