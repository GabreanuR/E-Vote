#include "../../include/Models/MajorityVoting.h"
#include "../../include/Models/Election.h"
#include <nlohmann/json.hpp>
#include <algorithm>
#include <unordered_map>
#include <iostream>

using json = nlohmann::json;

void MajorityVoting::computeResults(Election& election) {
    // Get candidates and their votes
    const auto& candidates = election.getCandidates();
    
    // Find the candidate with the most votes
    auto winner = std::max_element(candidates.begin(), candidates.end(),
        [](const Candidate& a, const Candidate& b) {
            return a.votes < b.votes;
        });
    
    if (winner != candidates.end()) {
        std::cout << "Election " << election.getId() << " results:\n";
        std::cout << "Winner: " << winner->name << " (" << winner->party << ") with " 
                  << winner->votes << " votes\n";
        
        // Print all candidates and their votes
        std::cout << "\nAll candidates:\n";
        for (const auto& candidate : candidates) {
            std::cout << candidate.name << " (" << candidate.party << "): " 
                      << candidate.votes << " votes\n";
        }
    } else {
        std::cout << "No candidates in election " << election.getId() << "\n";
    }
}
