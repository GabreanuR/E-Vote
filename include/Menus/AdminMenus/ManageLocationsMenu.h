#ifndef MANAGELOCATIONSMENU_H
#define MANAGELOCATIONSMENU_H

#include "../Meniu.h"

class ManageLocationsMenu final : public Meniu {
public:
    void display() override;

private:
    static void printMenuText();
    void addRegion();
    void addMunicipality();
    void addLocality();
    void addNonGovernment();
    void displayLocations();
};

#endif
