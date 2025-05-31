#include "../include/Menus/AdminMenus/ManageCandidatesMenu.h"
#include "../include/Services/CandidateService.h"
#include "../include/Services/ElectionService.h"
#include "../include/Models/Candidate.h"
#include "../include/Models/Election.h"
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

        CandidateService::getInstance().addCandidate(tempCandidate.getName(), tempCandidate.getPoliticalParty(),
                                                     tempCandidate.getDescription());
    } catch (const UserInputCancelledException &e) {
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
        std::cout << "--- Available Unassigned Candidates ---\n";
        const auto &allCandidates = CandidateService::getInstance().getAllCandidates();
        std::vector<std::shared_ptr<Candidate> > unassignedCandidates;
        for (const auto &cand: allCandidates) {
            if (cand && cand->getElectionId() == -1) {
                unassignedCandidates.push_back(cand);
            }
        }

        if (unassignedCandidates.empty()) {
            std::cout << "No unassigned candidates available in the system.\n";
            pauseScreen();
            return;
        }
        for (const auto &cand: unassignedCandidates) {
            std::cout << "ID: " << cand->getId() << ", Name: " << cand->getName()
                    << ", Party: " << cand->getPoliticalParty() << "\n";
        }
        std::cout << "-------------------------------------\n";

        std::string inputStr;
        std::shared_ptr<Candidate> selectedCandidate = nullptr;
        std::cout << "Enter ID of the candidate to assign (from the list above, or press Enter to cancel): ";
        while (true) {
            std::getline(std::cin, inputStr);
            if (inputStr.empty() && std::cin.eof()) {
                std::cin.clear();
                throw UserInputCancelledException();
            }
            if (inputStr.empty()) throw UserInputCancelledException();
            try {
                const int candidateIdToAssign = std::stoi(inputStr);
                bool foundInUnassigned = false;
                for (const auto &cand: unassignedCandidates) {
                    if (cand->getId() == candidateIdToAssign) {
                        selectedCandidate = cand;
                        foundInUnassigned = true;
                        break;
                    }
                }
                if (!foundInUnassigned) {
                    std::cerr << "Error: Candidate with ID " << candidateIdToAssign <<
                            " is not in the list of unassigned candidates or does not exist. \nPlease enter a valid ID: ";
                    continue;
                }
                std::cout << "\nSelected Candidate: " << selectedCandidate->getName() << " (ID: " << selectedCandidate->
                        getId() << ")\n";
                break;
            } catch (const std::invalid_argument &) {
                std::cerr << "Invalid input for Candidate ID. Please enter a number: ";
            } catch (const std::out_of_range &) {
                std::cerr << "Candidate ID out of range. Please enter a valid number: ";
            }
        }

        std::cout << "\n--- Available Elections (Status: Created) ---\n";
        const auto &allElections = ElectionService::getInstance().getAllElections();
        std::vector<std::shared_ptr<Election> > createdElections;
        for (const auto &election: allElections) {
            if (election && election->getStatus() == ElectionStatus::created) {
                createdElections.push_back(election);
            }
        }

        if (createdElections.empty()) {
            std::cout << "No elections with 'Created' status found. Cannot assign candidate.\n";
            pauseScreen();
            return;
        }
        for (const auto &election: createdElections) {
            std::cout << "ID: " << election->getId() << ", Name: " << election->getName()
                    << ", Level: " << Election::electionLevelToString(election->getElectionLevel())
                    << ", System: " << Election::votingSystemTypeToString(election->getVotingSystem()) << "\n";
        }
        std::cout << "------------------------------------------\n";

        int electionIdToAssignTo;
        std::cout << "Enter ID of the election to assign the candidate to (or press Enter to cancel): ";
        while (true) {
            std::getline(std::cin, inputStr);
            if (inputStr.empty() && std::cin.eof()) {
                std::cin.clear();
                throw UserInputCancelledException();
            }
            if (inputStr.empty()) throw UserInputCancelledException();
            try {
                electionIdToAssignTo = std::stoi(inputStr);
                const auto election = ElectionService::getInstance().getElection(electionIdToAssignTo);
                if (!election) {
                    std::cerr << "Error: Election with ID " << electionIdToAssignTo <<
                            " not found. \nPlease enter a valid Election ID: ";
                    continue;
                }
                if (election->getStatus() != ElectionStatus::created) {
                    std::cerr << "Error: Election with ID " << electionIdToAssignTo <<
                            " is not in 'Created' status. Current status: "
                            << Election::electionStatusToString(election->getStatus()) <<
                            ". \nPlease select an election with 'Created' status: ";
                    continue;
                }
                std::cout << "Selected Election: " << election->getName() << " (ID: " << election->getId() << ")\n";
                break;
            } catch (const std::invalid_argument &) {
                std::cerr << "Invalid input for Election ID. Please enter a number: ";
            } catch (const std::out_of_range &) {
                std::cerr << "Election ID out of range. Please enter a valid number: ";
            }
        }

        if (CandidateService::getInstance().
            assignCandidateToElection(selectedCandidate->getId(), electionIdToAssignTo)) {
            std::cout << "\nCandidate '" << selectedCandidate->getName() << "' (ID: " << selectedCandidate->getId()
                    << ") successfully assigned to election '" << ElectionService::getInstance().getElection(
                        electionIdToAssignTo)->getName()
                    << "' (ID: " << electionIdToAssignTo << ").\n";
        }
    } catch (const UserInputCancelledException &e) {
        std::cout << "\nAssignment cancelled: " << e.what() << "\n";
    } catch (const std::exception &e) {
        std::cerr << "\nAn unexpected error occurred during assignment: " << e.what() << "\n";
    }
    pauseScreen();
}

