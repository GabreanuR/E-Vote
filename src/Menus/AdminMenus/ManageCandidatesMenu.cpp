#include "../include/Menus/AdminMenus/ManageCandidatesMenu.h"
#include "../include/Services/CandidateService.h"
#include "../include/Services/ElectionService.h"
#include "../include/Models/Candidate.h"
#include "../include/Utils/Types.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <vector>

void ManageCandidatesMenu::printMenuText() {
    std::cout << "==================================================\n"
              << "Manage Candidates Menu\n"
              << "==================================================\n"
              << "1. Create Candidate\n"
              << "2. Assign Candidate to an Election\n"
              << "3. View Candidates\n"
              << "4. Back\n";
}

void ManageCandidatesMenu::display() {
    while (true) {
        clearScreen();
        printMenuText();

        switch (getValidatedInput(1, 4, printMenuText)) {
            case 1:
                std::cout << "Creating Candidate...\n";
                pauseScreen();
                createCandidate();
                break;
            case 2:
                std::cout << "Adding Candidate...\n";
                pauseScreen();
                addCandidateToElection();
                break;
            case 3:
                std::cout << "Viewing Candidates...\n";
                pauseScreen();
                viewCandidates();
                break;
            case 4:
                std::cout << "\nGoing Back...\n\n";
                pauseScreen();
                return;
            default:
                break;
        }
    }
}

void ManageCandidatesMenu::createCandidate() {
    clearScreen();
    std::cout << "==================================================\n"
              << "Create New Candidate Profile\n"
              << "==================================================\n";
    try {
        Candidate tempCandidate;
        std::cin >> tempCandidate;

        CandidateService::getInstance().addCandidate(tempCandidate.getName(), tempCandidate.getPoliticalParty(), tempCandidate.getDescription());
        
    } catch (const UserInputCancelledException& e) {
        std::cout << "\nCandidate creation cancelled: " << e.what() << "\n";
    }
    pauseScreen();
}

void ManageCandidatesMenu::addCandidateToElection() {
    clearScreen();
    std::cout << "==================================================\n"
              << "Assign Candidate to an Election\n"
              << "==================================================\n";

    try {
        std::string inputStr;
        int electionIdToAssignTo;
        int candidateIdToAssign;
        std::cout << "Enter ID of the candidate to assign (or press Enter to cancel): ";
        while (true) {
            std::getline(std::cin, inputStr);
            if (inputStr.empty() && std::cin.eof()) { std::cin.clear(); throw UserInputCancelledException(); }
            if (inputStr.empty()) throw UserInputCancelledException();
            try {
                candidateIdToAssign = std::stoi(inputStr);
                if (!CandidateService::getInstance().getCandidate(candidateIdToAssign)) {
                    std::cerr << "Error: Candidate with ID " << candidateIdToAssign << " not found. Please enter a valid Candidate ID: ";
                    continue;
                }
                break;
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid input for Candidate ID. Please enter a number: ";
            } catch (const std::out_of_range&) {
                std::cerr << "Candidate ID out of range. Please enter a valid number: ";
            }
        }

        std::cout << "Enter Election ID to assign the candidate to (or press Enter to cancel): ";
        while (true) {
            std::getline(std::cin, inputStr);
            if (inputStr.empty() && std::cin.eof()) { std::cin.clear(); throw UserInputCancelledException(); }
            if (inputStr.empty()) throw UserInputCancelledException();
            try {
                electionIdToAssignTo = std::stoi(inputStr);
                if (!ElectionService::getInstance().getElection(electionIdToAssignTo)) {
                    std::cerr << "Error: Election with ID " << electionIdToAssignTo << " not found or service stubbed. Please enter a valid Election ID: ";
                    continue;
                }
                break;
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid input for Election ID. Please enter a number: ";
            } catch (const std::out_of_range&) {
                std::cerr << "Election ID out of range. Please enter a valid number: ";
            }
        }

        if (CandidateService::getInstance().assignCandidateToElection(candidateIdToAssign, electionIdToAssignTo)) {
        } else {
            std::cout << "\nFailed to assign candidate to election. See error above or check logs.\n";
        }

    } catch (const UserInputCancelledException& e) {
        std::cout << "\nAssignment cancelled: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "\nAn unexpected error occurred during assignment: " << e.what() << "\n";
    }
    pauseScreen();
}

void ManageCandidatesMenu::printSortMenuText() {
    std::cout << "Sort by:\n"
              << "  1. Candidate ID (Ascending)\n"
              << "  2. Name (Alphabetical)\n"
              << "  3. Election ID (Unassigned first, then by ID Ascending)\n"
              << "  0. Display Unsorted (Current Order)\n";
}

void ManageCandidatesMenu::viewCandidates() {
    clearScreen();
    std::cout << "==================================================\n"
              << "View All Candidates\n"
              << "==================================================\n";

    std::vector<std::shared_ptr<Candidate>> allCandidates = CandidateService::getInstance().getAllCandidates();

    if (allCandidates.empty()) {
        std::cout << "No candidates found in the system.\n";
    } else {
        switch (getValidatedInput(1, 3, printSortMenuText)) {
            case 1:
                std::ranges::sort(allCandidates, [](const auto& a, const auto& b) {
                    return a->getId() < b->getId();
                });
                std::cout << "\n--- Candidates Sorted by ID ---\n";
                break;
            case 2:
                std::ranges::sort(allCandidates, [](const auto& a, const auto& b) {
                    return a->getName() < b->getName();
                });
                std::cout << "\n--- Candidates Sorted by Name ---\n";
                break;
            case 3:
                std::ranges::sort(allCandidates, [](const auto& a, const auto& b) {
                    const int idWEa = a->getElectionId();
                    const int idWEb = b->getElectionId();
                    const bool idEmpty1 = (idWEa == -1);
                    const bool idEmpty2 = (idWEb == -1);

                    if (idEmpty1 && !idEmpty2) return true;
                    if (!idEmpty1 && idEmpty2) return false;
                    if (idWEa != idWEb) return idWEa < idWEb;
                    if (idEmpty1) return a->getId() < b->getId();
                    return a->getId() < b->getId();
                });
                std::cout << "\n--- Candidates Sorted by Election ID (Unassigned First) ---\n";
                break;
            default:
                break;
        }

        for (const auto& candidate : allCandidates) {
            if(candidate) {
                std::cout << *candidate << "\n--------------------------------------------------\n";
            } else {
                std::cout << "Null candidate pointer encountered.\n--------------------------------------------------\n";
            }
        }
    }
    pauseScreen();
}

