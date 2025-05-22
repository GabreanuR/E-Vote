#ifndef UTILITY_H
#define UTILITY_H
#include <functional>

void clearScreen();

void pauseScreen();

int getValidatedInput(int min, int max, const std::function<void()>& reprintMenu);

#endif //UTILITY_H
