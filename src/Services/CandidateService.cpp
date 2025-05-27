#include "../../include/Services/CandidateService.h"
#include "../../include/Services/ElectionService.h"
#include <fstream>
#include <iostream>

CandidateService* CandidateService::instance = nullptr;

CandidateService::CandidateService() : nextId(1) {
    loadCandidates();
}

CandidateService& CandidateService::getInstance() {
    if (instance == nullptr) {
        instance = new CandidateService();
    }
    return *instance;
}

void CandidateService::loadCandidates() {
    json data = DataManager::loadData(dataFile);
    if (data.is_array()) {
        int maxId = 0;
        for (const auto& item : data) {
            try {
                auto candidate = std::make_shared<Candidate>(item);
                candidates.push_back(candidate);
                if (candidate->getId() > maxId) {
                    maxId = candidate->getId();
                }
            } catch (const json::exception& e) {
                std::cerr << "Error parsing candidate item: " << e.what() << "\\nItem: " << item.dump(4) << std::endl;
            }
        }
        nextId = maxId + 1;
    } else {
        std::cout << "No candidate data found or data is invalid in " << dataFile << ". Starting with an empty candidate list." << std::endl;
        nextId = 1;
    }
}

void CandidateService::saveCandidates() const {
    json data = json::array();
    for (const auto& candidate : candidates) {
        data.push_back(candidate->toJson());
    }
    DataManager::saveData(dataFile, data);
}

int CandidateService::generateNextId() {
    return nextId++;
}


bool CandidateService::addCandidate(const std::string& name, const std::string& politicalParty, const std::string& description) {


    int newId = generateNextId();

    auto newCandidate = std::make_shared<Candidate>(newId, name, -1, politicalParty, description);
    candidates.push_back(newCandidate);

    std::cout << "Candidate profile '" << name << "' created with ID " << newId << ". Not yet assigned to an election.\\n";
    return true;
}

bool CandidateService::assignCandidateToElection(int candidateId, int electionId) {
    auto candidate = getCandidate(candidateId);
    if (!candidate) {
        std::cerr << "Error: Candidate with ID " << candidateId << " not found.\\n";
        return false;
    }

    if (candidate->getElectionId() != -1) {
        std::cerr << "Error: Candidate '" << candidate->getName() << "' (ID: " << candidateId 
                  << ") is already assigned to Election ID: " << candidate->getElectionId() 
                  << ". Cannot reassign.\\n";
        return false;
    }
    
    if (electionId == -1) {
        std::cerr << "Error: Cannot assign candidate to invalid Election ID -1 (this ID is for unassigned candidates).\n";
        return false;
    }

    if (!ElectionService::getInstance().addCandidateToElection(electionId, candidateId)) {
        std::cerr << "CandidateService: Failed to update election (ID: " << electionId << ") with candidate (ID: " << candidateId << "). Assignment aborted.\n";
        return false; 
    }

    candidate->setElectionId(electionId);
    std::cout << "CandidateService: Candidate '" << candidate->getName() << "' (ID: " << candidateId 
              << ") successfully assigned to Election ID: " << electionId << " and election record updated.\\n";
    return true;
}

std::shared_ptr<Candidate> CandidateService::getCandidate(int id) const {
    for (const auto& candidate : candidates) {
        if (candidate->getId() == id) {
            return candidate;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Candidate>> CandidateService::getAllCandidates() const {
    return candidates;
}

std::vector<std::shared_ptr<Candidate>> CandidateService::getCandidatesForElection(int electionId) const {
    std::vector<std::shared_ptr<Candidate>> electionCandidates;
    if (electionId == -1) {
        return electionCandidates;
    }
    for (const auto& candidate : candidates) {
        if (candidate->getElectionId() == electionId) {
            electionCandidates.push_back(candidate);
        }
    }
    return electionCandidates;
}
