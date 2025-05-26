#ifndef MANAGE_CANDIDATES_MENU_H
#define MANAGE_CANDIDATES_MENU_H

#include "../Meniu.h"

class ManageCandidatesMenu final : public Meniu {
public:
    void display() override;

private:
    static void printMenuText();

    static void createCandidate();

    static void addCandidateToElection();

    static void viewCandidates();
};

#endif
