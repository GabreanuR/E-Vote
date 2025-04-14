#include "Admin.h"
#include "ui/ui.h"
#include <iostream>
#include <limits>
using namespace std;

Admin::Admin(const std::string& id) : User(id) {}

void Admin::showDashboard() {
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
                pause();
                break;
            }
            case '2': {
                cout << "\nViewing Elections...\n\n";
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