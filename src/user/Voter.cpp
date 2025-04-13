#include "Voter.h"
#include "ui/ui.h"
#include <iostream>
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

        if (choice == '6') break;
        cout << "Functionality not yet implemented";
        pause();
    }
}