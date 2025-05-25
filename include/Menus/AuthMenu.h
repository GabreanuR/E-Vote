#ifndef AUTHMENU_H
#define AUTHMENU_H

#include "Menus/Meniu.h"
#include "Models/User.h"
#include <string>

class AuthMenu final : public Meniu {
    static const std::string jsonPath;  // Path to the JSON file: "users.json"
    UserType userType;
    std::string authenticatedUser;    // Store username after successful login

    [[nodiscard]] bool verifyCredentials(const std::string& username, const std::string& password) const;

public:
    explicit AuthMenu(UserType type);

    void display() override;
};

#endif
