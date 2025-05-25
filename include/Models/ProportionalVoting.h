#ifndef PROPORTIONALVOTING_H
#define PROPORTIONALVOTING_H

#include "VotingStrategy.h"

class ProportionalVoting final : public VotingStrategy {
public:
    void computeResults(Election& election) override;
};

#endif
