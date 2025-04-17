#ifndef TYPE_H
#define TYPE_H
#include "Election.h"

using namespace std;

class Majority : public Election {
public:
    Majority(const std::string &id, const std::string &title, ElectionLevel level) : Election(id, title, level) {
    }

    void displayDetails() const override {
        std::cout << "Majority Election: " << id << " " << title << "\n";
    }
};

class Proportional : public Election {
public:
    Proportional(const std::string &id, const std::string &title,ElectionLevel level) : Election(id, title, level) {
    }

    void displayDetails() const override {
        std::cout << "Proportional Election: " << id << " " << title << "\n";
    }
};

#endif
