#include "../include/Models/Election.h"
#include "../include/Models/Candidate.h"
#include "../include/Services/CandidateService.h"
#include "../include/Utils/Types.h"
#include "../include/Services/LocationService.h"
#include "../include/Models/Location.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <string>


Election::Election(const int id, std::string name, const ElectionLevel level, const VotingSystemType votingSystem,
                   const int locationId)
    : id(id), name(std::move(name)), electionLevel(level), votingSystem(votingSystem),
      status(ElectionStatus::created), voteTotal(0), locationId(locationId) {
}

Election::Election()
    : id(-1), electionLevel(ElectionLevel::national), votingSystem(VotingSystemType::majority),
      status(ElectionStatus::created), voteTotal(0), locationId(0) {
}

void Election::addCandidateId(const int candidateId) {
    if (!hasCandidate(candidateId)) {
        candidateIds.push_back(candidateId);
    }
}

bool Election::hasCandidate(const int candidateId) const {
    return std::ranges::find(candidateIds, candidateId) != candidateIds.end();
}

bool Election::recordVote(const int candidateId, const int userId) {
    const CandidateService &candidateService = CandidateService::getInstance();

    if (const std::shared_ptr<Candidate> candidate = candidateService.getCandidate(candidateId)) {
        candidate->setVotes(candidate->getVotes() + 1);
        votersWhoVoted.insert(userId);
        voteTotal++;
        return true;
    }

    return false;
}

bool Election::hasVoted(const int userId) const {
    return votersWhoVoted.contains(userId);
}

int Election::getVotesForCandidate(const int candidateId) const {
    const CandidateService &candidateService = CandidateService::getInstance();
    if (const std::shared_ptr<Candidate> candidate = candidateService.getCandidate(candidateId)) {
        if (hasCandidate(candidateId)) {
            return candidate->getVotes();
        }
    }
    return 0;
}

json Election::toJson() const {
    json j;
    j["id"] = id;
    j["name"] = name;
    j["electionLevel"] = electionLevelToString(electionLevel);
    j["votingSystem"] = votingSystemTypeToString(votingSystem);
    j["status"] = electionStatusToString(status);
    j["candidateIds"] = candidateIds;
    j["locationId"] = locationId;
    j["voteTotal"] = voteTotal;
    j["votersWhoVoted"] = votersWhoVoted;
    return j;
}

Election Election::fromJson(const json &j) {
    Election election;
    election.id = j.at("id").get<int>();
    election.name = j.at("name").get<std::string>();

    try {
        election.electionLevel = stringToElectionLevel(j.at("electionLevel").get<std::string>());
    } catch (const std::exception &e) {
        std::cerr << "Warning: Error parsing electionLevel for election ID " << election.id <<
                ". Defaulting to national. Error: " << e.what() << std::endl;
        election.electionLevel = ElectionLevel::national;
    }
    try {
        election.votingSystem = stringToVotingSystemType(j.at("votingSystem").get<std::string>());
    } catch (const std::exception &e) {
        std::cerr << "Warning: Error parsing votingSystem for election ID " << election.id <<
                ". Defaulting to majority. Error: " << e.what() << std::endl;
        election.votingSystem = VotingSystemType::majority;
    }
    try {
        election.status = stringToElectionStatus(j.at("status").get<std::string>());
    } catch (const std::exception &e) {
        std::cerr << "Warning: Error parsing status for election ID " << election.id <<
                ". Defaulting to created. Error: " << e.what() << std::endl;
        election.status = ElectionStatus::created;
    }

    if (j.contains("candidateIds") && j.at("candidateIds").is_array()) {
        election.candidateIds = j.at("candidateIds").get<std::vector<int> >();
    }

    election.locationId = j.value("locationId", 0);

    if (j.contains("votersWhoVoted") && j.at("votersWhoVoted").is_array()) {
        election.votersWhoVoted = j.at("votersWhoVoted").get<std::set<int> >();
    }

    election.voteTotal = j.value("voteTotal", 0);

    return election;
}

