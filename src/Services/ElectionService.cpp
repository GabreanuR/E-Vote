#include "../include/Services/ElectionService.h"
#include "../include/Utils/DataManager.h"
#include "../include/Services/CandidateService.h"
#include "../include/Models/Election.h"
#include "../include/Models/User.h"
#include <iostream>
#include <algorithm>
#include <vector>

ElectionService::ElectionService() {
    loadElectionsFromDataManager();
}

ElectionService &ElectionService::getInstance() {
    static ElectionService serviceInstance;
    return serviceInstance;
}

void ElectionService::loadElectionsFromDataManager() {
    electionsCache.clear();

    if (json jsonData = DataManager::loadData(electionsFilePath); jsonData.is_array()) {
        for (const auto &electionJson: jsonData) {
            try {
                electionsCache.push_back(std::make_shared<Election>(Election::fromJson(electionJson)));
            } catch (const json::parse_error &e) {
                std::cerr << "Error parsing election JSON: " << e.what() << " at offset " << e.byte << std::endl;
            } catch (const json::type_error &e) {
                std::cerr << "Error in JSON structure for election: " << e.what() << std::endl;
            } catch (const std::exception &e) {
                std::cerr << "Generic error loading election: " << e.what() << std::endl;
            }
        }
        std::cout << "Loaded " << electionsCache.size() << " elections from " << electionsFilePath << std::endl;
    } else {
        std::cout << "No elections data found or data is not an array in " << electionsFilePath <<
                ". Initializing empty list." << std::endl;
    }
}

void ElectionService::saveElectionsToDataManager() const {
    json electionsJsonArray = json::array();
    for (const auto &electionPtr: electionsCache) {
        if (electionPtr) {
            electionsJsonArray.push_back(electionPtr->toJson());
        }
    }
    if (DataManager::saveData(electionsFilePath, electionsJsonArray)) {
        std::cout << "Successfully saved " << electionsCache.size() << " elections to " << electionsFilePath <<
                std::endl;
    } else {
        std::cerr << "Failed to save elections to " << electionsFilePath << std::endl;
    }
}

int ElectionService::getNextElectionId() const {
    if (electionsCache.empty()) {
        return 1;
    }
    int maxId = 0;
    for (const auto &electionPtr: electionsCache) {
        if (electionPtr && electionPtr->getId() > maxId) {
            maxId = electionPtr->getId();
        }
    }
    return maxId + 1;
}

std::shared_ptr<Election> ElectionService::createElection(const std::string &name, ElectionLevel level,
                                                          VotingSystemType votingSystem,
                                                          int locationId) {
    int newId = getNextElectionId();
    auto newElection = std::make_shared<Election>(newId, name, level, votingSystem, locationId);

    electionsCache.push_back(newElection);
    std::cout << "Election '" << name << "' created with ID " << newId << std::endl;
    return newElection;
}

std::shared_ptr<Election> ElectionService::getElection(int electionId) const {
    const auto it = std::ranges::find_if(electionsCache,
                                         [electionId](const std::shared_ptr<Election> &e) {
                                             return e && e->getId() == electionId;
                                         });
    if (it != electionsCache.end()) {
        return *it;
    }
    return nullptr;
}

const std::vector<std::shared_ptr<Election> > &ElectionService::getAllElections() const {
    return electionsCache;
}

