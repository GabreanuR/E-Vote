#include "ui.h"
#include <cstdlib>

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
#ifdef _WIN32
    system("pause");
#else
    system("read -p \"Press any key to continue...\" -n1");
#endif
}