#ifndef MANAGEELECTIONSMENU_H
#define MANAGEELECTIONSMENU_H

#include <memory>
#include "../Meniu.h"
#include "../include/Models/Election.h"

class ManageElectionsMenu final : public Meniu {
public:
    void display() override;

private:
    static void printMenuText();

    static void printToggleElectionStatusMenuText();

    static void createElection();

    static void toggleElectionStatus();

    static void openElection();

    static void closeElection();

    static void viewElections();

    static void displayElectionResults(const std::shared_ptr<Election> &electionPtr);
};

#endif
