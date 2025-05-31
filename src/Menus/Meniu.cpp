#include <iostream>
#include <sstream>
#include "Menus/Meniu.h"

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

int Meniu::getValidatedInput(const int min, const int max, const std::function<void()> &reprintMenu) {
    std::string input;
    int choice;

    while (true) {
        std::cout << "Enter your choice: ";
        std::getline(std::cin, input);

        if (std::stringstream ss(input); !(ss >> choice) || (ss.peek() != EOF)) {
            std::cout << "\nInvalid input! Please enter a number.\n";
        } else if (choice < min || choice > max) {
            std::cout << "\n'" << choice << "' is not a valid option!\n";
        } else {
            return choice;
        }

        pauseScreen();
        clearScreen();
        reprintMenu();
    }
}
