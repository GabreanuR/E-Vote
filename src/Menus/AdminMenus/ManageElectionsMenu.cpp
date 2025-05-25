#include <iostream>
#include "../include/Menus/AdminMenus/ManageElectionsMenu.h"

void ManageElectionsMenu::printMenuText() {
    std::cout << "==================================================\n"
              << "Manage Elections\n"
              << "==================================================\n"
              << "1. Create New Election\n"
              << "2. Manage Existing Elections\n"
              << "3. View Election Results\n"
              << "4. Back\n";
}

void ManageElectionsMenu::display() {
    while (true) {
        clearScreen();
        printMenuText();

        switch (getValidatedInput(1, 4, printMenuText)) {
            case 1:
                std::cout << "\nCreating New Election...\n\n";
                pauseScreen();
                createElection();
                break;
            case 2:
                std::cout << "\nManaging Existing Elections...\n\n";
                pauseScreen();
                manageExistingElections();
                break;
            case 3:
                std::cout << "\nViewing Election Results...\n\n";
                pauseScreen();
                viewElectionResults();
                break;
            case 4:
                return;
            default:
                break;
        }
    }
}

void ManageElectionsMenu::createElection() {
    clearScreen();
    std::cout << "===== Create Election =====\n";

    std::cout << "Functionality coming soon...\n";

    pauseScreen();
}

void ManageElectionsMenu::manageExistingElections() {
    clearScreen();
    std::cout << "===== Manage Existing Elections =====\n";

    std::cout << "Functionality coming soon...\n";

    pauseScreen();
}

void ManageElectionsMenu::viewElectionResults() {
    clearScreen();
    std::cout << "===== Election Results =====\n";

    std::cout << "Functionality coming soon...\n";

    pauseScreen();
} 