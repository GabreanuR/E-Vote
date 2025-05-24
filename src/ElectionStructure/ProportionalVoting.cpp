#include "../../include/ElectionStructure/ProportionalVoting.h"
#include <unordered_map>
#include <iostream>
#include <iomanip>

void ProportionalVoting::computeResults(const std::vector<Vote>& votes, const std::vector<Candidate>& candidates) {
    std::unordered_map<int, int> voteCounts;
    int totalVotes = 0;

    for (const auto& vote : votes) {
        voteCounts[vote.getCandidateId()]++;
        totalVotes++;
    }

    std::cout << "Proportional Voting Result:\n";

    for (const auto& candidate : candidates) {
        int candidateVotes = voteCounts[candidate.getId()];
        double percentage = totalVotes > 0 ? (100.0 * candidateVotes) / totalVotes : 0.0;

        std::cout << "  " << std::setw(15) << candidate.getName()
                  << " - " << std::setw(3) << candidateVotes << " votes ("
                  << std::fixed << std::setprecision(2) << percentage << "%)\n";
    }
}

std::string ProportionalVoting::getName() const {
    return "Proportional";
}
