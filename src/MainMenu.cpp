#include <iostream>
#include "../include/MainMenu.h"
#include <limits>
#include "../include/EvoteMenu.h"

void clearScreen();
void pause();

using namespace std;

void MainMenu::display() {
    while (true) {
        clearScreen();
        cout << "==================================================\n";
        cout << "Main Menu\n";
        cout << "==================================================\n";
        cout << "1. Normal Mode\n";
        cout << "2. Demo Mode\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        char choice;
        cin >> choice;
        //If the user inputs multiple characters, this ignores it
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case '1': {
                cout << "\nEntering Normal Mode...\n\n";
                pause();
                EvoteMenu menu;
                menu.display();
                break;
            }
            case '2': {
                cout << "\nEntering Demo Mode... (Not implemented)\n\n";
                pause();
                break;
            }
            case '3': {
                cout << "\nExiting Program!\n\n";
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
