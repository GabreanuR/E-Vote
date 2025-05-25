#ifndef MANAGEELECTIONSMENU_H
#define MANAGEELECTIONSMENU_H

#include "../Meniu.h"

class ManageElectionsMenu final : public Meniu {
public:
    void display() override;
    static void printMenuText();
};

#endif 