#include "Menus/AuthMenu.h"
#include "Menus/VoterMenus/VoterMenu.h"
#include "Menus/AdminMenus/AdminMenu.h"

#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

using namespace std;

AuthMenu::AuthMenu(UserType type) : userType(type) {
    jsonPath = "data/users.json";
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
        if (user["username"] == username && 
            user["password"] == password &&
            user["type"].get<int>() == static_cast<int>(userType) &&
            !user["disabled"].get<bool>()) {
            return true;
        }
    }
    return false;
}

void AuthMenu::display() {
    clearScreen();
    cout << (userType == UserType::voter ? "=== Voter Login ===" : "=== Admin Login ===") << "\n";

    int attempts = 3;
    while (attempts-- > 0) {
        string username, password;

        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        if (verifyCredentials(username, password)) {
            authenticatedUser = username;
            cout << "\nLogin successful! Welcome, " << username << ".\n";
            pauseScreen();

            if (userType == UserType::voter) {
                VoterMenu voterMenu;
                voterMenu.display();
            } else {
                AdminMenu adminMenu;
                adminMenu.display();
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