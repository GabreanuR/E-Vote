#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>

using namespace std;

class Candidate {
    static int idCounter;

    int id;
    string name;

public:
    explicit Candidate(string  name);

    [[nodiscard]] int getId() const;
    [[nodiscard]] const string& getName() const;

    bool operator==(const Candidate& other) const;
};

#endif
