#include <utility>
#include "../../include/Models/Candidate.h"

Candidate::Candidate(const json& data) {
    id = data.value("id", 0); // Use .value for safer parsing with default
    name = data.value("name", "");
    party = data.value("party", "");
    votes = data.value("votes", 0);
}

json Candidate::toJson() const {
    return {
        {"id", id},
        {"name", name},
        {"party", party},
        {"votes", votes}
    };
}

