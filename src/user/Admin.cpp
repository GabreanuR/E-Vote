#include "Admin.h"
#include "ui/ui.h"
#include <iostream>
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

        if (choice == '6') break;
        cout << "Functionality not yet implemented";
        pause();
    }
}