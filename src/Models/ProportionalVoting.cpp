#include "../../include/Models/ProportionalVoting.h"
#include "../../include/Models/Election.h"
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <iomanip>

void ProportionalVoting::computeResults(Election& election) {
    // Get candidates and their votes
    const auto& candidates = election.getCandidates();
    
    // // Calculate total votes
    // int totalVotes = 0;
    // for (const auto& candidate : candidates) {
    //     totalVotes += candidate.votes;
    // }
    //
    // if (totalVotes == 0) {
    //     std::cout << "No votes cast in election " << election.getId() << "\n";
    //     return;
    // }
    //
    // // Print results
    // std::cout << "Election " << election.getId() << " results:\n";
    // std::cout << "Total votes cast: " << totalVotes << "\n\n";
    //
    // // Sort candidates by votes (descending)
    // std::vector<Candidate> sortedCandidates = candidates;
    // std::sort(sortedCandidates.begin(), sortedCandidates.end(),
    //     [](const Candidate& a, const Candidate& b) {
    //         return a.votes > b.votes;
    //     });
    //
    // // Print each candidate's results
    // std::cout << std::fixed << std::setprecision(2);
    // for (const auto& candidate : sortedCandidates) {
    //     double percentage = (static_cast<double>(candidate.votes) / totalVotes) * 100.0;
    //     std::cout << candidate.name << " (" << candidate.party << "):\n"
    //               << "  Votes: " << candidate.votes << "\n"
    //               << "  Percentage: " << percentage << "%\n\n";
    // }
}

