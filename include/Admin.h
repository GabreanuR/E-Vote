#ifndef ADMIN_H
#define ADMIN_H

#include <memory>
#include <vector>
#include "Election.h"

class Admin{
public:

    //static void viewElectionsDashboard();

    static void createElection(std::vector<std::unique_ptr<Election>>& elections);
};

#endif
