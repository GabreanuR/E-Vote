#ifndef VOTERMENU_H
#define VOTERMENU_H

#include "../Meniu.h"
#include "../include/Models/User.h"
#include <memory>

class VoterMenu final : public Meniu {
    std::shared_ptr<User> loggedInUser;

    void vote() const;

    void viewResults() const;

public:
    explicit VoterMenu(std::shared_ptr<User> user);

    void printVoterMenuText() const;

    void display() override;
};

#endif
