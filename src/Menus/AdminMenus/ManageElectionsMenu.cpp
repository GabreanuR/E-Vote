#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "../include/Menus/AdminMenus/ManageElectionsMenu.h"
#include "../include/Menus/AdminMenus/ManageCandidatesMenu.h"
#include "../include/Services/ElectionService.h"
#include "../include/Services/CandidateService.h"
#include "../include/Models/Election.h"
#include "../include/Utils/Types.h"
#include "../include/Menus/Meniu.h"

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
    clearScreen();
    std::cout << "===== Create New Election =====\n\n";

    try {
        Election newElectionCandidate;
        std::cin >> newElectionCandidate;

        const auto createdElection = ElectionService::getInstance().createElection(
            newElectionCandidate.getName(),
            newElectionCandidate.getElectionLevel(),
            newElectionCandidate.getVotingSystem(),
            newElectionCandidate.getLocationId()
        );

        if (createdElection) {
            std::cout << "\nelection created successfully with ID: " << createdElection->getId() << "\n";
        } else {
            std::cout << "\nFailed to create election (service returned null or an error occurred).\n";
        }
    } catch (const UserInputCancelledException &) {
        std::cout << "\nElection creation cancelled by user.\n";
    } catch (const std::exception &e) {
        std::cout << "\nAn unexpected error occurred during election creation: " << e.what() << "\n";
    }
    pauseScreen();
}

void ManageElectionsMenu::printToggleElectionStatusMenuText() {
    std::cout << "==================================================\n"
            << "Toggle Election Status Submenu\n"
            << "==================================================\n"
            << "1. Open an Election (from 'Created' status)\n"
            << "2. Close an Election (from 'Open' status)\n"
            << "3. Back to Manage Elections Menu\n";
}

void ManageElectionsMenu::toggleElectionStatus() {
    while (true) {
        clearScreen();
        printToggleElectionStatusMenuText();

        switch (getValidatedInput(1, 3, printToggleElectionStatusMenuText)) {
            case 1:
                std::cout << "\nOpening Election...\n\n";
                pauseScreen();
                openElection();
                break;
            case 2:
                std::cout << "\nClosing Election...\n\n";
                pauseScreen();
                closeElection();
                break;
            case 3:
                std::cout << "\nGoing Back...\n\n";
                pauseScreen();
                return;
            default:
                break;
        }
    }
}

void ManageElectionsMenu::viewElections() {
    clearScreen();
    std::cout << "===== View All Elections =====\n\n";

    if (const auto &allElections = ElectionService::getInstance().getAllElections(); allElections.empty()) {
        std::cout << "No elections found in the system.\n";
    } else {
        for (const auto &electionPtr: allElections) {
            std::cout << "--------------------------------------------------\n";
            if (electionPtr) {
                std::cout << *electionPtr << "\n";

                if (electionPtr->getStatus() == ElectionStatus::closed) {
                    displayElectionResults(electionPtr);
                }
            }
        }
        std::cout << "--------------------------------------------------\n";
    }
    pauseScreen();
}

void ManageElectionsMenu::displayElectionResults(const std::shared_ptr<Election> &electionPtr) {
    std::cout << "  --- Candidates and Votes ---\n";
    if (const auto &candidateIds = electionPtr->getCandidateIds(); candidateIds.empty()) {
        std::cout << "    No candidates were assigned to this election.\n";
    } else {
        const int totalElectionVotes = electionPtr->getVoteTotal();

        for (const int candId: candidateIds) {
            if (const auto candidate = CandidateService::getInstance().getCandidate(candId)) {
                std::cout << "    Candidate: " << candidate->getName()
                        << " (ID: " << candidate->getId()
                        << ", Party: " << candidate->getPoliticalParty() << ")\n";
                std::cout << "      Votes: " << candidate->getVotes();
                if (electionPtr->getVotingSystem() == VotingSystemType::proportional) {
                    if (totalElectionVotes > 0) {
                        const double percentage =
                                (static_cast<double>(candidate->getVotes()) / totalElectionVotes) *
                                100.0;
                        std::cout << " (" << std::fixed << std::setprecision(2) << percentage << "%)";
                    } else {
                        std::cout << " (0.00%)";
                    }
                }
                std::cout << "\n";
            } else {
                std::cout << "    Could not retrieve details for Candidate ID: " << candId << "\n";
            }
        }
        if (totalElectionVotes == 0 && electionPtr->getVotingSystem() == VotingSystemType::proportional
            && !candidateIds.empty()) {
            std::cout << "    (Note: Percentages are 0.00% as the total election vote count is 0.)\n";
        }
    }
}

