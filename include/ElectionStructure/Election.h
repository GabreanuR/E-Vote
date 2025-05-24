#ifndef ELECTION_H
#define ELECTION_H

#include <vector>
#include <memory>
#include <string>
#include "Candidate.h"
#include "Vote.h"
#include "ElectionLevelUtils.h"
#include "VotingStrategy.h"

class Election {
    int id;
    std::string name;
    ElectionLevel level;
    std::vector<Candidate> candidates;
    std::vector<Vote> votes;
    std::unique_ptr<VotingStrategy> strategy;

public:
    Election(int id, std::string name, ElectionLevel level, std::unique_ptr<VotingStrategy> strategy);

    void addCandidate(const Candidate& candidate);
    void addVote(const Vote& vote);

    void computeResults() const;

    int getId() const;
    ElectionLevel getLevel() const;
    const std::string& getName() const;
    const std::vector<Candidate>& getCandidates() const;
    const std::vector<Vote>& getVotes() const;
};
#endif //ELECTION_H
