#ifndef MANAGELOCATIONSMENU_H
#define MANAGELOCATIONSMENU_H

#include "../Meniu.h"

class ManageLocationsMenu final : public Meniu {
public:
    void display() override;

    static void printMenuText();
};

#endif
