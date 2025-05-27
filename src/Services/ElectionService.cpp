#include "../../include/Services/ElectionService.h"
#include "../../include/Utils/DataManager.h"
#include "../../include/Services/CandidateService.h" // To check candidate count/existence
#include "../../include/Models/Election.h" // For Election::electionStatusToString, etc.
#include <iostream> // For cerr, cout
#include <fstream>  // For std::ifstream, std::ofstream for direct file ops if needed
#include <algorithm> // For std::max_element, std::find_if, std::remove_if
#include <limits>   // For std::numeric_limits

ElectionService* ElectionService::instance = nullptr;


ElectionService::ElectionService() {

}


ElectionService::~ElectionService() {

}

// Singleton instance accessor
ElectionService& ElectionService::getInstance() {
    if (instance == nullptr) {
        instance = new ElectionService();
        instance->loadElections(); // Load elections after instance is created
    }
    return *instance;
}

// Loads elections from the JSON file specified by electionsFilePath.
void ElectionService::loadElections() {
    elections.clear();
    // Use DataManager to load the raw JSON data.
    json jsonData = DataManager::getInstance().loadData(electionsFilePath);

    if (jsonData.is_array()) {
        for (const auto& electionJson : jsonData) {
            try {
                // Election::fromJson is a static method that constructs an Election object.
                // Store std::shared_ptr<Election> in the service.
                elections.push_back(std::make_shared<Election>(Election::fromJson(electionJson)));
            } catch (const json::parse_error& e) {
                std::cerr << "Error parsing election JSON: " << e.what() << " at offset " << e.byte << std::endl;
            } catch (const json::type_error& e) {
                std::cerr << "Error in JSON structure for election: " << e.what() << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Generic error loading election: " << e.what() << std::endl;
            }
        }
        std::cout << "Loaded " << elections.size() << " elections from " << electionsFilePath << std::endl;
    } else {
        std::cout << "No elections data found or data is not an array in " << electionsFilePath << ". Initializing empty list." << std::endl;
        // elections remains empty, which is an acceptable state (no elections yet).
    }
}

// Saves all current elections to the JSON file.
void ElectionService::saveElections() const {
    json electionsJsonArray = json::array();
    for (const auto& electionPtr : elections) {
        if (electionPtr) {
            electionsJsonArray.push_back(electionPtr->toJson());
        }
    }
    if (DataManager::getInstance().saveData(electionsFilePath, electionsJsonArray)) {
        std::cout << "Successfully saved " << elections.size() << " elections to " << electionsFilePath << std::endl;
    } else {
        std::cerr << "Failed to save elections to " << electionsFilePath << std::endl;
    }
}


// Generates the next available ID for a new election.
int ElectionService::getNextElectionId() {
    if (elections.empty()) {
        return 1; // Start IDs from 1
    }
    int maxId = 0;
    for (const auto& electionPtr : elections) {
        if (electionPtr && electionPtr->getId() > maxId) {
            maxId = electionPtr->getId();
        }
    }
    return maxId + 1;
}

// Creates a new election, adds it to the list, and returns a shared pointer to it.
std::shared_ptr<Election> ElectionService::createElection(const std::string& name, ElectionLevel level, const std::string& votingSystem) {
    int newId = getNextElectionId();
    auto newElection = std::make_shared<Election>(newId, name, level, votingSystem);
    elections.push_back(newElection);
    std::cout << "Election '" << name << "' (ID: " << newId << ") created with status created." << std::endl;
    // saveElections(); // Persist immediately or rely on saveElectionsOnExit
    return newElection;
}

// Retrieves an election by its ID.
std::shared_ptr<Election> ElectionService::getElection(int electionId) const {
    auto it = std::find_if(elections.begin(), elections.end(),
                           [electionId](const std::shared_ptr<Election>& e) {
                               return e && e->getId() == electionId;
                           });
    if (it != elections.end()) {
        return *it;
    }
    return nullptr;
}

// Returns a constant reference to the list of all elections.
const std::vector<std::shared_ptr<Election>>& ElectionService::getAllElections() const {
    return elections;
}

// Toggles the status of an election (created -> open -> closed).
bool ElectionService::toggleElectionStatus(int electionId) {
    auto election = getElection(electionId);
    if (!election) {
        std::cerr << "Toggle status failed: Election with ID " << electionId << " not found." << std::endl;
        return false;
    }

    ElectionStatus currentStatus = election->getStatus();
    if (currentStatus == ElectionStatus::created) {
        if (election->getCandidateIds().size() < 2) {
            std::cerr << "Cannot open Election ID " << electionId << " ('" << election->getName() << "'): Requires at least 2 candidates. Currently has " << election->getCandidateIds().size() << "." << std::endl;
            return false;
        }
        election->setStatus(ElectionStatus::open);
        std::cout << "Election ID " << electionId << " ('" << election->getName() << "') is now open." << std::endl;
    } else if (currentStatus == ElectionStatus::open) {
        election->setStatus(ElectionStatus::closed);
        std::cout << "Election ID " << electionId << " ('" << election->getName() << "') is now closed." << std::endl;

        std::cout << "(Vote tallying for election ID " << electionId << " would occur now - placeholder)." << std::endl;
    } else if (currentStatus == ElectionStatus::closed) {
        std::cerr << "Election ID " << electionId << " ('" << election->getName() << "') is already closed. No further status change possible." << std::endl;
        return false;
    }

    return true;
}

