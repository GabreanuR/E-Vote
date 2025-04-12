#include <iostream>
using namespace std;

void runEvoteApp() {
    while (true) {
        cout << endl << "E-VOTE APP:" << endl;
        cout << "1. Login as Voter" << endl;
        cout << "2. View Elections" << endl;
        cout << "3. Return to Main Menu" << endl;
        cout << "Enter your choice: ";

        char choice;
        cin >> choice;

        switch (choice) {
            case '1':
                cout << "Logging in as voter..." << endl;

            break;
            case '2':
                cout << "Viewing elections..." << endl;

            break;
            case '3':
                cout << "Exiting E-VOTE APP" << endl;
                return;
            default:
                cout << "'" << choice << "' is an invalid choice!" << endl;
        }
    }
}