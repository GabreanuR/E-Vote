#include "../../include/ElectionStructure/Vote.h"

Vote::Vote(const int electionId, const int voterId, const int candidateId)
    : electionId(electionId), voterId(voterId), candidateId(candidateId) {}

int Vote::getElectionId() const { return electionId; }
int Vote::getVoterId() const { return voterId; }
int Vote::getCandidateId() const { return candidateId; }