std::string Election::electionLevelToString(const ElectionLevel level) {
    switch (level) {
        case ElectionLevel::national: return "national";
        case ElectionLevel::regional: return "regional";
        case ElectionLevel::municipal: return "municipal";
        case ElectionLevel::local: return "local";
        case ElectionLevel::non_governmental: return "non-governmental";
        default: throw std::invalid_argument("Invalid ElectionLevel");
    }
}

ElectionLevel Election::stringToElectionLevel(const std::string &levelStr) {
    if (levelStr == "national") return ElectionLevel::national;
    if (levelStr == "regional") return ElectionLevel::regional;
    if (levelStr == "municipal") return ElectionLevel::municipal;
    if (levelStr == "local") return ElectionLevel::local;
    if (levelStr == "non-governmental" || levelStr == "nonGovernment") return ElectionLevel::non_governmental;
    std::cerr << "Warning: Unknown ElectionLevel string: " << levelStr << ". Defaulting to national." << std::endl;
    return ElectionLevel::national;
}

std::string Election::electionStatusToString(const ElectionStatus status) {
    switch (status) {
        case ElectionStatus::created: return "created";
        case ElectionStatus::open: return "open";
        case ElectionStatus::closed: return "closed";
        default: throw std::invalid_argument("Invalid ElectionStatus");
    }
}

ElectionStatus Election::stringToElectionStatus(const std::string &statusStr) {
    if (statusStr == "created") return ElectionStatus::created;
    if (statusStr == "open") return ElectionStatus::open;
    if (statusStr == "closed") return ElectionStatus::closed;
    std::cerr << "Warning: Unknown ElectionStatus string: " << statusStr << ". Defaulting may occur." << std::endl;
    return ElectionStatus::created;
}

std::string Election::votingSystemTypeToString(const VotingSystemType type) {
    switch (type) {
        case VotingSystemType::majority: return "majority";
        case VotingSystemType::proportional: return "proportional";
        default: throw std::invalid_argument("Invalid VotingSystemType");
    }
}

VotingSystemType Election::stringToVotingSystemType(const std::string &typeStr) {
    if (typeStr == "majority") return VotingSystemType::majority;
    if (typeStr == "proportional") return VotingSystemType::proportional;
    std::cerr << "Warning: Unknown VotingSystemType string: " << typeStr << ". Defaulting may occur." << std::endl;
    return VotingSystemType::majority;
}

std::ostream &operator<<(std::ostream &os, const Election &election) {
    os << "Election ID: " << election.getId() << "\n"
            << "  Name: " << election.getName() << "\n"
            << "  Level: " << Election::electionLevelToString(election.getElectionLevel()) << "\n";

    os << "  Associated Location ID: ";
    if (election.getLocationId() == 0) {
        if (election.getElectionLevel() == ElectionLevel::national) {
            os << "National (all applicable locations)";
        } else {
            os << "None specified / National";
        }
    } else {
        os << election.getLocationId();
    }
    os << "\n";

    os << "  Voting System: " << Election::votingSystemTypeToString(election.getVotingSystem()) << "\n"
            << "  Status: " << Election::electionStatusToString(election.getStatus()) << "\n"
            << "  Candidate IDs: ";
    if (const auto &candIds = election.getCandidateIds(); candIds.empty()) {
        os << "None";
    } else {
        for (size_t i = 0; i < candIds.size(); ++i) {
            os << candIds[i] << (i == candIds.size() - 1 ? "" : ", ");
        }
    }
    os << "\n" << "  Vote Total: " << election.getVoteTotal();
    return os;
}

