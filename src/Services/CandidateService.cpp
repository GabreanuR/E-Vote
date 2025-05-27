#include "../../include/Services/CandidateService.h"
#include "../../include/Services/ElectionService.h" // For validating electionId in assignCandidateToElection
#include <fstream>
#include <algorithm> // For std::max_element and std::sort (if needed later)
#include <iostream> // For std::cerr

CandidateService* CandidateService::instance = nullptr;

CandidateService::CandidateService() : nextId(1) { // Start ID generation from 1
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
                // Candidate constructor and fromJson now handle std::optional<int> electionId
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
        // If the file doesn't exist, is empty, or not an array, start fresh.
        // nextId is already 1.
        std::cout << "No candidate data found or data is invalid in " << dataFile << ". Starting with an empty candidate list." << std::endl;
        nextId = 1; // Ensure nextId is 1 if file is empty/invalid
    }
}

void CandidateService::saveCandidates() const {
    json data = json::array();
    for (const auto& candidate : candidates) {
        // Candidate::toJson now handles std::optional<int> electionId (writes null if not set)
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

// New method to assign a candidate to an election (only once)
bool CandidateService::assignCandidateToElection(int candidateId, int electionId) {
    auto candidate = getCandidate(candidateId);
    if (!candidate) {
        std::cerr << "Error: Candidate with ID " << candidateId << " not found.\\n";
        return false;
    }

    // Check if already assigned (i.e., electionId is not -1)
    if (candidate->getElectionId() != -1) {
        std::cerr << "Error: Candidate '" << candidate->getName() << "' (ID: " << candidateId 
                  << ") is already assigned to Election ID: " << candidate->getElectionId() 
                  << ". Cannot reassign.\\n";
        return false;
    }
    
    // Ensure the target election ID is not the unassigned marker
    if (electionId == -1) {
        std::cerr << "Error: Cannot assign candidate to invalid Election ID -1.\\n";
        return false;
    }

    // Validate the electionId using ElectionService (stubbed for now)
    if (!ElectionService::getInstance().getElection(electionId)) {
        std::cerr << "Error: Election with ID " << electionId << " not found or ElectionService is stubbed. Cannot assign candidate.\\n";
        // Note: If ElectionService::getElection is a stub that always returns nullptr for testing,
        // this assignment will always fail. This needs a working ElectionService.
        return false;
    }
    
    // Check for uniqueness of candidate name WITHIN the target election before assigning
    auto candidatesInTargetElection = getCandidatesForElection(electionId);
    for (const auto& existingCandInElec : candidatesInTargetElection) {
        if (existingCandInElec->getName() == candidate->getName()) {
            std::cerr << "Error: Another candidate named '" << candidate->getName() 
                      << "' already exists in Election ID " << electionId << ". Cannot assign.\\n";
            return false;
        }
    }

    candidate->setElectionId(electionId); // This sets the std::optional<int>
    // saveCandidates(); // Save on program exit
    std::cout << "Candidate '" << candidate->getName() << "' (ID: " << candidateId 
              << ") successfully assigned to Election ID: " << electionId << ".\\n";
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

// Gets candidates specifically assigned to an election
std::vector<std::shared_ptr<Candidate>> CandidateService::getCandidatesForElection(int electionId) const {
    std::vector<std::shared_ptr<Candidate>> electionCandidates;
    if (electionId == -1) { // Don't return candidates for the unassigned marker unless specifically intended
        return electionCandidates; // Or, could return all unassigned candidates if that becomes a feature.
    }
    for (const auto& candidate : candidates) {
        if (candidate->getElectionId() == electionId) {
            electionCandidates.push_back(candidate);
        }
    }
    return electionCandidates;
}
