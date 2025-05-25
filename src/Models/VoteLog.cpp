#include "../../include/Models/VoteLog.h"

VoteLog::VoteLog(const json& data) {
    userId = data["userId"];
    electionId = data["electionId"];
    candidateId = data["candidateId"];
}

json VoteLog::toJson() const {
    return {
        {"userId", userId},
        {"electionId", electionId},
        {"candidateId", candidateId}
    };
}

std::vector<VoteLog> VoteLog::loadAll() {
    // Implementation will be added later
    return {};
}

void VoteLog::save(const VoteLog& vote) {
    // Implementation will be added later
}

void VoteLog::updateVote(int userId, int electionId, int newCandidateId) {
    // Implementation will be added later
}
