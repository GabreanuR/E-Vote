#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Candidate {
public:
    int id;
    std::string name;
    std::string party;
    int votes;

    // Constructors
    Candidate() : id(0), votes(0) {}
    explicit Candidate(const json& data);

    // Serialization
    [[nodiscard]] json toJson() const;
};

#endif
