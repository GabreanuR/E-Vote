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

        std::cout << "Enter your choice: ";
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "6") {
            break;
        }

        switch (std::stoi(choice)) {
            case 1:
                std::cout << "\nViewing Users...\n\n";
                pauseScreen();
                UserService::getInstance().viewUsers();
                break;
            case 2:
                std::cout << "\nAdding User...\n\n";
                pauseScreen();
                UserService::getInstance().addUser();
                break;
            case 3:
                std::cout << "\nToggling User Status...\n\n";
                pauseScreen();
                UserService::getInstance().toggleUserStatus();
                break;
            case 4:
                std::cout << "\nDeleting User...\n\n";
                pauseScreen();
                UserService::getInstance().deleteUser();
                break;
            case 5:
                std::cout << "\nManaging User Access...\n\n";
                pauseScreen();
                UserService::getInstance().manageUserAccess();
                break;
            default:
                std::cout << "\nInvalid choice. Please try again.\n";
                pauseScreen();
                break;
        }
    }
}
