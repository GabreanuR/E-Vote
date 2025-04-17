#ifndef ELECTION_H
#define ELECTION_H

#include <string>
#include <utility>
#include <vector>
#include "Position.h"

class Election {
protected:
    std::string id;
    std::string title;
    std::string description;
    std::vector<Position> positions;
public:
    Election(std::string id, std::string title, std::string description)
        : id(std::move(id)), title(std::move(title)), description(std::move(description)) {}
    virtual void displayDetails() const = 0;
    virtual ~Election() = default;
};

enum class ElectionLevel {
    National, Regional, Municipal, Local, Corporate
};

#endif
