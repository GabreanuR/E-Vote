#ifndef MANAGEUSERSMENU_H
#define MANAGEUSERSMENU_H

#include "../Meniu.h"

class ManageUsersMenu final : public Meniu {
public:
    void display() override;
    static void printMenuText();
};

#endif
