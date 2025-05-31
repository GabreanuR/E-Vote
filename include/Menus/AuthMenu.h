#ifndef AUTHMENU_H
#define AUTHMENU_H

#include "Menus/Meniu.h"
#include "Models/User.h"
#include <string>
#include <memory>

class AuthMenu final : public Meniu {
    UserType userType;
    std::shared_ptr<User> currentUser;

    [[nodiscard]] std::shared_ptr<User> attemptLogin(const std::string &username, const std::string &password) const;

public:
    explicit AuthMenu(UserType type);

    void display() override;
};

#endif
