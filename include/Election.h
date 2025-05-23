#ifndef ELECTION_H
#define ELECTION_H

#include <string>
#include <utility>
#include <vector>
#include "Position.h"
#include "ElectionModes/ElectionLevelUtils.h"

class Election {
protected:
    std::string id;
    std::string title;
    ElectionLevel level;
    std::vector<Position> positions;

public:
    Election(std::string id, std::string title, ElectionLevel level)
        : id(std::move(id)), title(std::move(title)), level(level) {
    }

    virtual void displayDetails() const = 0;

    virtual ~Election() = default;

    void addPosition(const Position &pos) {
        positions.push_back(pos);
    }

    // [[nodiscard]] const std::vector<Position> &getPositions() const {
    //     return positions;
    // }
    //
    // [[nodiscard]] ElectionLevel getLevel() const { return level; }

    // std::string getId() const { return id; };
    //
    // std::string getTitle() const { return title; };
};
#endif //ELECTION_H
