#include <iostream>
#include "../../include/Menus/AdminMenu.h"
#include "../../include/Admin.h"

void AdminMenu::printAdminMenuText() {
    cout << "==================================================\n";
    cout << "Admin Dashboard\n";
    cout << "==================================================\n";
    cout << "1. Create Election\n";
    cout << "2. View Elections\n";
    cout << "3. End Election\n";
    cout << "4. Add Voter\n";
    cout << "5. Remove Voter\n";
    cout << "6. Logout\n";
}

void AdminMenu::display() {
    while (true) {
        clearScreen();
        printAdminMenuText();

        switch (getValidatedInput(1, 6, printAdminMenuText)) {
            case 1: {
                cout << "\nCreating Election...\n\n";
                //createElection(elections);
                pauseScreen();
                break;
            }
            case 2: {
                cout << "\nViewing Elections...\n\n";
                //viewElectionsDashboard();
                pauseScreen();
                break;
            }
            case 3: {
                cout << "\nEnding Election...\n\n";
                pauseScreen();
                break;
            }
            case 4: {
                cout << "\nAdding Voter...\n\n";
                pauseScreen();
                break;
            }
            case 5: {
                cout << "\nRemoving Voter...\n\n";
                pauseScreen();
                break;
            }
            case 6: {
                cout << "\nLogging out!\n\n";
                pauseScreen();
                return;
            }
            default: ;
        }
    }
}