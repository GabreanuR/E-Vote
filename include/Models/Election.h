#ifndef ELECTION_H
#define ELECTION_H

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "../Utils/Types.h"
#include "VotingStrategy.h"
#include "Candidate.h"

using json = nlohmann::json;

class Election {
private:
    int id;
    std::string title;
    std::string level;         // "national", "local", etc.
    std::string zone;          // "" or "Cluj", etc.
    std::string status;        // "active" / "inactive"
    std::string votingSystem;  // "majority" / "proportional"
    std::string startDate, endDate;
    std::vector<Candidate> candidates;
    std::unique_ptr<VotingStrategy> strategy;

public:
    // Constructor
    Election(int id, const std::string& title, const std::string& level, 
            const std::string& zone, const std::string& votingSystem);
    explicit Election(const json& data);
    
    // Getters
    int getId() const { return id; }
    const std::string& getTitle() const { return title; }
    const std::string& getLevel() const { return level; }
    const std::string& getZone() const { return zone; }
    const std::string& getStatus() const { return status; }
    const std::string& getVotingSystem() const { return votingSystem; }
    const std::string& getStartDate() const { return startDate; }
    const std::string& getEndDate() const { return endDate; }
    const std::vector<Candidate>& getCandidates() const { return candidates; }
    
    // Setters
    void setTitle(const std::string& newTitle) { title = newTitle; }
    void setLevel(const std::string& newLevel) { level = newLevel; }
    void setZone(const std::string& newZone) { zone = newZone; }
    void setStatus(const std::string& newStatus) { status = newStatus; }
    void setVotingSystem(const std::string& newSystem) { votingSystem = newSystem; }
    void setStartDate(const std::string& newDate) { startDate = newDate; }
    void setEndDate(const std::string& newDate) { endDate = newDate; }
    
    // Methods
    void computeResults();
    void addCandidate(const Candidate& candidate);
    void removeCandidate(int candidateId);
    bool isActive() const { return status == "active"; }
    void start();
    void end();
    [[nodiscard]] json toJson() const;
};

#endif
