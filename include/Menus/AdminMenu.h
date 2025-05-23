#ifndef ADMINMENU_H
#define ADMINMENU_H

#include "Meniu.h"

class AdminMenu final : public Meniu {
public:
    void display() override;

    static void printAdminMenuText();
};

#endif
