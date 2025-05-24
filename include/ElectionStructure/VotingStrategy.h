#ifndef VOTINGSTRATEGY_H
#define VOTINGSTRATEGY_H

#include <vector>
#include <string>
#include "Vote.h"
#include "Candidate.h"

using namespace std;

class VotingStrategy {
public:
    virtual void computeResults(const std::vector<Vote>& votes, const std::vector<Candidate>& candidates) = 0;
    [[nodiscard]] virtual std::string getName() const = 0;

    virtual ~VotingStrategy() = default;
};

#endif
