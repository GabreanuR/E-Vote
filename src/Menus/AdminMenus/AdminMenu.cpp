#include <iostream>
#include "../include/Menus/AdminMenus/AdminMenu.h"
#include "../include/Menus/AdminMenus/CreateElectionMenu.h"
#include "../include/Menus/AdminMenus/ManageExistingElectionsMenu.h"
#include "../include/Menus/AdminMenus/ManageVotersMenu.h"
#include "../include/Menus/AdminMenus/ViewingElectionResultsMenu.h"
#include "../include/Menus/AdminMenus/ManageLocationsMenu.h"

void AdminMenu::printAdminMenuText() {
    std::cout << "==================================================\n"
              << "Admin Dashboard\n"
              << "==================================================\n"
              << "1. Create New Election\n"
              << "2. Manage Existing Elections\n"
              << "3. Manage Voters\n"
              << "4. View Election Results\n"
              << "5. Manage Locations\n"
              << "6. Logout\n";
}

void AdminMenu::display() {
    while (true) {
        clearScreen();
        printAdminMenuText();

        switch (getValidatedInput(1, 6, printAdminMenuText)) {
            case 1: {
                std::cout << "\nCreating New Election...\n\n";
                pauseScreen();
                CreateElectionMenu().display();
                break;
            }
            case 2: {
                std::cout << "\nManaging Existing Elections...\n\n";
                pauseScreen();
                ManageExistingElectionsMenu().display();
                break;
            }
            case 3: {
                std::cout << "\nManaging Voters...\n\n";
                pauseScreen();
                ManageVotersMenu().display();
                break;
            }
            case 4: {
                std::cout << "\nViewing Election Results...\n\n";
                pauseScreen();
                ViewingElectionResultsMenu().display();
                break;
            }
            case 5: {
                std::cout << "\nManaging Locations...\n\n";
                pauseScreen();
                ManageLocationsMenu().display();
                break;
            }
            case 6: {
                std::cout << "\nLogging out!\n\n";
                pauseScreen();
                return;
            }
            default: ;
        }
    }
}
