#include "../../include/Services/ElectionService.h"
#include "../../include/Utils/DataManager.h"
#include "../../include/Services/CandidateService.h"
#include "../../include/Models/Election.h"
#include <iostream>
#include <algorithm>


ElectionService* ElectionService::instance = nullptr;


ElectionService::ElectionService() {

}


ElectionService::~ElectionService() {

}


ElectionService& ElectionService::getInstance() {
    if (instance == nullptr) {
        instance = new ElectionService();
        instance->loadElections();
    }
    return *instance;
}


void ElectionService::loadElections() {
    elections.clear();

    json jsonData = DataManager::getInstance().loadData(electionsFilePath);

    if (jsonData.is_array()) {
        for (const auto& electionJson : jsonData) {
            try {
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
    }
}

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

int ElectionService::getNextElectionId() {
    if (elections.empty()) {
        return 1;
    }
    int maxId = 0;
    for (const auto& electionPtr : elections) {
        if (electionPtr && electionPtr->getId() > maxId) {
            maxId = electionPtr->getId();
        }
    }
    return maxId + 1;
}

std::shared_ptr<Election> ElectionService::createElection(const std::string& name, ElectionLevel level, 
                                                        VotingSystemType votingSystem, int locationId) {
    int newId = getNextElectionId();
    auto newElection = std::make_shared<Election>(newId, name, level, votingSystem, locationId);
    
    elections.push_back(newElection);
    std::cout << "Election '" << name << "' (ID: " << newId << ") created with status created." << std::endl;
    return newElection;
}

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

const std::vector<std::shared_ptr<Election>>& ElectionService::getAllElections() const {
    return elections;
}

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

    if (!CandidateService::getInstance().getCandidate(candidateId)) {
        std::cerr << "Add candidate failed: Candidate with ID " << candidateId << " does not exist." << std::endl;
        return false;
    }

    if (election->hasCandidate(candidateId)) {
        std::cout << "Info: Candidate ID " << candidateId << " is already assigned to Election ID " << electionId << ". No action taken." << std::endl;
        return true;
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
    return true;
}
