#include "../include/AuthMenu.h"
#include "../include/Voter.h"
#include "../include/Admin.h"
#include "../include/Utility.h"

#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

using namespace std;

AuthMenu::AuthMenu(UserType type) : userType(type) {
    if (userType == UserType::Voter)
        jsonPath = "data/voters.json";
    else
        jsonPath = "data/admins.json";
}

bool AuthMenu::verifyCredentials(const string& username, const string& password) const {
    ifstream inFile(jsonPath);
    if (!inFile.is_open()) {
        cerr << "Error: could not open user file: " << jsonPath << endl;
        return false;
    }

    json userList;
    inFile >> userList;

    for (const auto& user : userList) {
        if (user["username"] == username && user["password"] == password) {
            return true;
        }
    }
    return false;
}

void AuthMenu::display() {
    clearScreen();
    cout << (userType == UserType::Voter ? "=== Voter Login ===" : "=== Admin Login ===") << "\n";

    int attempts = 3;
    while (attempts-- > 0) {
        string username, password;

        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        if (verifyCredentials(username, password)) {
            authenticatedUser = username;
            cout << "\nLogin successful! Welcome, " << username << ".\n";
            pauseScreen();

            if (userType == UserType::Voter) {
                Voter voterMenu(username);
                voterMenu.showDashboard();
            } else {
                Admin adminMenu(username);
                adminMenu.showDashboard();
            }

            return;
        } else {
            cout << "\nInvalid credentials. Attempts left: " << attempts << "\n";
        }
    }

    cout << "\nToo many failed attempts. Returning to main menu...\n";
    pauseScreen();
}

string AuthMenu::getAuthenticatedUsername() const {
    return authenticatedUser;
}