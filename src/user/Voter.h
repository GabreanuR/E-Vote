#ifndef VOTER_H
#define VOTER_H

#include "User.h"

class Voter : public User {
public:
    explicit Voter(const std::string& id);
    void showDashboard() override;
};

#endif