// Adds a candidate to an election if the election is in created state.
bool ElectionService::addCandidateToElection(int electionId, int candidateId) {
    auto election = getElection(electionId);
    if (!election) {
        std::cerr << "Add candidate failed: Election with ID " << electionId << " not found." << std::endl;
        return false;
    }

    if (election->getStatus() != ElectionStatus::created) {
        std::cerr << "Add candidate failed: Candidates can only be added to an election in 'created' state. Election ID " << electionId << " is currently '" << Election::electionStatusToString(election->getStatus()) << "'." << std::endl;
        return false;
    }

    // Verify candidate exists via CandidateService
    if (!CandidateService::getInstance().getCandidate(candidateId)) {
        std::cerr << "Add candidate failed: Candidate with ID " << candidateId << " does not exist." << std::endl;
        return false;
    }

    if (election->hasCandidate(candidateId)) {
        std::cout << "Info: Candidate ID " << candidateId << " is already assigned to Election ID " << electionId << ". No action taken." << std::endl;
        return true; // Or false, depending on desired behavior for duplicates
    }

    election->addCandidateId(candidateId);
    std::cout << "Candidate ID " << candidateId << " successfully added to Election ID " << electionId << " ('" << election->getName() << "')." << std::endl;

    return true;
}

bool ElectionService::removeCandidateFromElection(int electionId, int candidateId) {
    auto election = getElection(electionId);
    if (!election) {
        std::cerr << "Remove candidate failed: Election with ID " << electionId << " not found." << std::endl;
        return false;
    }

    if (election->getStatus() != ElectionStatus::created) {
        std::cerr << "Remove candidate failed: Candidates can only be removed from an election in 'created' state. Election ID " << electionId << " is currently '" << Election::electionStatusToString(election->getStatus()) << "'." << std::endl;
        return false;
    }

    if (!election->hasCandidate(candidateId)) {
        std::cerr << "Remove candidate failed: Candidate ID " << candidateId << " is not assigned to Election ID " << electionId << "." << std::endl;
        return false;
    }

    election->removeCandidateId(candidateId);
    std::cout << "Candidate ID " << candidateId << " successfully removed from Election ID " << electionId << " ('" << election->getName() << "')." << std::endl;
    // saveElections(); // Persist immediately or rely on saveElectionsOnExit
    return true;
}

// void ElectionService::createElection() {
//     Meniu::clearScreen();
//     std::cout << "===== Create New Election =====\n\n";
//
//     std::cout << "Enter election title: ";
//     std::string title;
//     std::getline(std::cin, title);
//
//     std::cout << "Enter election description: ";
//     std::string description;
//     std::getline(std::cin, description);
//
//     std::cout << "Enter start date (YYYY-MM-DD): ";
//     std::string startDate;
//     std::getline(std::cin, startDate);
//
//     std::cout << "Enter end date (YYYY-MM-DD): ";
//     std::string endDate;
//     std::getline(std::cin, endDate);
//
//     std::cout << "\nElection creation functionality coming soon...\n";
//
//     Meniu::pauseScreen();
// }
//
// void ElectionService::manageExistingElections() {
//     Meniu::clearScreen();
//     std::cout << "===== Manage Existing Elections =====\n\n";
//
//     std::cout << "No elections found.\n\n";
//
//     std::cout << "1. Edit Election\n";
//     std::cout << "2. Delete Election\n";
//     std::cout << "3. Toggle Election Status\n";
//     std::cout << "0. Back\n\n";
//
//     std::cout << "Enter your choice: ";
//     std::string choice;
//     std::getline(std::cin, choice);
//
//     std::cout << "\nElection management functionality coming soon...\n";
//
//     Meniu::pauseScreen();
// }
//
// void ElectionService::viewElectionResults() {
//     Meniu::clearScreen();
//     std::cout << "===== Election Results =====\n\n";
//
//     std::cout << "No election results available.\n\n";
//
//     std::cout << "1. View Results by Election\n";
//     std::cout << "2. View Results by Candidate\n";
//     std::cout << "3. Export Results\n";
//     std::cout << "0. Back\n\n";
//
//     std::cout << "Enter your choice: ";
//     std::string choice;
//     std::getline(std::cin, choice);
//
//     std::cout << "\nResults viewing functionality coming soon...\n";
//
//     Meniu::pauseScreen();
// }
//
// void ElectionService::voteInElection() {
//     Meniu::clearScreen();
//     std::cout << "===== Vote in Election =====\n\n";
//
//     std::cout << "No active elections available.\n\n";
//
//     std::cout << "1. Select Election\n";
//     std::cout << "2. View Election Details\n";
//     std::cout << "0. Back\n\n";
//
//     std::cout << "Enter your choice: ";
//     std::string choice;
//     std::getline(std::cin, choice);
//
//     std::cout << "\nVoting functionality coming soon...\n";
//
//     Meniu::pauseScreen();
// }