#ifndef VOTE_H
#define VOTE_H

using namespace std;

class Vote {
    int electionId;
    int voterId;
    int candidateId;

public:
    Vote(int electionId, int voterId, int candidateId);

    [[nodiscard]] int getElectionId() const;
    [[nodiscard]] int getVoterId() const;
    [[nodiscard]] int getCandidateId() const;

};

#endif
