#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "Election.h"

enum UserType {
    Voter,
    Admin
};

class User {
private:
    int id;
    std::string username;
    std::string password;
    UserType type;
    std::vector<std::string> access; // e.g., ["national", "local-Cluj"]

public:
    // Constructor
    User(int id, const std::string& username, const std::string& password, UserType type);
    
    // Getters
    int getId() const { return id; }
    const std::string& getUsername() const { return username; }
    const std::string& getPassword() const { return password; }
    UserType getType() const { return type; }
    const std::vector<std::string>& getAccess() const { return access; }
    
    // Setters
    void setUsername(const std::string& newUsername) { username = newUsername; }
    void setPassword(const std::string& newPassword) { password = newPassword; }
    void setType(UserType newType) { type = newType; }
    void setAccess(const std::vector<std::string>& newAccess) { access = newAccess; }
    
    // Methods
    bool canVoteIn(const Election& election) const;
    void addAccess(const std::string& location);
    void removeAccess(const std::string& location);
};

#endif
