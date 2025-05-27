#ifndef MANAGEELECTIONSMENU_H
#define MANAGEELECTIONSMENU_H

#include "../Meniu.h"

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
};

#endif
