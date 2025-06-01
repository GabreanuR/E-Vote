#include "../include/Services/CandidateService.h"
#include "../include/Services/ElectionService.h"
#include "../include/Utils/DataManager.h"
#include "../include/Models/Candidate.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

CandidateService::CandidateService() {
    loadCandidatesFromDataManager();
}

CandidateService &CandidateService::getInstance() {
    static CandidateService serviceInstance;
    return serviceInstance;
}

void CandidateService::loadCandidatesFromDataManager() {
    candidatesCache.clear();
    if (json data = DataManager::loadData(candidatesFilePath); data.is_array()) {
        for (const auto &item: data) {
            try {
                auto candidate = std::make_shared<Candidate>(item);
                candidatesCache.push_back(candidate);
            } catch (const json::exception &e) {
                std::cerr << "Error parsing candidate item: " << e.what() << "\nItem: " << item.dump(4) << std::endl;
            }
        }
        std::cout << "Loaded " << candidatesCache.size() << " candidates from " << candidatesFilePath << std::endl;
    } else {
        std::cout << "No candidate data found or data is invalid in " << candidatesFilePath <<
                ". Starting with an empty candidate list." << std::endl;
    }
}

void CandidateService::saveCandidatesToDataManager() const {
    json data = json::array();
    for (const auto &candidate: candidatesCache) {
        if (candidate) {
            data.push_back(candidate->toJson());
        }
    }
    DataManager::saveData(candidatesFilePath, data);
    std::cout << "Successfully saved " << candidatesCache.size() << " candidates to " << candidatesFilePath <<
            std::endl;
}

int CandidateService::calculateNextCandidateId() const {
    if (candidatesCache.empty()) {
        return 1;
    }
    int maxId = 0;
    for (const auto &candidate: candidatesCache) {
        if (candidate && candidate->getId() > maxId) {
            maxId = candidate->getId();
        }
    }
    return maxId + 1;
}

bool CandidateService::addCandidate(const std::string &name, const std::string &politicalParty,
                                    const std::string &description) {
    int newId = calculateNextCandidateId();

    const auto newCandidate = std::make_shared<Candidate>(newId, name, -1, politicalParty, description);
    candidatesCache.push_back(newCandidate);

    std::cout << "Candidate profile '" << name << "' created with ID " << newId <<
            ". Not yet assigned to an election.\n";
    return true;
}

bool CandidateService::assignCandidateToElection(const int candidateId, const int electionId) const {
    const auto candidate = getCandidate(candidateId);
    if (!candidate) {
        std::cerr << "Error: Candidate with ID " << candidateId << " not found.\n";
        return false;
    }

    if (candidate->getElectionId() != -1 && candidate->getElectionId() != 0) {
        std::cerr << "Error: Candidate '" << candidate->getName() << "' (ID: " << candidateId
                << ") is already assigned to Election ID: " << candidate->getElectionId()
                << ". Cannot reassign directly via this method. Unassign first or use a dedicated update method.\n";
        return false;
    }

    if (electionId == -1) {
        std::cerr <<
                "Error: Cannot assign candidate to Election ID -1 (this ID is conventionally for unassigned candidates).\n";
        return false;
    }

    if (!ElectionService::getInstance().addCandidateToElection(electionId, candidateId)) {
        std::cerr << "CandidateService: ElectionService failed to add candidate (ID: " << candidateId <<
                ") to election (ID: " << electionId << "). Assignment aborted.\n";
        return false;
    }

    candidate->setElectionId(electionId);
    return true;
}

std::shared_ptr<Candidate> CandidateService::getCandidate(const int id) const {
    for (const auto &candidate: candidatesCache) {
        if (candidate && candidate->getId() == id) {
            return candidate;
        }
    }
    return nullptr;
}

const std::vector<std::shared_ptr<Candidate> > &CandidateService::getAllCandidates() const {
    return candidatesCache;
}

std::vector<std::shared_ptr<Candidate> > CandidateService::getCandidatesForElection(const int electionId) const {
    std::vector<std::shared_ptr<Candidate> > electionCandidates;
    if (electionId == -1) {
        return electionCandidates;
    }
    for (const auto &candidate: candidatesCache) {
        if (candidate && candidate->getElectionId() == electionId) {
            electionCandidates.push_back(candidate);
        }
    }
    return electionCandidates;
}
