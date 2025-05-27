#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>
#include <iostream> // For std::ostream, std::istream
#include <stdexcept> // For std::runtime_error
#include "nlohmann/json.hpp"
#include "../Utils/Types.h" // For UserInputCancelledException

using json = nlohmann::json;

// Forward declaration if needed by other headers, though likely not for Candidate itself
// class Election; 

class Candidate {
private:
    int id;
    std::string name;
    int electionId; // Changed back to int, -1 will mean unassigned
    std::string politicalParty;
    std::string description;
    int votes;

public:
    // Constructors
    Candidate();
    Candidate(int id, std::string name, int electionId, std::string politicalParty, std::string description);
    explicit Candidate(const json& data); // Will call fromJson
    Candidate(const Candidate& other);    // Copy constructor
    virtual ~Candidate() = default;       // Virtual destructor

    // Assignment operator
    Candidate& operator=(const Candidate& other);

    // Equality operators
    bool operator==(const Candidate& other) const;

    // Getters
    [[nodiscard]] int getId() const;
    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] int getElectionId() const; // Return type changed back to int
    [[nodiscard]] const std::string& getPoliticalParty() const;
    [[nodiscard]] const std::string& getDescription() const;
    [[nodiscard]] int getVotes() const;

    // Setters
    void setId(int newId);
    void setName(const std::string& newName);
    void setElectionId(int newElectionId); // Parameter type changed back to int
    void setPoliticalParty(const std::string& newParty);
    void setDescription(const std::string& newDescription);
    void setVotes(int newVotes); // Usually votes are incremented, but a setter might be useful

    // Serialization/Deserialization
    [[nodiscard]] json toJson() const;
    void fromJson(const json& data);

    // Virtual helpers for stream operators
    virtual void print(std::ostream& os) const;
    virtual void read(std::istream& is); // Will handle input and potentially throw UserInputCancelledException
};

// Non-member operator!=
bool operator!=(const Candidate& lhs, const Candidate& rhs);

// Friend stream operators
std::ostream& operator<<(std::ostream& os, const Candidate& candidate);
std::istream& operator>>(std::istream& is, Candidate& candidate);

#endif // CANDIDATE_H
