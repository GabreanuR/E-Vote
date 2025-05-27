#include "../../include/Models/Election.h"
#include "../../include/Models/Candidate.h"      // For Candidate objects
#include "../../include/Models/VotingStrategy.h" // For VotingStrategy
#include "../../include/Models/MajorityVoting.h" // For MajorityVoting strategy
#include "../../include/Models/ProportionalVoting.h" // For ProportionalVoting strategy
#include <stdexcept> // For std::runtime_error
#include <iostream>  // For std::ostream
#include <algorithm> // For std::find and std::remove

// Constructor
Election::Election(int id, const std::string& name, ElectionLevel level, const std::string& votingSystem)
    : id(id), name(name), electionLevel(level), votingSystem(votingSystem),
      status(ElectionStatus::created), voteTotal(0) {
    // candidateIds is already initialized as empty by std::vector default constructor
}

// Default constructor
Election::Election()
    : id(-1), name(""), electionLevel(ElectionLevel::national), votingSystem(""),
      status(ElectionStatus::created), voteTotal(0) {
}

// Candidate ID Management
void Election::addCandidateId(int candidateId) {
    if (!hasCandidate(candidateId)) {
        candidateIds.push_back(candidateId);
    }
}

void Election::removeCandidateId(int candidateId) {
    candidateIds.erase(std::remove(candidateIds.begin(), candidateIds.end(), candidateId), candidateIds.end());
}

bool Election::hasCandidate(int candidateId) const {
    return std::find(candidateIds.begin(), candidateIds.end(), candidateId) != candidateIds.end();
}

// Serialization
json Election::toJson() const {
    json j;
    j["id"] = id;
    j["name"] = name;
    j["electionLevel"] = electionLevelToString(electionLevel);
    j["votingSystem"] = votingSystem;
    j["status"] = electionStatusToString(status);
    j["candidateIds"] = candidateIds; // Direct serialization of vector of ints
    j["voteTotal"] = voteTotal;
    return j;
}

Election Election::fromJson(const json& j) {
    Election election;
    election.id = j.at("id").get<int>();
    election.name = j.at("name").get<std::string>();
    election.electionLevel = stringToElectionLevel(j.at("electionLevel").get<std::string>());
    election.votingSystem = j.at("votingSystem").get<std::string>();
    election.status = stringToElectionStatus(j.at("status").get<std::string>());
    if (j.contains("candidateIds") && j.at("candidateIds").is_array()) {
        election.candidateIds = j.at("candidateIds").get<std::vector<int>>();
    }
    election.voteTotal = j.at("voteTotal").get<int>();
    return election;
}

// Utility for ElectionLevel to/from string
std::string Election::electionLevelToString(ElectionLevel level) {
    switch (level) {
        case ElectionLevel::national: return "national";
        case ElectionLevel::regional: return "regional";
        case ElectionLevel::municipal: return "municipal";
        case ElectionLevel::local: return "local";
        case ElectionLevel::nonGovernment: return "nonGovernment";
        default: throw std::invalid_argument("Invalid ElectionLevel");
    }
}

ElectionLevel Election::stringToElectionLevel(const std::string& levelStr) {
    if (levelStr == "national") return ElectionLevel::national;
    if (levelStr == "regional") return ElectionLevel::regional;
    if (levelStr == "municipal") return ElectionLevel::municipal;
    if (levelStr == "local") return ElectionLevel::local;
    if (levelStr == "nonGovernment") return ElectionLevel::nonGovernment;
    throw std::invalid_argument("Invalid ElectionLevel string: " + levelStr);
}

// Utility for ElectionStatus to/from string
std::string Election::electionStatusToString(ElectionStatus st) {
    switch (st) {
        case ElectionStatus::created: return "created";
        case ElectionStatus::open: return "open";
        case ElectionStatus::closed: return "closed";
        default: throw std::invalid_argument("Invalid ElectionStatus");
    }
}

ElectionStatus Election::stringToElectionStatus(const std::string& statusStr) {
    if (statusStr == "created") return ElectionStatus::created;
    if (statusStr == "open") return ElectionStatus::open;
    if (statusStr == "closed") return ElectionStatus::closed;
    throw std::invalid_argument("Invalid ElectionStatus string: " + statusStr);
}

