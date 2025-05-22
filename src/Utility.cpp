#include <iostream>
#include <limits>
#include <functional>
#include "../include/Utility.h"
using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
#ifdef _WIN32
    system("pause");
#else
    system("read -p \"Press any key to continue...\" -n1");
#endif
}

int getValidatedInput(const int min, const int max, const std::function<void()>& reprintMenu) {
    int choice;
    while (true) {
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cin.clear(); // reset flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input! Please enter a number.\n";
        } else if (choice < min || choice > max) {
            cout << "\n'" << choice << "' is not a valid option!\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }

        pauseScreen();
        clearScreen();
        reprintMenu();
    }
}
