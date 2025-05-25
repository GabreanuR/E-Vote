#ifndef VOTERMENU_H
#define VOTERMENU_H

#include "../Meniu.h"

class VoterMenu final : public Meniu {
public:
    void display() override;
    static void printMenuText();
};

#endif
