#ifndef VOTINGSTRATEGY_H
#define VOTINGSTRATEGY_H

#include <string>

class Election;  // Forward declaration

class VotingStrategy {
public:
    virtual void computeResults(Election& election) = 0;
    virtual ~VotingStrategy() = default;
};

#endif
