#include <iostream>
#include "../include/MainMenu.h"
#include "../include/AuthMenu.h"
#include "../include/UserType.h"

void MainMenu::printMainMenuText() {
    cout << "==================================================\n";
    cout << "Welcome to the E-VOTE System!\n";
    cout << "==================================================\n";
    cout << "1. Login as Voter\n";
    cout << "2. Login as Admin\n";
    cout << "3. Exit\n";
}

void MainMenu::display() {
    while (true) {
        clearScreen();
        printMainMenuText();

        switch (getValidatedInput(1, 3, printMainMenuText)) {
            case 1:
                cout << "\nAccessing Voter Login\n\n";
                pauseScreen();
                AuthMenu(UserType::Voter).display();
                break;
            case 2:
                cout << "\nAccessing Admin Login\n\n";
                pauseScreen();
                AuthMenu(UserType::Admin).display();
                break;
            case 3:
                cout << "\nExiting Program!\n\n";
                pauseScreen();
                return;
            default: ;
        }
    }
}
