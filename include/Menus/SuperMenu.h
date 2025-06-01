#ifndef SUPERMENU_H
#define SUPERMENU_H

#include "Menus/Meniu.h"
#include "Menus/MainMenu.h"
#include <memory>

class SuperMenu final : public Meniu {
    std::unique_ptr<MainMenu> mainMenu;

    static void printSuperMenuOptions();

public:
    SuperMenu();

    void display() override;
};

#endif
