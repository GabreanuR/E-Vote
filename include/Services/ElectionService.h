#ifndef ELECTIONSERVICE_H
#define ELECTIONSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "../Models/Election.h"
#include "../Models/User.h"
#include "../Utils/Types.h"

using json = nlohmann::json;

class ElectionService {
    std::vector<std::shared_ptr<Election> > electionsCache;
    std::string electionsFilePath = "data/elections.json";

    ElectionService();

    void loadElectionsFromDataManager();

    [[nodiscard]] int getNextElectionId() const;

public:
    ElectionService(const ElectionService &) = delete;

    ElectionService &operator=(const ElectionService &) = delete;

    ~ElectionService() = default;

    static ElectionService &getInstance();


    void saveElectionsToDataManager() const;

    std::shared_ptr<Election> createElection(const std::string &name, ElectionLevel level,
                                             VotingSystemType votingSystem,
                                             int locationId);

    [[nodiscard]] bool toggleElectionStatus(int electionId) const;

    [[nodiscard]] std::shared_ptr<Election> getElection(int electionId) const;

    [[nodiscard]] const std::vector<std::shared_ptr<Election> > &getAllElections() const;

    [[nodiscard]] std::vector<std::shared_ptr<Election> > getAvailableElectionsForUser(
        const std::shared_ptr<User> &user) const;

    [[nodiscard]] bool castVote(int electionId, int candidateId, const std::shared_ptr<User> &user) const;

    [[nodiscard]] bool addCandidateToElection(int electionId, int candidateId) const;
};

#endif
