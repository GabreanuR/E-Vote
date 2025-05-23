#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Meniu.h"

class MainMenu final : public Meniu {
public:
    void display() override;

    static void printMainMenuText();
};

#endif
