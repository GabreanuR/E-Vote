#include "../../include/Models/Election.h"
#include "../../include/Models/Candidate.h"      // For Candidate objects
#include "../../include/Models/VotingStrategy.h" // For VotingStrategy
#include "../../include/Models/MajorityVoting.h" // For MajorityVoting strategy
#include "../../include/Models/ProportionalVoting.h" // For ProportionalVoting strategy
#include <stdexcept> // For std::runtime_error
#include <iostream> // For debug prints, remove later

// Existing constructor (if you keep it, otherwise remove or comment out)
Election::Election(int id, const std::string& title, const std::string& level, 
                   const std::string& zone, const std::string& votingSystem)
    : id(id), title(title), level(level), zone(zone), votingSystem(votingSystem), status("upcoming") {
    // Initialize strategy based on votingSystem string
    if (votingSystem == "majority") {
        strategy = std::make_unique<MajorityVoting>();
    } else if (votingSystem == "proportional") {
        strategy = std::make_unique<ProportionalVoting>();
    } else {
        // Default or error handling
        strategy = nullptr; // Or throw an exception
    }
}

Election::Election(const json& data) {
    id = data["id"].get<int>();
    title = data["title"].get<std::string>();
    status = data["status"].get<std::string>();

    const auto& typeData = data["type"];
    level = typeData["level"].get<std::string>();
    if (typeData.contains("zone") && !typeData["zone"].is_null()) {
        zone = typeData["zone"].get<std::string>();
    } else {
        zone = "";
    }
    votingSystem = typeData["votingSystem"].get<std::string>();

    const auto& datesData = data["dates"];
    startDate = datesData["start"].get<std::string>();
    endDate = datesData["end"].get<std::string>();

    if (data.contains("candidates")) {
        for (const auto& candData : data["candidates"]) {
            candidates.emplace_back(candData); // Use Candidate's JSON constructor
        }
    }

    // Initialize strategy based on votingSystem string
    if (votingSystem == "majority") {
        strategy = std::make_unique<MajorityVoting>();
    } else if (votingSystem == "proportional") {
        strategy = std::make_unique<ProportionalVoting>();
    } else {
        throw std::runtime_error("Unknown voting system: " + votingSystem);
    }
}

json Election::toJson() const {
    json data;
    data["id"] = id;
    data["title"] = title;
    data["status"] = status;

    data["type"] = {
        {"level", level},
        {"votingSystem", votingSystem}
    };
    if (!zone.empty()) {
        data["type"]["zone"] = zone;
    } else {
        data["type"]["zone"] = nullptr;
    }

    data["dates"] = {
        {"start", startDate},
        {"end", endDate}
    };

    data["candidates"] = json::array();
    for (const auto& c : candidates) {
        data["candidates"].push_back(c.toJson()); // Use Candidate's toJson method
    }

    return data;
}

// Implement other methods like computeResults, addCandidate, etc. later

void Election::computeResults() {
    if (strategy) {
        // strategy->calculateResults(candidates, /* pass votes if needed */);
        // This depends on VotingStrategy's interface
        std::cout << "computeResults called for election " << id << std::endl;
    } else {
        throw std::runtime_error("Voting strategy not set for election " + std::to_string(id));
    }
}

void Election::addCandidate(const Candidate& candidate) {
    candidates.push_back(candidate);
    // Potentially save changes or update other state
}

void Election::removeCandidate(int candidateId) {
    candidates.erase(
        std::remove_if(candidates.begin(), candidates.end(),
                       [candidateId](const Candidate& c){ return c.id == candidateId; }),
        candidates.end()
    );
    // Potentially save changes or update other state
}

void Election::start() {
    status = "active";
    // Save change to file if necessary
}

void Election::end() {
    status = "ended"; // Or "closed", "completed"
    computeResults();
    // Save change to file if necessary
}

