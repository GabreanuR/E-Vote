#ifndef ELECTION_H
#define ELECTION_H

#include <string>
#include <vector>
#include <set>
#include <nlohmann/json.hpp>
#include "../Utils/Types.h"

using json = nlohmann::json;

class Election {
    int id;
    std::string name;
    ElectionLevel electionLevel;
    VotingSystemType votingSystem;
    ElectionStatus status;
    std::vector<int> candidateIds;
    int voteTotal;
    std::set<int> votersWhoVoted;
    int locationId;

public:
    Election();

    Election(int id, std::string name, ElectionLevel level, VotingSystemType votingSystem, int locationId = 0);

    [[nodiscard]] int getId() const { return id; }
    [[nodiscard]] const std::string &getName() const { return name; }
    [[nodiscard]] ElectionLevel getElectionLevel() const { return electionLevel; }
    [[nodiscard]] VotingSystemType getVotingSystem() const { return votingSystem; }
    [[nodiscard]] ElectionStatus getStatus() const { return status; }
    [[nodiscard]] const std::vector<int> &getCandidateIds() const { return candidateIds; }
    [[nodiscard]] int getVoteTotal() const { return voteTotal; }
    [[nodiscard]] int getLocationId() const { return locationId; }

    void setName(const std::string &newName) { this->name = newName; }
    void setElectionLevel(const ElectionLevel newLevel) { this->electionLevel = newLevel; }
    void setVotingSystem(const VotingSystemType newSystem) { this->votingSystem = newSystem; }
    void setStatus(const ElectionStatus newStatus) { this->status = newStatus; }
    void setLocationId(const int id) { this->locationId = id; }

    void addCandidateId(int candidateId);

    [[nodiscard]] bool hasCandidate(int candidateId) const;

    bool recordVote(int candidateId, int userId);

    [[nodiscard]] bool hasVoted(int userId) const;

    [[nodiscard]] int getVotesForCandidate(int candidateId) const;

    [[nodiscard]] json toJson() const;

    static Election fromJson(const json &j);

    static std::string electionLevelToString(ElectionLevel level);

    static std::string electionStatusToString(ElectionStatus status);

    static std::string votingSystemTypeToString(VotingSystemType type);

    static ElectionLevel stringToElectionLevel(const std::string &levelStr);

    static ElectionStatus stringToElectionStatus(const std::string &statusStr);

    static VotingSystemType stringToVotingSystemType(const std::string &typeStr);
};

std::ostream &operator<<(std::ostream &os, const Election &election);

std::istream &operator>>(std::istream &is, Election &election);

#endif
