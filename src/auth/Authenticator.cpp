#include "Authenticator.h"
#include "user/Voter.h"
#include "user/Admin.h"
#include <iostream>

using namespace std;

Authenticator::Authenticator(const string &type) : userType(type) {}

User* Authenticator::authenticate() {
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
    } else {
        return new Admin(id);
    }
}