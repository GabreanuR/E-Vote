#ifndef ELECTION_H
#define ELECTION_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "../Utils/Types.h"
// #include "VotingStrategy.h" // Will remove for now
// #include "Candidate.h" // Candidate objects not stored directly, only IDs

using json = nlohmann::json;

class Election {
private:
    int id;
    std::string name;
    ElectionLevel electionLevel;
    std::string votingSystem; // Placeholder, could be its own class later
    ElectionStatus status;
    std::vector<int> candidateIds;
    int voteTotal;

public:
    // Constructors
    Election(int id, const std::string& name, ElectionLevel level, const std::string& votingSystem);
    Election(); // Default constructor for convenience, e.g. for json deserialization

    // Getters
    int getId() const { return id; }
    const std::string& getName() const { return name; }
    ElectionLevel getElectionLevel() const { return electionLevel; }
    const std::string& getVotingSystem() const { return votingSystem; }
    ElectionStatus getStatus() const { return status; }
    const std::vector<int>& getCandidateIds() const { return candidateIds; }
    int getVoteTotal() const { return voteTotal; }

    // Setters
    void setId(int newId) { id = newId; } // Typically ID is set on creation
    void setName(const std::string& newName) { name = newName; }
    void setElectionLevel(ElectionLevel newLevel) { electionLevel = newLevel; }
    void setVotingSystem(const std::string& newSystem) { votingSystem = newSystem; }
    void setStatus(ElectionStatus newStatus) { status = newStatus; }
    // candidateIds will be managed by add/remove methods
    void setVoteTotal(int newVoteTotal) { voteTotal = newVoteTotal; }

    // Methods
    void addCandidateId(int candidateId);
    void removeCandidateId(int candidateId);
    bool hasCandidate(int candidateId) const;


    // Serialization
    [[nodiscard]] json toJson() const;
    static Election fromJson(const json& j);


    // Utility for converting ElectionLevel to string and back (for JSON)
    static std::string electionLevelToString(ElectionLevel level);
    static ElectionLevel stringToElectionLevel(const std::string& levelStr);

    // Utility for converting ElectionStatus to string and back (for JSON)
    static std::string electionStatusToString(ElectionStatus status);
    static ElectionStatus stringToElectionStatus(const std::string& statusStr);
};

// operator<< for easy printing
std::ostream& operator<<(std::ostream& os, const Election& election);

#endif
