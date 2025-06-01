#ifndef CANDIDATE_SERVICE_H
#define CANDIDATE_SERVICE_H

#include <vector>
#include <string>
#include <memory>
#include "../Models/Candidate.h"

class CandidateService {
    std::vector<std::shared_ptr<Candidate> > candidatesCache;
    std::string candidatesFilePath = "data/candidates.json";

    CandidateService();

    ~CandidateService() = default;

    void loadCandidatesFromDataManager();

    [[nodiscard]] int calculateNextCandidateId() const;

public:
    static CandidateService &getInstance();

    CandidateService(const CandidateService &) = delete;

    CandidateService &operator=(const CandidateService &) = delete;

    void saveCandidatesToDataManager() const;

    bool addCandidate(const std::string &name, const std::string &politicalParty, const std::string &description);

    [[nodiscard]] bool assignCandidateToElection(int candidateId, int electionId) const;

    [[nodiscard]] std::shared_ptr<Candidate> getCandidate(int id) const;

    [[nodiscard]] const std::vector<std::shared_ptr<Candidate> > &getAllCandidates() const;

    [[nodiscard]] std::vector<std::shared_ptr<Candidate> > getCandidatesForElection(int electionId) const;
};

#endif
