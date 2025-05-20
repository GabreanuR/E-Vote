#ifndef ADMIN_H
#define ADMIN_H

#include <memory>
#include <vector>
#include "User.h"
#include "Election.h"

class Admin : public User {
public:
    explicit Admin(const std::string& id);

    //static void viewElectionsDashboard();

    void showDashboard() override;
    static void createElection(std::vector<std::unique_ptr<Election>>& elections);
};

#endif
