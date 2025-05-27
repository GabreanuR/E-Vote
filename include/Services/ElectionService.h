#ifndef ELECTIONSERVICE_H
#define ELECTIONSERVICE_H

#include <string>
#include <vector>
#include <memory> // For std::shared_ptr
#include <nlohmann/json.hpp>
#include "../Models/Election.h"
#include "../Utils/Types.h" // For ElectionLevel, ElectionStatus
// Forward declaration if DataManager is only used for path or simple ops
// #include "../Utils/DataManager.h" 

using json = nlohmann::json;

class ElectionService {
private:
    static ElectionService* instance;
    std::vector<std::shared_ptr<Election>> elections;
    std::string electionsFilePath = "data/elections.json";
    // For ID generation, similar to LocationService if using counters.json
    // std::string countersFilePath = "data/counters.json"; 

    ElectionService(); // Private constructor
    void loadElections();
    int getNextElectionId();

public:
    ElectionService(const ElectionService&) = delete;
    ElectionService& operator=(const ElectionService&) = delete;
    ~ElectionService();

    static ElectionService& getInstance();

    // CRUD operations for Elections
    std::shared_ptr<Election> createElection(const std::string& name, ElectionLevel level, const std::string& votingSystem);
    bool toggleElectionStatus(int electionId);
    std::shared_ptr<Election> getElection(int electionId) const;
    const std::vector<std::shared_ptr<Election>>& getAllElections() const;

    // Candidate management within an election
    bool addCandidateToElection(int electionId, int candidateId);
    bool removeCandidateFromElection(int electionId, int candidateId);
    
    // Ensure elections are saved on program exit (called from main.cpp)
    void saveElections() const;

};

#endif //ELECTIONSERVICE_H 