void ManageCandidatesMenu::printSortMenuText() {
    std::cout << "==================================================\n"
            << "View All Candidates\n"
            << "==================================================\n"
            << "Select display/sort option (default is by Candidate ID):\n"
            << "  1. Sort by Name (Alphabetical)\n"
            << "  2. Sort by Election ID (Unassigned first, then by ID Ascending)\n"
            << "  3. Back\n";
}

void ManageCandidatesMenu::viewCandidates() {
    clearScreen();

    std::vector<std::shared_ptr<Candidate> > allCandidates = CandidateService::getInstance().getAllCandidates();

    if (allCandidates.empty()) {
        std::cout << "No candidates found in the system.\n";
        pauseScreen();
        return;
    }

    printSortMenuText();

    std::string listHeader = "\n--- Candidate List (Default Order by ID) ---";

    switch (getValidatedInput(1, 3, printSortMenuText)) {
        case 1:
            std::ranges::sort(allCandidates, [](const auto &a, const auto &b) {
                return a->getName() < b->getName();
            });
            listHeader = "\n--- Candidates Sorted by Name ---";
            break;
        case 2:
            std::ranges::sort(allCandidates, [](const auto &a, const auto &b) {
                const int electionIdA = a->getElectionId();
                const int electionIdB = b->getElectionId();
                const bool isAUnassigned = (electionIdA == -1);
                const bool isBUnassigned = (electionIdB == -1);

                if (isAUnassigned && !isBUnassigned) return true;
                if (!isAUnassigned && isBUnassigned) return false;

                if (isAUnassigned) {
                    return a->getId() < b->getId();
                }

                if (electionIdA != electionIdB) {
                    return electionIdA < electionIdB;
                }

                return a->getId() < b->getId();
            });
            listHeader = "\n--- Candidates Sorted by Election ID (Unassigned First) ---";
            break;
        case 3:
            std::cout << "\nGoing Back...\n";
            pauseScreen();
            return;
        default:
            break;
    }

    if (!allCandidates.empty()) {
        std::cout << listHeader;
        std::cout << "\n--------------------------------------------------\n";
        for (const auto &candidate: allCandidates) {
            if (candidate) {
                std::cout << *candidate << "\n--------------------------------------------------\n";
            }
        }
    }

    pauseScreen();
}
