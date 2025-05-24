#include <utility>
#include "../../include/ElectionStructure/Candidate.h"

int Candidate::idCounter = 0;

Candidate::Candidate(string  name) : id(++idCounter), name(move(name)) {}

int Candidate::getId() const {
    return id;
}

const string& Candidate::getName() const {
    return name;
}

bool Candidate::operator==(const Candidate& other) const {
    return id == other.id;
}