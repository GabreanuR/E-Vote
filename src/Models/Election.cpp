#include "../../include/Models/Election.h"
#include "../../include/Models/Candidate.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include "../../include/Menus/Meniu.h"
#include "../../include/Utils/Types.h"
#include "../../include/Services/LocationService.h"
#include "../../include/Models/Location.h"


Election::Election(int id, const std::string& name, ElectionLevel level, VotingSystemType votingSystem, int locationId)
    : id(id), name(name), electionLevel(level), votingSystem(votingSystem),
      status(ElectionStatus::created), locationId(locationId), voteTotal(0) {
}

Election::Election()
    : id(-1), name(""), electionLevel(ElectionLevel::national), votingSystem(VotingSystemType::majority),
      status(ElectionStatus::created), locationId(0), voteTotal(0) {
}

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
    return j;
}

Election Election::fromJson(const json& j) {
    Election election;
    election.id = j.at("id").get<int>();
    election.name = j.at("name").get<std::string>();
    election.electionLevel = stringToElectionLevel(j.at("electionLevel").get<std::string>());
    election.votingSystem = stringToVotingSystemType(j.at("votingSystem").get<std::string>());
    election.status = stringToElectionStatus(j.at("status").get<std::string>());
    if (j.contains("candidateIds") && j.at("candidateIds").is_array()) {
        election.candidateIds = j.at("candidateIds").get<std::vector<int>>();
    }

    if (j.contains("locationId") && j.at("locationId").is_number_integer()) {
        election.locationId = j.at("locationId").get<int>();
    } else {
        election.locationId = 0;
    }
    election.voteTotal = j.at("voteTotal").get<int>();
    return election;
}

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

std::string Election::votingSystemTypeToString(VotingSystemType type) {
    switch (type) {
        case VotingSystemType::majority: return "majority";
        case VotingSystemType::proportional: return "proportional";
        default: throw std::invalid_argument("Invalid VotingSystemType");
    }
}

VotingSystemType Election::stringToVotingSystemType(const std::string& typeStr) {
    if (typeStr == "majority") return VotingSystemType::majority;
    if (typeStr == "proportional") return VotingSystemType::proportional;
    throw std::invalid_argument("Invalid VotingSystemType string: " + typeStr);
}

std::ostream& operator<<(std::ostream& os, const Election& election) {
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
    const auto& candIds = election.getCandidateIds();
    if (candIds.empty()) {
        os << "None";
    } else {
        for (size_t i = 0; i < candIds.size(); ++i) {
            os << candIds[i] << (i == candIds.size() - 1 ? "" : ", ");
        }
    }
    os << "\n" << "  Vote Total: " << election.getVoteTotal();
    return os;
}

