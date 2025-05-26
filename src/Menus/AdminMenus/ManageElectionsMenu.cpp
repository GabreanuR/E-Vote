#include <iostream>
#include <string>
#include <stdexcept>
#include "../include/Menus/AdminMenus/ManageElectionsMenu.h"
#include "../include/Menus/AdminMenus/ManageCandidatesMenu.h"
#include "../include/Services/ElectionService.h"
#include "../include/Models/Election.h"

void ManageElectionsMenu::printMenuText() {
    std::cout << "==================================================\n"
              << "Manage Elections Menu\n"
              << "==================================================\n"
              << "1. Create Election\n"
              << "2. Start/Close Election\n"
              << "3. View Elections\n"
              << "4. Manage Candidates for an Election\n"
              << "5. Back\n";
}

void ManageElectionsMenu::display() {
    while (true) {
        clearScreen();
        printMenuText();

        switch (getValidatedInput(1, 5, printMenuText)) {
            case 1:
                std::cout << "\nCreating Election...\n\n";
                pauseScreen();
                createElection();
                break;
            case 2:
                std::cout << "\nStarting/Closing Election...\n\n";
                pauseScreen();
                toggleElectionStatus();
                break;
            case 3:
                std::cout << "\nViewing Elections...\n\n";
                pauseScreen();
                viewElections();
                break;
            case 4:
                std::cout << "\nManaging Candidates for an Election...\n\n";
                pauseScreen();
                ManageCandidatesMenu().display();
                break;
            case 5:
                std::cout << "\nGoing Back...\n\n";
                pauseScreen();
                return;
            default:
                break;
        }
    }
}

void ManageElectionsMenu::createElection() {

}

void ManageElectionsMenu::toggleElectionStatus() {

}

void ManageElectionsMenu::viewElections() {
}