std::istream &operator>>(std::istream &is, Election &election) {
    std::string name_temp;
    std::cout << "Enter election name (or press Enter to cancel): ";
    std::getline(is, name_temp);
    if (name_temp.empty()) {
        is.clear();
        throw UserInputCancelledException();
    }
    election.setName(name_temp);
    election.setLocationId(0);

    std::cout << "\nSelect Election Level:\n";
    const std::vector<std::pair<std::string, ElectionLevel> > electionLevelOptions = {
        {"National", ElectionLevel::national},
        {"Regional", ElectionLevel::regional},
        {"Municipal", ElectionLevel::municipal},
        {"Local", ElectionLevel::local},
        {"Non-Governmental", ElectionLevel::non_governmental}
    };
    for (size_t i = 0; i < electionLevelOptions.size(); ++i) {
        std::cout << (i + 1) << ". " << electionLevelOptions[i].first << std::endl;
    }

    int attempts_level = 3;
    std::string input_str_level;
    auto selectedLevel = ElectionLevel::national;

    while (attempts_level > 0) {
        std::cout << "Enter choice for Election Level (1-" << electionLevelOptions.size() <<
                ") (or press Enter to cancel) [" << attempts_level << " tries left]: ";
        std::getline(is, input_str_level);
        if (input_str_level.empty()) {
            is.clear();
            throw UserInputCancelledException();
        }

        try {
            if (const int level_choice_val = std::stoi(input_str_level);
                level_choice_val >= 1 && level_choice_val <= static_cast<int>(electionLevelOptions.size())) {
                selectedLevel = electionLevelOptions[static_cast<size_t>(level_choice_val) - 1].second;
                election.setElectionLevel(selectedLevel);
                break;
            }
            std::cout << "Invalid choice. Please enter a number between 1 and " << electionLevelOptions.size() <<
                    ".\n";
        } catch (const std::invalid_argument &) {
            std::cout << "Invalid input. Please enter a number.\n";
        } catch (const std::out_of_range &) {
            std::cout << "Input out of range. Please enter a valid number.\n";
        }
        attempts_level--;
        if (attempts_level == 0) {
            std::cout << "Too many failed attempts for Election Level.\n";
            throw UserInputCancelledException();
        }
        std::cout << "\nSelect Election Level:\n";
        for (size_t i = 0; i < electionLevelOptions.size(); ++i) {
            std::cout << (i + 1) << ". " << electionLevelOptions[i].first << std::endl;
        }
    }

    if (selectedLevel != ElectionLevel::national) {
        std::cout << "\n--- Select Associated Location ---\n";
        std::vector<std::shared_ptr<Location> > availableLocations;
        std::string locationTypeString;

        switch (selectedLevel) {
            case ElectionLevel::regional:
                availableLocations = LocationService::getInstance().getAllRegionsAsBase();
                locationTypeString = "Region";
                break;
            case ElectionLevel::municipal:
                availableLocations = LocationService::getInstance().getAllMunicipalitiesAsBase();
                locationTypeString = "Municipality";
                break;
            case ElectionLevel::local:
                availableLocations = LocationService::getInstance().getAllLocalitiesAsBase();
                locationTypeString = "Locality";
                break;
            case ElectionLevel::non_governmental:
                availableLocations = LocationService::getInstance().getAllNonGovernmentAsBase();
                locationTypeString = "Non-Governmental Entity";
                break;
            default:
                break;
        }

        if (availableLocations.empty()) {
            std::cout << "No " << locationTypeString <<
                    "s found in the system. Cannot proceed with this election level unless locations are added.";
            std::cout << "\nElection creation cancelled due to missing locations for the selected level.\n";
            throw UserInputCancelledException();
        }

        std::cout << "Available " << locationTypeString << "s:\n";
        for (size_t i = 0; i < availableLocations.size(); ++i) {
            if (availableLocations[i]) {
                std::cout << "  " << (i + 1) << ". ID: " << availableLocations[i]->getId() << " - " <<
                        availableLocations[i]->getName() << "\n";
            }
        }

        int attempts_location = 3;
        std::string input_str_location;

        while (attempts_location > 0) {
            std::cout << "Enter number of the desired " << locationTypeString
                    << " (1-" << availableLocations.size() << ") (or press Enter to cancel selection for this level) ["
                    << attempts_location << " tries left]: ";
            std::getline(is, input_str_location);

            if (input_str_location.empty()) {
                std::cout << "\nNo location selected for a non-national election. Election creation cancelled.\n";
                throw UserInputCancelledException();
            }
            try {
                if (const int location_choice_val = std::stoi(input_str_location);
                    location_choice_val >= 1 && location_choice_val <= static_cast<int>(availableLocations.size())) {
                    election.setLocationId(availableLocations[static_cast<size_t>(location_choice_val) - 1]->getId());
                    std::cout << "Selected " << locationTypeString << ": ID " << election.getLocationId() << " - " <<
                            availableLocations[static_cast<size_t>(location_choice_val) - 1]->getName() << "\n";
                    break;
                }
                std::cout << "Invalid choice. Please enter a number between 1 and " << availableLocations.size() <<
                        ".\n";
            } catch (const std::invalid_argument &) {
                std::cout << "Invalid input. Please enter a number.\n";
            } catch (const std::out_of_range &) {
                std::cout << "Input out of range. Please enter a valid number.\n";
            }
            attempts_location--;
            if (attempts_location == 0) {
                std::cout << "Too many failed attempts for Location selection.\n";
                throw UserInputCancelledException();
            }
            std::cout << "\nAvailable " << locationTypeString << "s:\n";
            for (size_t i = 0; i < availableLocations.size(); ++i) {
                if (availableLocations[i]) {
                    std::cout << "  " << (i + 1) << ". ID: " << availableLocations[i]->getId() << " - " <<
                            availableLocations[i]->getName() << "\n";
                }
            }
        }

        if (election.getLocationId() == 0) {
            std::cout << "No valid location was selected for a non-national election. Election creation cancelled.\n";
            throw UserInputCancelledException();
        }
    } else {
        std::cout <<
                "\nNational election selected. No specific location ID needed (defaults to 0 or handled as national).\n";
        election.setLocationId(0);
    }

    std::cout << "\nSelect Voting System Type:\n";
    const std::vector<std::pair<std::string, VotingSystemType> > votingSystemOptions = {
        {"Majority", VotingSystemType::majority},
        {"Proportional", VotingSystemType::proportional}
    };
    for (size_t i = 0; i < votingSystemOptions.size(); ++i) {
        std::cout << (i + 1) << ". " << votingSystemOptions[i].first << std::endl;
    }

    int attempts_system = 3;
    std::string input_str_system;

    while (attempts_system > 0) {
        std::cout << "Enter choice for Voting System (1-" << votingSystemOptions.size() <<
                ") (or press Enter to cancel) [" << attempts_system << " tries left]: ";
        std::getline(is, input_str_system);
        if (input_str_system.empty()) {
            is.clear();
            throw UserInputCancelledException();
        }

        try {
            if (const int system_choice_val = std::stoi(input_str_system);
                system_choice_val >= 1 && system_choice_val <= static_cast<int>(votingSystemOptions.size())) {
                election.setVotingSystem(votingSystemOptions[static_cast<size_t>(system_choice_val) - 1].second);
                break;
            }
            std::cout << "Invalid choice. Please enter a number between 1 and " << votingSystemOptions.size() <<
                    ".\n";
        } catch (const std::invalid_argument &) {
            std::cout << "Invalid input. Please enter a number.\n";
        } catch (const std::out_of_range &) {
            std::cout << "Input out of range. Please enter a valid number.\n";
        }
        attempts_system--;
        if (attempts_system == 0) {
            std::cout << "Too many failed attempts for Voting System Type.\n";
            throw UserInputCancelledException();
        }
        std::cout << "\nSelect Voting System Type:\n";
        for (size_t i = 0; i < votingSystemOptions.size(); ++i) {
            std::cout << (i + 1) << ". " << votingSystemOptions[i].first << std::endl;
        }
    }

    election.setStatus(ElectionStatus::created);

    return is;
}
