#ifndef VOTELOG_H
#define VOTELOG_H

#include <vector>

class VoteLog {
public:
    int userId;
    int electionId;
    int candidateId;

    static std::vector<VoteLog> loadAll();
    static void save(const VoteLog& vote);
    static void updateVote(int userId, int electionId, int newCandidateId);
};

#endif
