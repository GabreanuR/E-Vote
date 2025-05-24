#include "../../include/ElectionStructure/MajorityVoting.h"

#include <algorithm>
#include <unordered_map>
#include <iostream>

void MajorityVoting::computeResults(const std::vector<Vote>& votes, const std::vector<Candidate>& candidates) {
    std::unordered_map<int, int> voteCounts;

    for (const auto& vote : votes) {
        voteCounts[vote.getCandidateId()]++;
    }

    int maxVotes = 0;
    int winnerId = -1;

    for (const auto& [candidateId, count] : voteCounts) {
        if (count > maxVotes) {
            maxVotes = count;
            winnerId = candidateId;
        }
    }

    const auto it = ranges::find_if(candidates, [&](const Candidate& c) {
        return c.getId() == winnerId;
    });

    if (it != candidates.end()) {
        std::cout << "Majority Voting Result: Winner is " << it->getName() << " with " << maxVotes << " votes.\n";
    } else {
        std::cout << "Majority Voting Result: No winner found.\n";
    }
}

std::string MajorityVoting::getName() const {
    return "Majority";
}
