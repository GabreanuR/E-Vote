#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Candidate final {
    int id;
    std::string name;
    int electionId;
    std::string politicalParty;
    std::string description;
    int votes;

public:
    Candidate();

    Candidate(int id, std::string name, int electionId, std::string politicalParty, std::string description);

    explicit Candidate(const json &data);

    Candidate(const Candidate &other) = default;

    ~Candidate() = default;

    Candidate &operator=(const Candidate &other);

    bool operator==(const Candidate &other) const;

    [[nodiscard]] int getId() const;

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] int getElectionId() const;

    [[nodiscard]] const std::string &getPoliticalParty() const;

    [[nodiscard]] const std::string &getDescription() const;

    [[nodiscard]] int getVotes() const;

    void setId(int newId);

    void setName(const std::string &newName);

    void setElectionId(int newElectionId);

    void setVotes(int newVotes);

    [[nodiscard]] json toJson() const;

    void fromJson(const json &data);

    void print(std::ostream &os) const;

    void read(std::istream &is);
};

bool operator!=(const Candidate &lhs, const Candidate &rhs);

std::ostream &operator<<(std::ostream &os, const Candidate &candidate);

std::istream &operator>>(std::istream &is, Candidate &candidate);

#endif