std::istream& operator>>(std::istream& is, Election& election) {
    std::string name_temp;
    std::cout << "Enter election name (or press Enter to cancel): ";
    std::getline(is, name_temp);
    if (name_temp.empty()) {
        throw UserInputCancelledException();
    }
    election.setName(name_temp);
    election.setLocationId(0);

    std::cout << "\nSelect Election Level:\n";
    std::vector<std::pair<std::string, ElectionLevel>> electionLevelOptions = {
        {"National", ElectionLevel::national},
        {"Regional", ElectionLevel::regional},
        {"Municipal", ElectionLevel::municipal},
        {"Local", ElectionLevel::local},
        {"Non-Government", ElectionLevel::nonGovernment}
    };
    for (size_t i = 0; i < electionLevelOptions.size(); ++i) {
        std::cout << (i + 1) << ". " << electionLevelOptions[i].first << std::endl;
    }

    int level_choice_val = -1;
    int attempts_level = 3;
    std::string input_str_level;
    ElectionLevel selectedLevel = ElectionLevel::national;

    while (attempts_level > 0) {
        std::cout << "Enter choice for Election Level (1-" << electionLevelOptions.size() << ") (or press Enter to cancel) [" << attempts_level << " tries left]: ";
        std::getline(is, input_str_level);
        if (input_str_level.empty()) {
            throw UserInputCancelledException();
        }

        try {
            level_choice_val = std::stoi(input_str_level);
            if (level_choice_val >= 1 && level_choice_val <= static_cast<int>(electionLevelOptions.size())) {
                selectedLevel = electionLevelOptions[static_cast<size_t>(level_choice_val) - 1].second;
                election.setElectionLevel(selectedLevel);
                break;
            } else {
                std::cout << "Invalid choice. Please enter a number between 1 and " << electionLevelOptions.size() << ".\n";
            }
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid input. Please enter a number.\n";
        } catch (const std::out_of_range&) {
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
        std::vector<std::shared_ptr<Location>> availableLocations;
        std::string locationTypeString = "";

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
            case ElectionLevel::nonGovernment:
                availableLocations = LocationService::getInstance().getAllNonGovernmentAsBase();
                locationTypeString = "Non-Government Entity";
                break;
            default:
                break;
        }

        if (availableLocations.empty()) {
            std::cout << "No " << locationTypeString << "s found in the system. Cannot proceed with this election level unless locations are added.";
            std::cout << "\nElection creation cancelled due to missing locations for the selected level.\n";
            throw UserInputCancelledException(); 
        }

        std::cout << "Available " << locationTypeString << "s:\n";
        for (size_t i = 0; i < availableLocations.size(); ++i) {
            if(availableLocations[i]){
                 std::cout << "  " << (i + 1) << ". ID: " << availableLocations[i]->getId() << " - " << availableLocations[i]->getName() << "\n";
            }
        }
        
        int location_choice_val = -1;
        int attempts_location = 3;
        std::string input_str_location;

        while(attempts_location > 0) {
            std::cout << "Enter number of the desired " << locationTypeString 
                      << " (1-" << availableLocations.size() << ") (or press Enter to cancel selection for this level) [" << attempts_location << " tries left]: ";
            std::getline(is, input_str_location);

            if (input_str_location.empty()) {
                if (selectedLevel != ElectionLevel::national) {
                     std::cout << "\nNo location selected for a non-national election. Election creation cancelled.\n";
                     throw UserInputCancelledException();
                }
            }
            try {
                location_choice_val = std::stoi(input_str_location);
                if (location_choice_val >= 1 && location_choice_val <= static_cast<int>(availableLocations.size())) {
                    election.setLocationId(availableLocations[static_cast<size_t>(location_choice_val) - 1]->getId());
                    std::cout << "Selected " << locationTypeString << ": ID " << election.getLocationId() << " - " << availableLocations[static_cast<size_t>(location_choice_val) - 1]->getName() << "\n";
                    break; 
                } else {
                    std::cout << "Invalid choice. Please enter a number between 1 and " << availableLocations.size() << ".\n";
                }
            } catch (const std::invalid_argument&) {
                std::cout << "Invalid input. Please enter a number.\n";
            } catch (const std::out_of_range&) {
                 std::cout << "Input out of range. Please enter a valid number.\n";
            }
            attempts_location--;
             if (attempts_location == 0) {
                std::cout << "Too many failed attempts for Location selection.\n";
                throw UserInputCancelledException();
            }
            std::cout << "\nAvailable " << locationTypeString << "s:\n";
            for (size_t i = 0; i < availableLocations.size(); ++i) {
                if(availableLocations[i]){
                     std::cout << "  " << (i + 1) << ". ID: " << availableLocations[i]->getId() << " - " << availableLocations[i]->getName() << "\n";
                }
            }
        }

        if (election.getLocationId() == 0 && selectedLevel != ElectionLevel::national) {
            std::cout << "No valid location was selected for a non-national election. Election creation cancelled.\n";
            throw UserInputCancelledException();
        }


    } else {
         std::cout << "\nNational election selected. It will apply to all relevant jurisdictions by default.\n";
         election.setLocationId(0);
    }

    std::cout << "\nSelect Voting System Type:\n";
    std::vector<std::pair<std::string, VotingSystemType>> votingSystemOptions = {
        {"Majority", VotingSystemType::majority},
        {"Proportional", VotingSystemType::proportional}
    };
    for (size_t i = 0; i < votingSystemOptions.size(); ++i) {
        std::cout << (i + 1) << ". " << votingSystemOptions[i].first << std::endl;
    }
    
    int system_choice = -1;
    int attempts_system = 3;
    std::string input_str_system;

    while (attempts_system > 0) {
        std::cout << "Enter choice for Voting System (1-" << votingSystemOptions.size() << ") (or press Enter to cancel) [" << attempts_system << " tries left]: ";
        std::getline(is, input_str_system);
        if (input_str_system.empty()) {
            throw UserInputCancelledException();
        }

        try {
            system_choice = std::stoi(input_str_system);
            if (system_choice >= 1 && system_choice <= static_cast<int>(votingSystemOptions.size())) {
                election.setVotingSystem(votingSystemOptions[static_cast<size_t>(system_choice) - 1].second);
                break;
            } else {
                std::cout << "Invalid choice. Please enter a number between 1 and " << votingSystemOptions.size() << ".\n";
            }
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid input. Please enter a number.\n";
        } catch (const std::out_of_range&) {
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

    return is;
}


