#include "../../include/Models/Candidate.h"
#include <limits> // Required for std::numeric_limits
#include <iomanip> // For std::quoted (potentially if names have spaces)

// Default constructor
Candidate::Candidate() : id(0), electionId(-1), votes(0) {}

// Parameterized constructor
Candidate::Candidate(int id, std::string name, int electionId, std::string politicalParty, std::string description)
    : id(id), name(std::move(name)), electionId(electionId), politicalParty(std::move(politicalParty)), description(std::move(description)), votes(0) {}

// JSON-based constructor (delegates to fromJson)
Candidate::Candidate(const json& data) : id(0), electionId(-1), votes(0) {
    fromJson(data);
}

// Copy constructor
Candidate::Candidate(const Candidate& other)
    : id(other.id), name(other.name), electionId(other.electionId), politicalParty(other.politicalParty), description(other.description), votes(other.votes) {}

// Assignment operator
Candidate& Candidate::operator=(const Candidate& other) {
    if (this == &other) {
        return *this; // Handle self-assignment
    }
    id = other.id;
    name = other.name;
    electionId = other.electionId;
    politicalParty = other.politicalParty;
    description = other.description;
    votes = other.votes;
    return *this;
}

// Equality operator
bool Candidate::operator==(const Candidate& other) const {
    return id == other.id &&
           name == other.name &&
           electionId == other.electionId &&
           politicalParty == other.politicalParty &&
           description == other.description &&
           votes == other.votes;
}

// Getters
int Candidate::getId() const { return id; }
const std::string& Candidate::getName() const { return name; }
int Candidate::getElectionId() const { return electionId; }
const std::string& Candidate::getPoliticalParty() const { return politicalParty; }
const std::string& Candidate::getDescription() const { return description; }
int Candidate::getVotes() const { return votes; }

// Setters
void Candidate::setId(int newId) { id = newId; }
void Candidate::setName(const std::string& newName) { name = newName; }
void Candidate::setElectionId(int newElectionId) { electionId = newElectionId; }
void Candidate::setPoliticalParty(const std::string& newParty) { politicalParty = newParty; }
void Candidate::setDescription(const std::string& newDescription) { description = newDescription; }
void Candidate::setVotes(int newVotes) { votes = newVotes; }

// Serialization
json Candidate::toJson() const {
    json data;
    data["id"] = id;
    data["name"] = name;
    if (electionId == -1) {
        data["electionId"] = nullptr;
    } else {
        data["electionId"] = electionId;
    }
    data["politicalParty"] = politicalParty;
    data["description"] = description;
    data["votes"] = votes;
    return data;
}

// Deserialization
void Candidate::fromJson(const json& data) {
    if (data.contains("id") && data["id"].is_number_integer()) {
        id = data["id"].get<int>();
    }
    if (data.contains("name") && data["name"].is_string()) {
        name = data["name"].get<std::string>();
    }
    if (data.contains("electionId")) {
        if (data["electionId"].is_null()) {
            electionId = -1;
        } else if (data["electionId"].is_number_integer()) {
            electionId = data["electionId"].get<int>();
        }
    } else {
        electionId = -1;
    }
    if (data.contains("politicalParty") && data["politicalParty"].is_string()) {
        politicalParty = data["politicalParty"].get<std::string>();
    }
    if (data.contains("description") && data["description"].is_string()) {
        description = data["description"].get<std::string>();
    }
    if (data.contains("votes") && data["votes"].is_number_integer()) {
        votes = data["votes"].get<int>();
    }
}

// Virtual print helper
void Candidate::print(std::ostream& os) const {
    os << "Candidate ID: " << id << "\\n"
       << "Name: " << name << "\\n";
    os << "Election ID: ";
    if (electionId == -1) {
        os << "N/A";
    } else {
        os << electionId;
    }
    os << "\\n"
       << "Political Party: " << politicalParty << "\\n"
       << "Description: " << description << "\\n"
       << "Votes: " << votes;
}

// Virtual read helper
void Candidate::read(std::istream& is) {
    std::string tempName, tempParty, tempDescription;

    std::cout << "Enter Candidate Name (or press Enter to cancel): ";
    std::getline(is, tempName);
    if (tempName.empty() && is.eof()) { is.clear(); throw UserInputCancelledException(); } // Handle EOF with empty line
    if (tempName.empty()) throw UserInputCancelledException();
    name = tempName;

    std::cout << "Enter Political Party (or press Enter to cancel): ";
    std::getline(is, tempParty);
    if (tempParty.empty() && is.eof()) { is.clear(); throw UserInputCancelledException(); }
    if (tempParty.empty()) throw UserInputCancelledException();
    politicalParty = tempParty;

    std::cout << "Enter Description (or press Enter to cancel): ";
    std::getline(is, tempDescription);
    if (tempDescription.empty() && is.eof()) { is.clear(); throw UserInputCancelledException(); }
    if (tempDescription.empty()) throw UserInputCancelledException(); // Allow empty description if intended or enforce non-empty
    description = tempDescription;
    
}

// Non-member operator!=
bool operator!=(const Candidate& lhs, const Candidate& rhs) {
    return !(lhs == rhs);
}

// Friend stream operators
std::ostream& operator<<(std::ostream& os, const Candidate& candidate) {
    candidate.print(os);
    return os;
}

std::istream& operator>>(std::istream& is, Candidate& candidate) {
    candidate.read(is);
    return is;
}

