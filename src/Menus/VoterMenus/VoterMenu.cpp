#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Menus/VoterMenus/VoterMenu.h"
#include "Services/ElectionService.h"
#include "Services/CandidateService.h"
#include "Models/User.h"
#include "Models/Election.h"
#include "Models/Candidate.h"
#include "Menus/Meniu.h"
#include <memory>
#include <utility>

VoterMenu::VoterMenu(std::shared_ptr<User> user) : loggedInUser(std::move(user)) {
}

void VoterMenu::printVoterMenuText() const {
    std::cout << "==================================================\n"
            << "Voter Dashboard (Logged in as: " << (loggedInUser ? loggedInUser->getUsername() : "Unknown") << ")\n"
            << "==================================================\n"
            << "1. Vote\n"
            << "2. View Results\n"
            << "3. Back\n";
}

void VoterMenu::display() {
    while (true) {
        clearScreen();
        printVoterMenuText();

        switch (getValidatedInput(1, 3, [this]() { this->printVoterMenuText(); })) {
            case 1: {
                std::cout << "\nVoting...\n\n";
                pauseScreen();
                vote();
                break;
            }
            case 2: {
                std::cout << "\nViewing results...\n\n";
                pauseScreen();
                viewResults();
                break;
            }
            case 3:
                std::cout << "\nGoing Back...\n\n";
                pauseScreen();
                return;
            default: {
                std::cout << "\nInvalid choice. Please try again.\n";
                pauseScreen();
                break;
            }
        }
    }
}

void VoterMenu::vote() const {
    clearScreen();
    std::cout << "--- Available Elections for Voting ---\n";
    ElectionService &electionService = ElectionService::getInstance();
    const std::vector<std::shared_ptr<Election> > availableElections = electionService.
            getAvailableElectionsForUser(loggedInUser);

    if (availableElections.empty()) {
        std::cout << "No elections currently available for you to vote in.\n";
        pauseScreen();
        return;
    }

    for (size_t i = 0; i < availableElections.size(); ++i) {
        std::cout << (i + 1) << ". " << availableElections[i]->getName()
                << " (Level: " << Election::electionLevelToString(availableElections[i]->getElectionLevel())
                << ")\n";
    }
    std::cout << "Enter election choice (Press Enter to Cancel): ";
    std::string election_choice_str;
    std::getline(std::cin, election_choice_str);

    if (election_choice_str.empty()) {
        if (std::cin.eof()) {
            std::cin.clear();
        }
        std::cout << "\nVoting cancelled.\n";
        pauseScreen();
        return;
    }

    int election_idx;
    try {
        election_idx = std::stoi(election_choice_str);
    } catch (const std::exception &) {
        std::cout << "Invalid election choice format.\n";
        pauseScreen();
        return;
    }

    if (election_idx < 1 || election_idx > static_cast<int>(availableElections.size())) {
        std::cout << "Invalid election choice number.\n";
        pauseScreen();
        return;
    }

    const std::shared_ptr<Election> &selectedElection = availableElections[static_cast<size_t>(election_idx) - 1];

    clearScreen();
    std::cout << "--- Candidates for Election: " << selectedElection->getName() << " ---\n";
    const CandidateService &candidateService = CandidateService::getInstance();
    const std::vector<std::shared_ptr<Candidate> > candidates = candidateService.getCandidatesForElection(
        selectedElection->getId());

    if (candidates.empty()) {
        std::cout << "No candidates found for this election. Please contact an administrator.\n";
        pauseScreen();
        return;
    }

    for (size_t i = 0; i < candidates.size(); ++i) {
        std::cout << (i + 1) << ". " << candidates[i]->getName()
                << " (Party: " << candidates[i]->getPoliticalParty() << ")\n";
    }
    std::cout << "Enter candidate choice (Press Enter to Cancel): ";
    std::string candidate_choice_str;
    std::getline(std::cin, candidate_choice_str);

    if (candidate_choice_str.empty()) {
        if (std::cin.eof()) {
            std::cin.clear();
        }
        std::cout << "\nVoting cancelled.\n";
        pauseScreen();
        return;
    }

    int candidate_idx;
    try {
        candidate_idx = std::stoi(candidate_choice_str);
    } catch (const std::exception &) {
        std::cout << "Invalid candidate choice format.\n";
        pauseScreen();
        return;
    }

    if (candidate_idx < 1 || candidate_idx > static_cast<int>(candidates.size())) {
        std::cout << "Invalid candidate choice number.\n";
        pauseScreen();
        return;
    }

    const std::shared_ptr<Candidate> &selectedCandidate = candidates[static_cast<size_t>(candidate_idx) - 1];

    std::cout << "\nYou are about to vote for \"" << selectedCandidate->getName()
            << "\" in the election \"" << selectedElection->getName() << "\"." << std::endl;
    std::cout << "This action is final. Confirm? (y/n): ";
    std::string confirm_vote_str;
    std::getline(std::cin, confirm_vote_str);

    if (!confirm_vote_str.empty() && (std::tolower(static_cast<unsigned char>(confirm_vote_str[0])) == 'y')) {
        if (electionService.castVote(selectedElection->getId(), selectedCandidate->getId(), loggedInUser)) {
            std::cout << "\nVote has been processed.\n";
        } else {
            std::cout << "\nVote could not be processed. Please see previous messages for details.\n";
        }
        pauseScreen();
    } else {
        std::cout << "Voting cancelled by user.\n";
        pauseScreen();
    }
}

