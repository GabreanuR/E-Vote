#ifndef MAJORITYVOTING_H
#define MAJORITYVOTING_H

#include "VotingStrategy.h"
#include <vector>
#include <string>

class MajorityVoting final : public VotingStrategy {
public:
    void computeResults(const std::vector<Vote>& votes, const std::vector<Candidate>& candidates) override;
    [[nodiscard]] std::string getName() const override;
};

#endif
