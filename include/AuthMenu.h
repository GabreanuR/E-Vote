#ifndef AUTHMENU_H
#define AUTHMENU_H

#include "Meniu.h"
#include "UserType.h"
#include <string>

class AuthMenu : public Meniu {
    UserType userType;
    string jsonPath;             // Path to the JSON file: "voters.json" or "admins.json"
    string authenticatedUser;    // Store username after successful login

    bool verifyCredentials(const std::string& username, const std::string& password) const;

public:
    AuthMenu(UserType type);

    void display() override;
    string getAuthenticatedUsername() const;
};


#endif
