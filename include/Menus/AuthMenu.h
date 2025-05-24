#ifndef AUTHMENU_H
#define AUTHMENU_H

#include "Menus/Meniu.h"
#include "Models/User.h"
#include <string>

class AuthMenu final : public Meniu {
private:
    UserType userType;
    std::string jsonPath;             // Path to the JSON file: "voters.json" or "admins.json"
    std::string authenticatedUser;    // Store username after successful login

    bool verifyCredentials(const std::string& username, const std::string& password) const;

public:
    explicit AuthMenu(UserType type);

    void display() override;
    std::string getAuthenticatedUsername() const;
};

#endif
