#ifndef MANAGELOCATIONSMENU_H
#define MANAGELOCATIONSMENU_H

#include "../Meniu.h"

class ManageLocationsMenu final : public Meniu {
public:
    void display() override;

private:
    static void printMenuText();

    static void addRegion();

    static void addMunicipality();

    static void addLocality();

    static void addNonGovernment();

    static void displayLocations();
};

#endif