void ManageElectionsMenu::openElection() {
    clearScreen();
    std::cout << "===== Open an Election =====\n";
    const auto &allElections = ElectionService::getInstance().getAllElections();
    std::vector<std::shared_ptr<Election> > createdElections;

    for (const auto &election: allElections) {
        if (election && election->getStatus() == ElectionStatus::created) {
            createdElections.push_back(election);
        }
    }

    if (createdElections.empty()) {
        std::cout << "No elections are currently in 'Created' status. Cannot open any election.\n";
        pauseScreen();
        return;
    }

    std::cout << "\n--- Elections in 'Created' Status ---\n";
    for (size_t i = 0; i < createdElections.size(); ++i) {
        const auto &election = createdElections[i];
        std::cout << (i + 1) << ". ID: " << election->getId() << ", Name: " << election->getName() << "\n";
    }
    std::cout << "-------------------------------------\n";

    auto reprintOpenMenu = [&]() {
        clearScreen();
        std::cout << "===== Open an Election =====\n";
        std::cout << "\n--- Elections in 'Created' Status ---\n";
        for (size_t i = 0; i < createdElections.size(); ++i) {
            const auto &election = createdElections[i];
            std::cout << (i + 1) << ". ID: " << election->getId() << ", Name: " << election->getName() << "\n";
        }
        std::cout << "-------------------------------------\n";
        std::cout << "Enter the number of the election to open (or press Enter to cancel): ";
    };

    std::string inputStr;
    int chosenListIndex;

    while (true) {
        reprintOpenMenu();
        std::getline(std::cin, inputStr);

        if (inputStr.empty()) {
            if (std::cin.eof()) {
                std::cin.clear();
            }
            std::cout << "\nOperation cancelled.\n";
            pauseScreen();
            return;
        }

        try {
            if (const int choice = std::stoi(inputStr);
                choice >= 1 && choice <= static_cast<int>(createdElections.size())) {
                chosenListIndex = choice - 1;
                break;
            }
            std::cout << "\nInvalid choice. Please enter a number between 1 and "
                    << createdElections.size() << ".\n";
        } catch (const std::invalid_argument &) {
            std::cout << "\nInvalid input. Please enter a number or press Enter to cancel.\n";
        } catch (const std::out_of_range &) {
            std::cout << "\nInput is out of range for a number. Please try again.\n";
        }
        pauseScreen();
    }

    const std::shared_ptr<Election> selectedElection = createdElections[static_cast<size_t>(chosenListIndex)];

    std::cout << "\nAttempting to open election: '" << selectedElection->getName() << "' (ID: " << selectedElection->
            getId() << ")...\n";
    ElectionService::getInstance().toggleElectionStatus(selectedElection->getId());
    pauseScreen();
}

void ManageElectionsMenu::closeElection() {
    clearScreen();
    std::cout << "===== Close an Election =====\n";
    const auto &allElections = ElectionService::getInstance().getAllElections();
    std::vector<std::shared_ptr<Election> > openElections;

    for (const auto &election: allElections) {
        if (election && election->getStatus() == ElectionStatus::open) {
            openElections.push_back(election);
        }
    }

    if (openElections.empty()) {
        std::cout << "No elections are currently in 'Open' status. Cannot close any election.\n";
        pauseScreen();
        return;
    }

    std::cout << "\n--- Elections in 'Open' Status ---\n";
    for (size_t i = 0; i < openElections.size(); ++i) {
        const auto &election = openElections[i];
        std::cout << (i + 1) << ". ID: " << election->getId() << ", Name: " << election->getName() << "\n";
    }
    std::cout << "----------------------------------\n";

    auto reprintCloseMenu = [&]() {
        clearScreen();
        std::cout << "===== Close an Election =====\n";
        std::cout << "\n--- Elections in 'Open' Status ---\n";
        for (size_t i = 0; i < openElections.size(); ++i) {
            const auto &election = openElections[i];
            std::cout << (i + 1) << ". ID: " << election->getId() << ", Name: " << election->getName() << "\n";
        }
        std::cout << "----------------------------------\n";
        std::cout << "Enter the number of the election to close (or press Enter to cancel): ";
    };

    std::string inputStr;
    int chosenListIndex;

    while (true) {
        reprintCloseMenu();
        std::getline(std::cin, inputStr);

        if (inputStr.empty()) {
            if (std::cin.eof()) {
                std::cin.clear();
            }
            std::cout << "\nOperation cancelled.\n";
            pauseScreen();
            return;
        }

        try {
            if (const int choice = std::stoi(inputStr);
                choice >= 1 && choice <= static_cast<int>(openElections.size())) {
                chosenListIndex = choice - 1;
                break;
            }
            std::cout << "\nInvalid choice. Please enter a number between 1 and "
                    << openElections.size() << ".\n";
        } catch (const std::invalid_argument &) {
            std::cout << "\nInvalid input. Please enter a number or press Enter to cancel.\n";
        } catch (const std::out_of_range &) {
            std::cout << "\nInput is out of range for a number. Please try again.\n";
        }
        pauseScreen();
    }

    const std::shared_ptr<Election> selectedElection = openElections[static_cast<size_t>(chosenListIndex)];

    std::cout << "\nAttempting to close election: '" << selectedElection->getName() << "' (ID: " << selectedElection->
            getId() << ")...\n";
    ElectionService::getInstance().toggleElectionStatus(selectedElection->getId());
    pauseScreen();
}
