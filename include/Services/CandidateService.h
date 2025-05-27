#ifndef CANDIDATE_SERVICE_H
#define CANDIDATE_SERVICE_H

#include <vector>
#include <string>
#include <memory>
#include "../Models/Candidate.h"
#include "../Utils/DataManager.h"
#include "../Models/User.h" // For potential future use (e.g. logging who added a candidate)
                           // And for User::updateAdminAccess if candidates grant some form of access (unlikely but for consistency)

class CandidateService {
private:
    std::vector<std::shared_ptr<Candidate>> candidates;
    std::string dataFile = "data/candidates.json";
    int nextId;

    // Singleton instance
    static CandidateService* instance;

    // Private constructor for singleton
    CandidateService();
    ~CandidateService() = default;

    // Private copy constructor and assignment operator to prevent copying
    CandidateService(const CandidateService&) = delete;
    CandidateService& operator=(const CandidateService&) = delete;

    void loadCandidates();
    int generateNextId();

public:
    // Static method to get the singleton instance
    static CandidateService& getInstance();
    void saveCandidates() const;

    // CRUD operations (or similar)
    bool addCandidate(const std::string& name, const std::string& politicalParty, const std::string& description);
    bool assignCandidateToElection(int candidateId, int electionId);
    std::shared_ptr<Candidate> getCandidate(int id) const;
    std::vector<std::shared_ptr<Candidate>> getAllCandidates() const;
    std::vector<std::shared_ptr<Candidate>> getCandidatesForElection(int electionId) const;
    // bool updateCandidate(int id, const Candidate& updatedCandidate); // Optional for now
    // bool deleteCandidate(int id); // Optional for now

    // void incrementVoteCount(int candidateId); // This might be better in an ElectionService or VotingService

};

#endif // CANDIDATE_SERVICE_H 