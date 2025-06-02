#ifndef MENIU_H
#define MENIU_H

#include <functional>

class Meniu {
public:
    virtual void display() = 0;

    virtual ~Meniu() = default;

    static void clearScreen();

    static void pauseScreen();

    static int getValidatedInput(int min, int max, const std::function<void()> &reprintMenu);
    
    static int getChoice(int min, int max);
};

#endif
