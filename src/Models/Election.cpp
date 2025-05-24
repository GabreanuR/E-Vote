#include "../../include/Models/Election.h"
#include <stdexcept>

Election::Election(int id, std::string name, ElectionLevel level, std::unique_ptr<VotingStrategy> strategy)
    : id(id), name(std::move(name)), level(level), strategy(std::move(strategy)) {}

void Election::addCandidate(const Candidate& candidate) {
    candidates.push_back(candidate);
}

void Election::addVote(const Vote& vote) {
    // optionally: validate that candidateId is valid
    votes.push_back(vote);
}

void Election::computeResults() const {
    if (!strategy) {
        throw std::runtime_error("No voting strategy set for this election.");
    }
    strategy->computeResults(votes, candidates);
}

int Election::getId() const { return id; }
ElectionLevel Election::getLevel() const { return level; }
const std::string& Election::getName() const { return name; }
const std::vector<Candidate>& Election::getCandidates() const { return candidates; }
const std::vector<Vote>& Election::getVotes() const { return votes; }
