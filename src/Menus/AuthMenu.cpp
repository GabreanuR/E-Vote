#include "Menus/AuthMenu.h"
#include "Menus/VoterMenus/VoterMenu.h"
#include "Menus/AdminMenus/AdminMenu.h"
#include "Utils/DataManager.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

const std::string AuthMenu::jsonPath = "data/users.json";

AuthMenu::AuthMenu(const UserType type) : userType(type) {}

bool AuthMenu::verifyCredentials(const std::string& username, const std::string& password) const {
    const json userList = DataManager::getInstance().loadData(jsonPath);
    
    for (const auto& user : userList) {
        if (user["username"] == username && 
            user["password"] == password &&
            user["type"].get<std::string>() == (userType == UserType::admin ? "admin" : "voter") &&
            !user["disabled"].get<bool>()) {
            return true;
        }
    }
    return false;
}

void AuthMenu::display() {
    clearScreen();
    std::cout << (userType == UserType::voter ? "=== Voter Login ===" : "=== Admin Login ===") << "\n";

    int attempts = 3;
    while (attempts-- > 0) {
        std::string username, password;

        std::cout << "Username: ";
        std::getline(std::cin, username);

        std::cout << "Password: ";
        std::getline(std::cin, password);

        if (verifyCredentials(username, password)) {
            authenticatedUser = username;
            std::cout << "\nLogin successful! Welcome, " << username << ".\n";
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
            std::cout << "\nInvalid credentials. Attempts left: " << attempts << "\n";
            pauseScreen();
        }
    }

    std::cout << "\nToo many failed attempts. Returning to main menu...\n";
    pauseScreen();
}