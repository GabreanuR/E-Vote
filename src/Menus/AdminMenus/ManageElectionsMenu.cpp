#include <iostream>
#include "../../../include/Menus/AdminMenus/ManageElectionsMenu.h"
#include "../../../include/Services/ElectionService.h"

void ManageElectionsMenu::printMenuText() {
    std::cout << "==================================================\n"
              << "Manage Elections\n"
              << "==================================================\n"
              << "1. Create New Election\n"
              << "2. Manage Existing Elections\n"
              << "3. View Election Results\n"
              << "0. Back\n";
}

void ManageElectionsMenu::display() {
    while (true) {
        clearScreen();
        printMenuText();

        std::cout << "Enter your choice: ";
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "0") {
            break;
        }

        switch (std::stoi(choice)) {
            case 1:
                std::cout << "\nCreating New Election...\n\n";
                pauseScreen();
                ElectionService::getInstance().createElection();
                break;
            case 2:
                std::cout << "\nManaging Existing Elections...\n\n";
                pauseScreen();
                ElectionService::getInstance().manageExistingElections();
                break;
            case 3:
                std::cout << "\nViewing Election Results...\n\n";
                pauseScreen();
                ElectionService::getInstance().viewElectionResults();
                break;
            default:
                std::cout << "\nInvalid choice. Please try again.\n";
                pauseScreen();
                break;
        }
    }
}
