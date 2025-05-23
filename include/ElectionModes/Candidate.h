#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>

using namespace std;

class Candidate {
    int id;
    string name;

public:
    Candidate(int id, const string& name);

    int getId() const;
    const string& getName() const;

    bool operator==(const Candidate& other) const;
};

#endif
