#include <iostream>
#include "ui/ui.h"
#include "admin.h"
using namespace std;

void adminLogin() {
    clearScreen();
    cout << "=== ADMIN LOGIN ===\n";
    cout << "Enter Admin ID: ";
    string id;
    cin >> id;

    cout << "Enter Password: ";
    string password;
    cin >> password;

    // TODO: validate admin credentials
    cout << "\nWelcome, Admin " << id << "!\n";
    pause();
    adminMenu();
}

void adminMenu() {
    while (true) {
        clearScreen();
        cout << "==================================================\n";
        cout << "Admin Dashboard\n";
        cout << "==================================================\n";
        cout << "1. Create New Election\n";
        cout << "2. View All Elections\n";
        cout << "3. End an Election\n";
        cout << "4. Add New Voter\n";
        cout << "5. Remove Voter\n";
        cout << "6. Logout\n";
        cout << "Enter your choice: ";

        char choice;
        cin >> choice;

        switch (choice) {
            case '1':
                cout << "\n[Create New Election functionality here]\n";
                pause();
                break;
            case '2':
                cout << "\n[View All Elections functionality here]\n";
                pause();
                break;
            case '3':
                cout << "\n[End an Election functionality here]\n";
                pause();
                break;
            case '4':
                cout << "\n[Add New Voter functionality here]\n";
                pause();
                break;
            case '5':
                cout << "\n[Remove Voter functionality here]\n";
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
