#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

class Position {
    std::string name;
    std::vector<std::string> candidates;

public:
    Position(std::string name, std::vector<std::string> candidates)
        : name(std::move(name)), candidates(std::move(candidates)) {
    }

    [[nodiscard]] const std::string &getName() const { return name; }
    [[nodiscard]] const std::vector<std::string> &getCandidates() const { return candidates; }
};

#endif
