#ifndef MENIU_H
#define MENIU_H

#include <functional>

using namespace std;

class Meniu {
public:
    virtual void display() = 0;
    virtual ~Meniu() = default;
protected:
    static void clearScreen();

    static void pauseScreen();

    static int getValidatedInput(int min, int max, const function<void()>& reprintMenu);
};

#endif