void VoterMenu::viewResults() const {
    clearScreen();
    std::cout << "--- Election Results ---" << std::endl;
    const ElectionService &electionService = ElectionService::getInstance();
    const auto &allElections = electionService.getAllElections();

    std::vector<std::shared_ptr<Election> > viewableElections;
    for (const auto &election: allElections) {
        if (election && (election->getStatus() == ElectionStatus::open || election->getStatus() ==
                         ElectionStatus::closed)) {
            if (loggedInUser && loggedInUser->hasAccessTo(election->getElectionLevel(),
                                                          election->getLocationId())) {
                viewableElections.push_back(election);
            }
        }
    }

    if (viewableElections.empty()) {
        std::cout <<
                "No election results available for viewing at this time."
                << std::endl;
        pauseScreen();
        return;
    }

    std::cout << "Select an election to view its results:" << std::endl;
    for (size_t i = 0; i < viewableElections.size(); ++i) {
        std::cout << (i + 1) << ". " << viewableElections[i]->getName()
                << " (Status: " << Election::electionStatusToString(viewableElections[i]->getStatus()) <<
                ")" << std::endl;
    }
    std::cout << "Enter choice (Press Enter to Go Back): ";
    std::string result_choice_str;
    std::getline(std::cin, result_choice_str);

    if (result_choice_str.empty()) {
        if (std::cin.eof()) {
            std::cin.clear();
        }
        return;
    }

    int result_idx;

    try {
        result_idx = std::stoi(result_choice_str);
    } catch (const std::exception &) {
        std::cout << "Invalid choice format." << std::endl;
        pauseScreen();
        return;
    }

    if (result_idx < 1 || result_idx > static_cast<int>(viewableElections.size())) {
        std::cout << "Invalid election choice number." << std::endl;
        pauseScreen();
        return;
    }

    const std::shared_ptr<Election> selectedElectionForResults = viewableElections[
        static_cast<size_t>(result_idx) - 1];
    clearScreen();
    std::cout << "--- Results for Election: " << selectedElectionForResults->getName() << " ---" <<
            std::endl;
    std::cout << "Status: " << Election::electionStatusToString(selectedElectionForResults->getStatus()) <<
            std::endl;
    std::cout << "Total Votes Cast: " << selectedElectionForResults->getVoteTotal() << std::endl <<
            std::endl;

    const CandidateService &candidateService = CandidateService::getInstance();

    if (const auto &candidateIds = selectedElectionForResults->getCandidateIds(); candidateIds.empty()) {
        std::cout << "No candidates were registered for this election." << std::endl;
    } else {
        std::cout << "Candidate Votes:" << std::endl;
        std::vector<std::pair<std::string, int> > candidateResults;
        for (const int candidateId: candidateIds) {
            if (const auto candidate = candidateService.getCandidate(candidateId)) {
                candidateResults.emplace_back(
                    candidate->getName() + " (Party: " + candidate->getPoliticalParty() + ")",
                    selectedElectionForResults->getVotesForCandidate(candidateId)
                );
            }
        }
        std::ranges::sort(candidateResults, [](const auto &a, const auto &b) {
            return a.second > b.second;
        });

        for (const auto &[candidate, votes]: candidateResults) {
            std::cout << "  - " << candidate << ": " << votes << " vote(s)" << std::endl;
        }
    }
    pauseScreen();
}
