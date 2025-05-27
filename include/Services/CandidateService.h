#ifndef CANDIDATE_SERVICE_H
#define CANDIDATE_SERVICE_H

#include <vector>
#include <string>
#include <memory>
#include "../Models/Candidate.h"
#include "../Utils/DataManager.h"

class CandidateService {
    std::vector<std::shared_ptr<Candidate>> candidates;
    std::string dataFile = "data/candidates.json";
    int nextId;

    static CandidateService* instance;

    CandidateService();
    ~CandidateService() = default;

    CandidateService(const CandidateService&) = delete;
    CandidateService& operator=(const CandidateService&) = delete;

    void loadCandidates();
    int generateNextId();

public:
    static CandidateService& getInstance();
    void saveCandidates() const;

    bool addCandidate(const std::string& name, const std::string& politicalParty, const std::string& description);
    bool assignCandidateToElection(int candidateId, int electionId);
    std::shared_ptr<Candidate> getCandidate(int id) const;
    std::vector<std::shared_ptr<Candidate>> getAllCandidates() const;
    std::vector<std::shared_ptr<Candidate>> getCandidatesForElection(int electionId) const;
};

#endif