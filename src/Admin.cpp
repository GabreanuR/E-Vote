#include "../include/Admin.h"
#include <iostream>
#include <limits>
#include "../include/Type.h"
#include <fstream>
#include <sstream>

void clearScreen();
void pause();

using namespace std;

Admin::Admin(const std::string& id) : User(id) {}

void Admin::showDashboard() {
    std::vector<std::unique_ptr<Election>> elections;

    while (true) {
        clearScreen();
        cout << "==================================================\n";
        cout << "Admin Dashboard\n";
        cout << "==================================================\n";
        cout << "1. Create Election\n";
        cout << "2. View Elections\n";
        cout << "3. End Election\n";
        cout << "4. Add Voter\n";
        cout << "5. Remove Voter\n";
        cout << "6. Logout\n";
        cout << "Enter your choice: ";

        char choice;
        cin >> choice;
        //If the user inputs multiple characters, this ignores it
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case '1': {
                cout << "\nCreating Election...\n\n";
                createElection(elections);
                pause();
                break;
            }
            case '2': {
                cout << "\nViewing Elections...\n\n";
                //viewElectionsDashboard();
                pause();
                break;
            }
            case '3': {
                cout << "\nEnding Election...\n\n";
                pause();
                break;
            }
            case '4': {
                cout << "\nAdding Voter...\n\n";
                pause();
                break;
            }
            case '5': {
                cout << "\nRemoving Voter...\n\n";
                pause();
                break;
            }
            case '6': {
                cout << "\nLogging out!\n\n";
                pause();
                return;
            }
            default: {
                cout << "\n'" << choice << "' is an invalid choice!\n\n";
                pause();
            }
        }
    }
}

void Admin::createElection(std::vector<std::unique_ptr<Election>> &elections) {
    while (true) {
        clearScreen();
        cout << "=== Create New Election ===\n";

        string electionId;
        cout << "Enter Election ID: ";
        cin >> electionId;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        string title;
        cout << "Enter Election Title: ";
        getline(cin, title);

        char typeChoice;
        while (true) {
            cout << "Choose election type:\n";
            cout << "1. Majority\n";
            cout << "2. Proportional\n";
            cout << "Choice: ";
            cin >> typeChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (typeChoice == '1' || typeChoice == '2') break;
            cout << "\nInvalid election type. Please enter 1 or 2.\n\n";
        }

        char levelChoice;
        while (true) {
            cout << "Choose election level:\n";
            cout << "1. National\n";
            cout << "2. Regional\n";
            cout << "3. Municipal\n";
            cout << "4. Local\n";
            cout << "5. Corporate\n";
            cout << "Choice: ";
            cin >> levelChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (levelChoice >= '1' && levelChoice <= '5') break;
            cout << "\nInvalid election level. Please enter a number from 1 to 5.\n\n";
        }

        auto level = static_cast<ElectionLevel>(levelChoice - '1');

        unique_ptr<Election> election;
        if (typeChoice == '1') {
            election = make_unique<Majority>(electionId, title, level);
        } else {
            election = make_unique<Proportional>(electionId, title, level);
        }

        string posName;
        cout << "Enter name of the position to be elected (e.g., 'President'): ";
        getline(cin, posName);

        int candCount;
        cout << "How many candidates for " << posName << "? ";
        cin >> candCount;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        vector<string> candidates;
        for (int j = 0; j < candCount; ++j) {
            string cand;
            cout << "Candidate " << (j + 1) << ": ";
            getline(cin, cand);
            candidates.push_back(cand);
        }

        Position pos(posName, candidates);
        election->addPosition(pos);

        clearScreen();
        cout << "\nPreview of the election:\n";
        election->displayDetails();
        cout << "Position: " << pos.getName() << " - Candidates: ";
        for (const auto& cand : pos.getCandidates()) {
            cout << cand << ", ";
        }
        cout << "\n";

        char confirm;
        cout << "\nConfirm creation? (y/n): ";
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (tolower(confirm) == 'y') {
            elections.push_back(std::move(election));
            cout << "Election created successfully!\n";

            ofstream outFile("ElectionDate.txt", ios::app);
            if (outFile.is_open()) {
                outFile << electionId << "|"
                        << title << "|"
                        << (typeChoice == '1' ? "Majority" : "Proportional") << "|"
                        << static_cast<int>(level) << "|"
                        << pos.getName() << "|";

                for (size_t i = 0; i < candidates.size(); ++i) {
                    outFile << candidates[i];
                    if (i != candidates.size() - 1) outFile << ",";
                }
                outFile << "\n";
                outFile.close();
            }
            break;
        } else {
            cout << "Restarting election creation...\n";
            pause();
        }


    }
}

// std::vector<std::unique_ptr<Election>> loadElectionsFromFile() {
//     std::vector<std::unique_ptr<Election>> elections;
//     ifstream inFile("src/Elections/ElectionData.txt");
//     string line;
//     while (getline(inFile, line)) {
//         istringstream iss(line);
//         string id, title, typeStr, levelStr, posName, candList;
//         getline(iss, id, '|');
//         getline(iss, title, '|');
//         getline(iss, typeStr, '|');
//         getline(iss, levelStr, '|');
//         getline(iss, posName, '|');
//         getline(iss, candList);
//
//         ElectionLevel level = static_cast<ElectionLevel>(stoi(levelStr));
//         vector<string> candidates;
//         istringstream candStream(candList);
//         string cand;
//         while (getline(candStream, cand, ',')) {
//             candidates.push_back(cand);
//         }
//
//         unique_ptr<Election> election;
//         if (typeStr == "Majority") {
//             election = make_unique<Majority>(id, title, level);
//         } else {
//             election = make_unique<Proportional>(id, title, level);
//         }
//
//         Position pos(posName, candidates);
//         election->addPosition(pos);
//         elections.push_back(std::move(election));
//     }
//     return elections;
// }

// void Admin::viewElectionsDashboard() {
//     clearScreen();
//     auto elections = loadElectionsFromFile();
//     if (elections.empty()) {
//         cout << "No elections found.\n";
//         pause();
//         return;
//     }
//
//     cout << "=== All Elections ===\n";
//     for (size_t i = 0; i < elections.size(); ++i) {
//         cout << i + 1 << ". " << elections[i]->getId() << " - " << elections[i]->getTitle() << "\n";
//     }
//
//     cout << "\nSelect an election to view details or 0 to return: ";
//     int choice;
//     cin >> choice;
//     cin.ignore();
//
//     if (choice > 0 && choice <= elections.size()) {
//         clearScreen();
//         elections[choice - 1]->displayDetails();
//         pause();
//     }
// }



