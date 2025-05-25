#include <iostream>
#include "../include/Menus/VoterMenus/VoterMenu.h"
#include "../include/Menus/VoterMenus/ViewAvailableElectionsMenu.h"
#include "../include/Menus/VoterMenus/VoteInElectionMenu.h"
#include "../include/Menus/VoterMenus/ViewMyVotesMenu.h"
#include "../include/Menus/VoterMenus/ViewElectionResultsMenu.h"

void VoterMenu::printVoterMenuText() {
    std::cout << "==================================================\n"
              << "Voter Dashboard\n"
              << "==================================================\n"
              << "1. View Available Elections\n"
              << "2. Vote in an Election\n"
              << "3. View My Votes\n"
              << "4. View Election Results\n"
              << "5. Logout\n";
}

void VoterMenu::display() {
    while (true) {
        clearScreen();
        printVoterMenuText();

        switch (getValidatedInput(1, 5, printVoterMenuText)) {
            case 1: {
                std::cout << "\nViewing Available Elections...\n\n";
                pauseScreen();
                ViewAvailableElectionsMenu().display();
                break;
            }
            case 2: {
                std::cout << "\nVoting in Election...\n\n";
                pauseScreen();
                VoteInElectionMenu().display();
                break;
            }
            case 3: {
                std::cout << "\nViewing My Votes...\n\n";
                pauseScreen();
                ViewMyVotesMenu().display();
                break;
            }
            case 4: {
                std::cout << "\nViewing Election Results...\n\n";
                pauseScreen();
                ViewElectionResultsMenu().display();  // <-- New functionality
                break;
            }
            case 5: {
                std::cout << "\nLogging out!\n\n";
                pauseScreen();
                return;
            }
            default: ;
        }
    }
}
