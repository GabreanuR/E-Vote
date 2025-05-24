#include <iostream>
#include "../include/Menus/AdminMenus/AdminMenu.h"
#include "../include/Menus/AdminMenus/CreateElectionMenu.h"
#include "../include/Menus/AdminMenus/ManageExistingElectionsMenu.h"
#include "../include/Menus/AdminMenus/ManageVotersMenu.h"
#include "../include/Menus/AdminMenus/ViewingElectionResultsMenu.h"

void AdminMenu::printAdminMenuText() {
    cout << "==================================================\n";
    cout << "Admin Dashboard\n";
    cout << "==================================================\n";
    cout << "1. Create New Election\n";
    cout << "2. Manage Existing Elections\n";
    cout << "3. Manage Voters\n";
    cout << "4. View Election Results\n";
    cout << "5. Logout\n";
}

void AdminMenu::display() {
    while (true) {
        clearScreen();
        printAdminMenuText();

        switch (getValidatedInput(1, 5, printAdminMenuText)) {
            case 1: {
                cout << "\nCreating New Election...\n\n";
                pauseScreen();
                CreateElectionMenu().display();
                break;
            }
            case 2: {
                cout << "\nManaging Existing Elections...\n\n";
                pauseScreen();
                ManageExistingElectionsMenu().display();
                break;
            }
            case 3: {
                cout << "\nManaging Voters...\n\n";
                pauseScreen();
                ManageVotersMenu().display();
                break;
            }
            case 4: {
                cout << "\nViewing Election Results...\n\n";
                pauseScreen();
                ViewingElectionResultsMenu().display();
                break;
            }
            case 5: {
                cout << "\nLogging out!\n\n";
                pauseScreen();
                return;
            }
            default: ;
        }
    }
}
