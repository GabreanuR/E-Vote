#include <iostream>
#include <sstream>
#include "../include/Meniu.h"

void Meniu::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Meniu::pauseScreen() {
#ifdef _WIN32
    system("pause");
#else
    system("read -p \"Press any key to continue...\" -n1");
#endif
}

int Meniu::getValidatedInput(const int min, const int max, const std::function<void()>& reprintMenu) {
    string input;
    int choice;

    while (true) {
        cout << "Enter your choice: ";
        getline(cin, input);

        if (stringstream ss(input); !(ss >> choice) || (ss.peek() != EOF)) {
            cout << "\nInvalid input! Please enter a number.\n";
        } else if (choice < min || choice > max) {
            cout << "\n'" << choice << "' is not a valid option!\n";
        } else {
            return choice;
        }

        pauseScreen();
        clearScreen();
        reprintMenu();
    }
}