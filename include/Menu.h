#ifndef MENU_H
#define MENU_H

using namespace std;

class Menu {
public:
    virtual void display() = 0;
    virtual ~Menu() = default;
};

#endif
