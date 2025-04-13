#include <iostream>
#include "ui/ui.h"
#include "EvoteMenu.h"
#include "auth/Authenticator.h"

using namespace std;

void EvoteMenu::display() {
    while (true) {
        clearScreen();
        cout << "==================================================\n";
        cout << "Welcome to the E-VOTE System!\n";
        cout << "==================================================\n";
        cout << "1. Login as Voter\n";
        cout << "2. Login as Admin\n";
        cout << "3. Return to Main Menu\n";
        cout << "Enter your choice: ";

        char choice;
        cin >> choice;

        switch (choice) {
            case '1': {
                cout << "\nAccessing Voter Login\n\n";
                pause();
                Authenticator auth("voter");
                User* user = auth.authenticate();
                if (user) user->showDashboard();
                delete user;
                break;
            }
            case '2': {
                cout << "\nAccessing Admin Login\n\n";
                pause();
                Authenticator auth("admin");
                User* user = auth.authenticate();
                if (user) user->showDashboard();
                delete user;
                break;
            }
            case '3':
                cout << "\nExiting E-VOTE System\n\n";
            pause();
            return;
            default:
                cout << "\n'" << choice << "' is an invalid choice!\n\n";
            pause();
        }
    }
}