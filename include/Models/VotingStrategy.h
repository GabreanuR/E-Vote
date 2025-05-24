#ifndef VOTINGSTRATEGY_H
#define VOTINGSTRATEGY_H

#include <string>
#include "Election.h"

using namespace std;

class VotingStrategy {
public:
    virtual void computeResults(Election& election) = 0;
    virtual ~VotingStrategy() = default;
};

#endif
