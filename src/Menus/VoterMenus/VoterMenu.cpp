#include <iostream>
#include "../../../include/Menus/VoterMenus/VoterMenu.h"
#include "../../../include/Services/ElectionService.h"

void VoterMenu::printMenuText() {
    std::cout << "==================================================\n"
              << "Voter Dashboard\n"
              << "==================================================\n"
              << "1. Vote\n"
              << "2. View Results\n"
              << "0. Back\n";
}

void VoterMenu::display() {
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
            case 1: {
                std::cout << "\nVoting in Election...\n\n";
                pauseScreen();
                ElectionService::getInstance().voteInElection();
                break;
            }
            case 2: {
                std::cout << "\nViewing Election Results...\n\n";
                pauseScreen();
                ElectionService::getInstance().viewElectionResults();
                break;
            }
            default: {
                std::cout << "\nInvalid choice. Please try again.\n";
                pauseScreen();
                break;
            }
        }
    }
}
