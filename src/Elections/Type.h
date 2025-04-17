#ifndef TYPE_H
#define TYPE_H
#include "Election.h"

using namespace std;

class Majority : public Election {
public:
    using Election::Election;
    void displayDetails() const override {
        std::cout << "[Majority Election] " << title << ": " << description << "\n";
    }
};

class Proportional : public Election {
public:
    using Election::Election;
    void displayDetails() const override {
        std::cout << "[Proportional Election] " << title << ": " << description << "\n";
    }
};

#endif
