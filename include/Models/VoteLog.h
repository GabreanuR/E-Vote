#ifndef VOTELOG_H
#define VOTELOG_H

#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class VoteLog {
public:
    int userId;
    int electionId;
    int candidateId;

    // Constructors
    VoteLog() : userId(0), electionId(0), candidateId(0) {}
    VoteLog(int userId, int electionId, int candidateId)
        : userId(userId), electionId(electionId), candidateId(candidateId) {}
    explicit VoteLog(const json& data);

    // Static methods
    static std::vector<VoteLog> loadAll();
    static void save(const VoteLog& vote);
    static void updateVote(int userId, int electionId, int newCandidateId);

    // Serialization
    [[nodiscard]] json toJson() const;
};

#endif
