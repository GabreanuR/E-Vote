#ifndef ELECTIONSERVICE_H
#define ELECTIONSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "../Models/Election.h"
#include "../Utils/Types.h"

using json = nlohmann::json;

class ElectionService {
    static ElectionService* instance;
    std::vector<std::shared_ptr<Election>> elections;
    std::string electionsFilePath = "data/elections.json";

    ElectionService();
    void loadElections();
    int getNextElectionId();

public:
    ElectionService(const ElectionService&) = delete;
    ElectionService& operator=(const ElectionService&) = delete;
    ~ElectionService();

    static ElectionService& getInstance();

    std::shared_ptr<Election> createElection(const std::string& name, ElectionLevel level, 
                                           VotingSystemType votingSystem, int locationId);
    bool toggleElectionStatus(int electionId);
    std::shared_ptr<Election> getElection(int electionId) const;
    const std::vector<std::shared_ptr<Election>>& getAllElections() const;

    bool addCandidateToElection(int electionId, int candidateId);
    bool removeCandidateFromElection(int electionId, int candidateId);

    void saveElections() const;

};

#endif