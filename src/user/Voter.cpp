#include "Voter.h"
#include "ui/ui.h"
#include <iostream>
#include <limits>
using namespace std;

Voter::Voter(const std::string& id) : User(id) {}

void Voter::showDashboard() {
    while (true) {
        clearScreen();
        cout << "==================================================\n";
        cout << "Voter Dashboard\n";
        cout << "==================================================\n";
        cout << "1. View Available Elections\n";
        cout << "2. Cast Vote\n";
        cout << "3. View My Votes\n";
        cout << "4. Change Vote\n";
        cout << "5. View Results\n";
        cout << "6. Logout\n";
        cout << "Enter your choice: ";

        char choice;
        cin >> choice;
        //If the user inputs multiple characters, this ignores it
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case '1': {
                cout << "\nViewing Available Elections...\n\n";
                pause();
                break;
            }
            case '2': {
                cout << "\nCasting Vote...\n\n";
                pause();
                break;
            }
            case '3': {
                cout << "\nViewing My Votes...\n\n";
                pause();
                break;
            }
            case '4': {
                cout << "\nChanging Vote...\n\n";
                pause();
                break;
            }
            case '5': {
                cout << "\nViewing Results...\n\n";
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