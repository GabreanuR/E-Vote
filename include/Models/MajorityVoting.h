#ifndef MAJORITYVOTING_H
#define MAJORITYVOTING_H

#include "VotingStrategy.h"

class MajorityVoting final : public VotingStrategy {
public:
    void computeResults(Election& election) override;
};

#endif
