#ifndef MANAGEELECTIONSMENU_H
#define MANAGEELECTIONSMENU_H

#include "../Meniu.h"

class ManageElectionsMenu final : public Meniu {
public:
    void display() override;

private:
    static void printMenuText();

    static void createElection();

    static void toggleElectionStatus();

    static void viewElections();
};

#endif