bool ElectionService::toggleElectionStatus(const int electionId) const {
    const auto election = getElection(electionId);
    if (!election) {
        std::cerr << "Toggle status failed: Election with ID " << electionId << " not found." << std::endl;
        return false;
    }

    if (const ElectionStatus currentStatus = election->getStatus(); currentStatus == ElectionStatus::created) {
        if (election->getCandidateIds().size() < 2) {
            std::cerr << "Cannot open Election ID " << electionId << " ('" << election->getName() <<
                    "'): Requires at least 2 candidates. Currently has " << election->getCandidateIds().size() << "." <<
                    std::endl;
            return false;
        }
        election->setStatus(ElectionStatus::open);
        std::cout << "Election ID " << electionId << " ('" << election->getName() << "') is now open." << std::endl;
    } else if (currentStatus == ElectionStatus::open) {
        election->setStatus(ElectionStatus::closed);
        std::cout << "Election ID " << electionId << " ('" << election->getName() << "') is now closed." << std::endl;
    } else if (currentStatus == ElectionStatus::closed) {
        std::cerr << "Election ID " << electionId << " ('" << election->getName() <<
                "') is already closed. No further status change possible." << std::endl;
        return false;
    }

    return true;
}

bool ElectionService::addCandidateToElection(const int electionId, const int candidateId) const {
    const auto election = getElection(electionId);
    if (!election) {
        std::cerr << "Add candidate failed: Election with ID " << electionId << " not found." << std::endl;
        return false;
    }

    if (election->getStatus() != ElectionStatus::created) {
        std::cerr <<
                "Add candidate failed: Candidates can only be added to an election in 'created' state. Election ID " <<
                electionId << " is currently '" << Election::electionStatusToString(election->getStatus()) << "'." <<
                std::endl;
        return false;
    }

    if (!CandidateService::getInstance().getCandidate(candidateId)) {
        std::cerr << "Add candidate failed: Candidate with ID " << candidateId << " does not exist." << std::endl;
        return false;
    }

    if (election->hasCandidate(candidateId)) {
        std::cout << "Info: Candidate ID " << candidateId << " is already assigned to Election ID " << electionId <<
                ". No action taken." << std::endl;
        return true;
    }

    election->addCandidateId(candidateId);

    return true;
}

std::vector<std::shared_ptr<Election> > ElectionService::getAvailableElectionsForUser(
    const std::shared_ptr<User> &user) const {
    std::vector<std::shared_ptr<Election> > availableElections;
    if (!user) {
        std::cerr << "Error: Cannot get available elections for a null user." << std::endl;
        return availableElections;
    }

    for (const auto &allElections = getAllElections(); const auto &election: allElections) {
        if (election && election->getStatus() == ElectionStatus::open) {
            const bool hasLocationAccess = user->hasAccessTo(election->getElectionLevel(), election->getLocationId());

            if (const bool alreadyVoted = election->hasVoted(user->getId()); hasLocationAccess && !alreadyVoted) {
                availableElections.push_back(election);
            }
        }
    }
    return availableElections;
}

bool ElectionService::castVote(const int electionId, const int candidateId, const std::shared_ptr<User> &user) const {
    if (!user) {
        std::cerr << "Error casting vote: User is null." << std::endl;
        return false;
    }

    const auto election = getElection(electionId);
    if (!election) {
        std::cerr << "Error casting vote: Election with ID " << electionId << " not found." << std::endl;
        return false;
    }

    if (election->getStatus() != ElectionStatus::open) {
        std::cerr << "Error casting vote: Election ID " << electionId << " ('" << election->getName() <<
                "') is not open for voting." << std::endl;
        return false;
    }

    if (!election->hasCandidate(candidateId)) {
        std::cerr << "Error casting vote: Candidate ID " << candidateId << " is not part of election ID " << electionId
                << "." << std::endl;
        return false;
    }

    if (election->hasVoted(user->getId())) {
        std::cerr << "Error casting vote: User " << user->getUsername() << " (ID: " << user->getId()
                << ") has already voted in election ID " << electionId << "." << std::endl;
        return false;
    }

    try {
        election->recordVote(candidateId, user->getId());
        std::cout << "Vote successfully cast by " << user->getUsername() << " for candidate ID " << candidateId
                << " in election '" << election->getName() << "' (ID: " << electionId << ")." << std::endl;
        return true;
    } catch (const std::exception &e) {
        std::cerr << "Error recording vote in election object: " << e.what() << std::endl;
        return false;
    }
}
