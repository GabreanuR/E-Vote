#include <iostream>
#include "ui/ui.h"
using namespace std;

void runEvoteApp() {
    bool running = true;
    while (running) {
        clearScreen();
        cout << "==================================================\n";
        cout << "E-VOTE APP:\n";
        cout << "==================================================\n";
        cout << "1. Login\n";
        cout << "2. View Elections\n";
        cout << "3. Return to Main Menu\n";
        cout << "Enter your choice: ";

        char choice;
        cin >> choice;

        switch (choice) {
            case '1':
                cout << "\nLogin...\n\n";
                pause();
                break;
            case '2':
                cout << "\nViewing elections...\n\n";
                pause();
                break;
            case '3':
                cout << "\nExiting E-VOTE APP\n\n";
                running = false;
                return;
            default:
                cout << "\n'" << choice << "' is an invalid choice!\n\n";
                pause();
        }
    }
}
