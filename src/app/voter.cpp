#include <iostream>
#include "ui/ui.h"
#include "voter.h"
using namespace std;

void voterLogin() {
    clearScreen();
    cout << "=== VOTER LOGIN ===\n";
    cout << "Enter Voter ID: ";
    string id;
    cin >> id;

    cout << "Enter Password: ";
    string password;
    cin >> password;

    // TODO: validate credentials from file or data
    cout << "\nWelcome, Voter " << id << "!\n";
    pause();
    voterMenu();
}

void voterMenu() {
    while (true) {
        clearScreen();
        cout << "==================================================\n";
        cout << "Voter Dashboard\n";
        cout << "==================================================\n";
        cout << "1. View Available Elections\n";
        cout << "2. Cast Vote in Ongoing Election\n";
        cout << "3. View My Votes (if public)\n";
        cout << "4. Change My Vote (if allowed)\n";
        cout << "5. View Election Results\n";
        cout << "6. Logout\n";
        cout << "Enter your choice: ";

        char choice;
        cin >> choice;

        switch (choice) {
            case '1':
                cout << "\n[Viewing available elections...]\n";
                pause();
                break;
            case '2':
                cout << "\n[Casting vote in an ongoing election...]\n";
                pause();
                break;
            case '3':
                cout << "\n[Viewing your votes...]\n";
                pause();
                break;
            case '4':
                cout << "\n[Changing your vote...]\n";
                pause();
                break;
            case '5':
                cout << "\n[Viewing election results...]\n";
                pause();
                break;
            case '6':
                cout << "\nLogging out...\n";
                pause();
                return;
            default:
                cout << "\n'" << choice << "' is an invalid choice!\n";
                pause();
        }
    }
}
