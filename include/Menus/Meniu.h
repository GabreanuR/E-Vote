#ifndef MENIU_H
#define MENIU_H

#include <functional>

class Meniu {
public:
    virtual void display() = 0;
    virtual ~Meniu() = default;

    // Utility methods
    static void clearScreen();
    static void pauseScreen();

protected:
    static int getValidatedInput(int min, int max, const std::function<void()>& reprintMenu);
};

#endif
