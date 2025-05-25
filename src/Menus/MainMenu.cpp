#include <iostream>
#include "Menus/MainMenu.h"
#include "Menus/AuthMenu.h"
#include "Models/User.h"

void MainMenu::printMainMenuText() {
    std::cout << "==================================================\n";
    std::cout << "Welcome to the E-VOTE System!\n";
    std::cout << "==================================================\n";
    std::cout << "1. Login as Voter\n";
    std::cout << "2. Login as Admin\n";
    std::cout << "3. Exit\n";
}

void MainMenu::display() {
    while (true) {
        clearScreen();
        printMainMenuText();

        switch (getValidatedInput(1, 3, printMainMenuText)) {
            case 1:
                std::cout << "\nAccessing Voter Login\n\n";
                pauseScreen();
                AuthMenu(UserType::voter).display();
                break;
            case 2:
                std::cout << "\nAccessing Admin Login\n\n";
                pauseScreen();
                AuthMenu(UserType::admin).display();
                break;
            case 3:
                std::cout << "\nExiting Program!\n\n";
                pauseScreen();
                return;
            default: ;
        }
    }
}
