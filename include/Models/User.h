#ifndef USER_H
#define USER_H

#include "../Utils/Types.h"
#include <string>
#include <map>
#include <set>
#include <nlohmann/json.hpp>
#include <stdexcept>

using json = nlohmann::json;

class Election;

class User {
    int id;
    std::string username;
    std::string password;
    UserType type;
    bool disabled;

    std::map<ElectionLevel, std::set<int> > restrictedAccess;

public:
    class UserCreationError final : public std::runtime_error {
    public:
        explicit UserCreationError(const std::string &message) : std::runtime_error(message) {
        }
    };

    User(const int id, std::string username, std::string password, const UserType type, const bool disabled = false)
        : id(id),
          username(std::move(username)),
          password(std::move(password)),
          type(type),
          disabled(disabled) {
    }

    explicit User(const json &data);

    [[nodiscard]] int getId() const { return id; }
    [[nodiscard]] const std::string &getUsername() const { return username; }
    [[nodiscard]] const std::string &getPassword() const { return password; }
    [[nodiscard]] UserType getType() const { return type; }
    [[nodiscard]] bool isDisabled() const { return disabled; }

    [[nodiscard]] const std::map<ElectionLevel, std::set<int> > &getRestrictedAccess() const {
        return restrictedAccess;
    }

    void setUsername(const std::string &newUsername) { username = newUsername; }
    void setPassword(const std::string &newPassword) { password = newPassword; }
    void setType(const UserType newType) { type = newType; }
    void setDisabled(const bool status) { disabled = status; }
    void setRestrictedAccess(const std::map<ElectionLevel, std::set<int> > &accessMap) { restrictedAccess = accessMap; }

    bool grantAccess(ElectionLevel level, int entityId = -1);

    bool revokeAccess(ElectionLevel level, int entityId);

    [[nodiscard]] bool hasAccessTo(ElectionLevel level, int entityId) const;

    [[nodiscard]] json toJson() const;
};

#endif
