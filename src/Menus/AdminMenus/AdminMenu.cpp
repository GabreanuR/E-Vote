#include <iostream>
#include "../include/Menus/AdminMenus/AdminMenu.h"
#include "../include/Menus/AdminMenus/ManageElectionsMenu.h"
#include "../include/Menus/AdminMenus/ManageUsersMenu.h"
#include "../include/Menus/AdminMenus/ManageLocationsMenu.h"
#include "../include/Models/User.h"

void AdminMenu::printAdminMenuText() {
    std::cout << "==================================================\n"
            << "Admin Dashboard\n"
            << "==================================================\n"
            << "1. Manage Elections\n"
            << "2. Manage Users\n"
            << "3. Manage Locations\n"
            << "4. Logout\n";
}

void AdminMenu::display() {
    while (true) {
        clearScreen();
        printAdminMenuText();

        switch (getValidatedInput(1, 4, printAdminMenuText)) {
            case 1: {
                std::cout << "\nManaging Elections...\n\n";
                pauseScreen();
                ManageElectionsMenu().display();
                break;
            }
            case 2: {
                std::cout << "\nManaging Users...\n\n";
                pauseScreen();
                ManageUsersMenu().display();
                break;
            }
            case 3: {
                std::cout << "\nManaging Locations...\n\n";
                pauseScreen();
                ManageLocationsMenu().display();
                break;
            }
            case 4: {
                std::cout << "\nLogging out!\n\n";
                pauseScreen();
                return;
            }
            default: ;
        }
    }
}
