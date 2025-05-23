#include "../../include/ElectionModes/Candidate.h"

Candidate::Candidate(int id, const std::string& name)
    : id(id), name(name) {}

int Candidate::getId() const {
    return id;
}

const std::string& Candidate::getName() const {
    return name;
}

bool Candidate::operator==(const Candidate& other) const {
    return id == other.id;
}