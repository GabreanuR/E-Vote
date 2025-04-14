#include "Authenticator.h"
#include "user/Voter.h"
#include "user/Admin.h"
#include <iostream>
#include <utility>
#include "ui/ui.h"

using namespace std;

Authenticator::Authenticator(string type) : userType(std::move(type)) {}

User* Authenticator::authenticate() const {
    clearScreen();
    cout << "=== " << (userType == "voter" ? "VOTER" : "ADMIN") << " LOGIN ===\n";
    cout << "Enter ID: ";
    string id;
    cin >> id;

    cout << "Enter Password: ";
    string password;
    cin >> password;

    // TODO: Check from file/database
    if (userType == "voter") {
        return new Voter(id);
    }
    if (userType == "admin") {
        return new Admin(id);
    }
    return nullptr;
}
