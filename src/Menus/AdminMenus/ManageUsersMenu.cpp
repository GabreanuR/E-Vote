#include <iostream>
#include "../include/Menus/AdminMenus/ManageUsersMenu.h"
#include "../include/Services/UserService.h"

void ManageUsersMenu::printMenuText() {
    std::cout << "==================================================\n"
            << "Manage Users\n"
            << "==================================================\n"
            << "1. View All Users\n"
            << "2. Add New User\n"
            << "3. Enable/Disable User\n"
            << "4. Delete User\n"
            << "5. Manage User Access\n"
            << "6. Back\n";
}

void ManageUsersMenu::display() {
    while (true) {
        clearScreen();
        printMenuText();

        switch (getValidatedInput(1, 6, printMenuText)) {
            case 1:
                std::cout << "\nViewing Users...\n\n";
                pauseScreen();
                UserService::viewUsers();
                break;
            case 2:
                std::cout << "\nAdding User...\n\n";
                pauseScreen();
                UserService::addUser();
                break;
            case 3:
                std::cout << "\nToggling User Status...\n\n";
                pauseScreen();
                UserService::toggleUserStatus();
                break;
            case 4:
                std::cout << "\nDeleting User...\n\n";
                pauseScreen();
                UserService::deleteUser();
                break;
            case 5:
                std::cout << "\nManaging User Access...\n\n";
                pauseScreen();
                UserService::manageUserAccess();
                break;
            case 6:
                std::cout << "\nGoing Back...\n\n";
                pauseScreen();
                return;
            default:
                std::cout << "\nInvalid choice. Please try again.\n";
                pauseScreen();
                break;
        }
    }
}
