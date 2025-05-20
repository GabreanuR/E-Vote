#include "../include/Authenticator.h"
#include "../include/Voter.h"
#include "../include/Admin.h"
#include <iostream>
#include <utility>

void clearScreen();
void pauseScreen();

using namespace std;

Authenticator::Authenticator(string type) : userType(std::move(type)) {}

User* Authenticator::authenticate() const {
    clearScreen();
    cout << "=== " << (userType == "voter" ? "VOTER" : "ADMIN") << " LOGIN ===\n";

    string id, password;

    cout << "Enter ID: ";
    getline(cin, id);

    cout << "Enter Password: ";
    getline(cin, password);

    // TODO: Check from file/database
    if (userType == "voter") {
        return new Voter(id);
    }
    if (userType == "admin") {
        return new Admin(id);
    }
    return nullptr;
}
