#ifndef ELECTIONSERVICE_H
#define ELECTIONSERVICE_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "../Models/Election.h"
#include "../Utils/DataManager.h"
#include "../Menus/Meniu.h"

using json = nlohmann::json;

class ElectionService {
private:
    static ElectionService* instance;
    ElectionService() = default;

public:
    static ElectionService& getInstance();
    void createElection();
    void manageExistingElections();
    void viewElectionResults();
    void voteInElection();
};

#endif //ELECTIONSERVICE_H 