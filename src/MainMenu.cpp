#include <iostream>
#include <limits>
#include "../include/MainMenu.h"
#include "../include/Utility.h"
#include "../include/Authenticator.h"

using namespace std;

void MainMenu::display() {
    while (true) {
        clearScreen();
        cout << "==================================================\n";
        cout << "Welcome to the E-VOTE System!\n";
        cout << "==================================================\n";
        cout << "1. Login as Voter\n";
        cout << "2. Login as Admin\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        char choice;
        cin >> choice;
        //If the user inputs multiple characters, this ignores it
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case '1': {
                cout << "\nAccessing Voter Login\n\n";
                pauseScreen();
                Authenticator auth("voter");
                if (User *user = auth.authenticate()) {
                    user->showDashboard();
                    delete user;
                }
                break;
            }
            case '2': {
                cout << "\nAccessing Admin Login\n\n";
                pauseScreen();
                Authenticator auth("admin");
                if (User *user = auth.authenticate()) {
                    user->showDashboard();
                    delete user;
                }
                break;
            }
            case '3': {
                cout << "\nExiting Program!\n\n";
                pauseScreen();
                return;
            }
            default: {
                cout << "\n'" << choice << "' is an invalid choice!\n\n";
                pauseScreen();
            }
        }
    }
}
