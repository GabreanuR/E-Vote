#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "AccessLevel.h"

class User {
    int id;
    std::string type; // "voter" or "admin"
    std::string username;
    std::string password;
    bool disabled;
    std::vector<int> hasVotedFor;
    std::optional<AccessLevel> access; // std::nullopt for admins

public:
    User() = default;
    User(int id, std::string type, std::string username, std::string password, bool disabled,
         std::vector<int> hasVotedFor, std::optional<AccessLevel> access);

    bool isDisabled() const;
    bool isAdmin() const;
    bool isPasswordCorrect(const std::string& input) const;
    void markVoted(int electionId);
    bool alreadyVoted(int electionId) const;

    const std::string& getUsername() const;
    int getId() const;
    std::string getType() const;
    const std::optional<AccessLevel>& getAccessLevel() const;

    nlohmann::json toJson() const;
    static User fromJson(const nlohmann::json& j);
};

#endif
