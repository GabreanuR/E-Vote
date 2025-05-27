#ifndef ELECTION_H
#define ELECTION_H

#include <string>
#include <vector>
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
    int locationId;
    int voteTotal;

public:
    Election(int id, const std::string& name, ElectionLevel level, VotingSystemType votingSystem, int locationId = 0);
    Election();

    int getId() const { return id; }
    const std::string& getName() const { return name; }
    ElectionLevel getElectionLevel() const { return electionLevel; }
    VotingSystemType getVotingSystem() const { return votingSystem; }
    ElectionStatus getStatus() const { return status; }
    const std::vector<int>& getCandidateIds() const { return candidateIds; }
    int getLocationId() const { return locationId; }
    int getVoteTotal() const { return voteTotal; }

    void setId(int newId) { id = newId; }
    void setName(const std::string& newName) { name = newName; }
    void setElectionLevel(ElectionLevel newLevel) { electionLevel = newLevel; }
    void setVotingSystem(VotingSystemType newSystem) { votingSystem = newSystem; }
    void setStatus(ElectionStatus newStatus) { status = newStatus; }
    void setLocationId(int newLocationId) { locationId = newLocationId; }
    void setVoteTotal(int newVoteTotal) { voteTotal = newVoteTotal; }

    void addCandidateId(int candidateId);
    void removeCandidateId(int candidateId);
    bool hasCandidate(int candidateId) const;

    [[nodiscard]] json toJson() const;
    static Election fromJson(const json& j);

    static std::string electionLevelToString(ElectionLevel level);
    static ElectionLevel stringToElectionLevel(const std::string& levelStr);

    static std::string electionStatusToString(ElectionStatus status);
    static ElectionStatus stringToElectionStatus(const std::string& statusStr);

    static std::string votingSystemTypeToString(VotingSystemType type);
    static VotingSystemType stringToVotingSystemType(const std::string& typeStr);
};

std::ostream& operator<<(std::ostream& os, const Election& election);

std::istream& operator>>(std::istream& is, Election& election);

#endif