// operator<< for easy printing
std::ostream& operator<<(std::ostream& os, const Election& election) {
    os << "Election ID: " << election.getId() << "\n"
       << "  Name: " << election.getName() << "\n"
       << "  Level: " << Election::electionLevelToString(election.getElectionLevel()) << "\n"
       << "  Voting System: " << election.getVotingSystem() << "\n"
       << "  Status: " << Election::electionStatusToString(election.getStatus()) << "\n"
       << "  Candidate IDs: ";
    const auto& ids = election.getCandidateIds();
    if (ids.empty()) {
        os << "None";
    } else {
        for (size_t i = 0; i < ids.size(); ++i) {
            os << ids[i] << (i == ids.size() - 1 ? "" : ", ");
        }
    }
    os << "\n" << "  Vote Total: " << election.getVoteTotal();
    return os;
}

// Existing constructor (if you keep it, otherwise remove or comment out)
// Election::Election(int id, const std::string& title, const std::string& level,
//                    const std::string& zone, const std::string& votingSystem)
//     : id(id), title(title), level(level), zone(zone), votingSystem(votingSystem), status("upcoming") {
//     // Initialize strategy based on votingSystem string
//     if (votingSystem == "majority") {
//         strategy = std::make_unique<MajorityVoting>();
//     } else if (votingSystem == "proportional") {
//         strategy = std::make_unique<ProportionalVoting>();
//     } else {
//         // Default or error handling
//         strategy = nullptr; // Or throw an exception
//     }
// }

// Election::Election(const json& data) {
//     id = data["id"].get<int>();
//     title = data["title"].get<std::string>();
//     status = data["status"].get<std::string>();
//
//     const auto& typeData = data["type"];
//     level = typeData["level"].get<std::string>();
//     if (typeData.contains("zone") && !typeData["zone"].is_null()) {
//         zone = typeData["zone"].get<std::string>();
//     } else {
//         zone = "";
//     }
//     votingSystem = typeData["votingSystem"].get<std::string>();
//
//     const auto& datesData = data["dates"];
//     startDate = datesData["start"].get<std::string>();
//     endDate = datesData["end"].get<std::string>();
//
//     if (data.contains("candidates")) {
//         for (const auto& candData : data["candidates"]) {
//             candidates.emplace_back(candData); // Use Candidate's JSON constructor
//         }
//     }
//
//     // Initialize strategy based on votingSystem string
//     if (votingSystem == "majority") {
//         strategy = std::make_unique<MajorityVoting>();
//     } else if (votingSystem == "proportional") {
//         strategy = std::make_unique<ProportionalVoting>();
//     } else {
//         throw std::runtime_error("Unknown voting system: " + votingSystem);
//     }
// }

// json Election::toJson() const {
//     json data;
//     data["id"] = id;
//     data["title"] = title;
//     data["status"] = status;
//
//     data["type"] = {
//         {"level", level},
//         {"votingSystem", votingSystem}
//     };
//     if (!zone.empty()) {
//         data["type"]["zone"] = zone;
//     } else {
//         data["type"]["zone"] = nullptr;
//     }
//
//     data["dates"] = {
//         {"start", startDate},
//         {"end", endDate}
//     };
//
//     data["candidates"] = json::array();
//     for (const auto& c : candidates) {
//         data["candidates"].push_back(c.toJson()); // Use Candidate's toJson method
//     }
//
//     return data;
// }
//
// // Implement other methods like computeResults, addCandidate, etc. later
//
// void Election::computeResults() {
//     if (strategy) {
//         // strategy->calculateResults(candidates, /* pass votes if needed */);
//         // This depends on VotingStrategy's interface
//         std::cout << "computeResults called for election " << id << std::endl;
//     } else {
//         throw std::runtime_error("Voting strategy not set for election " + std::to_string(id));
//     }
// }
//
// void Election::addCandidate(const Candidate& candidate) {
//     candidates.push_back(candidate);
//     // Potentially save changes or update other state
// }

// void Election::removeCandidate(int candidateId) {
//     candidates.erase(
//         std::remove_if(candidates.begin(), candidates.end(),
//                        [candidateId](const Candidate& c){ return c.id == candidateId; }),
//         candidates.end()
//     );
//     // Potentially save changes or update other state
// }

// void Election::start() {
//     status = "active";
//     // Save change to file if necessary
// }
//
// void Election::end() {
//     status = "ended"; // Or "closed", "completed"
//     computeResults();
//     // Save change to file if necessary
// }

