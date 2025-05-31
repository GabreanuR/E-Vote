#include "Menus/AuthMenu.h"
#include "Menus/VoterMenus/VoterMenu.h"
#include "Menus/AdminMenus/AdminMenu.h"
#include "Services/UserService.h"
#include "Models/User.h"
#include <iostream>
#include <memory>

AuthMenu::AuthMenu(const UserType type) : userType(type) {
}

std::shared_ptr<User> AuthMenu::attemptLogin(const std::string &username, const std::string &password) const {
    const UserService &userService = UserService::getInstance();
    std::shared_ptr<User> user = userService.getUserByUsername(username);

    if (!user) {
        return nullptr;
    }

    if (user->isDisabled()) {
        std::cout << "Account disabled. Please contact an administrator." << std::endl;
        return nullptr;
    }

    if (user->getType() != userType) {
        std::cout << "Access denied. User type does not match login type." << std::endl;
        return nullptr;
    }

    if (user->getPassword() != password) {
        return nullptr;
    }

    return user;
}

void AuthMenu::display() {
    clearScreen();
    std::cout << (userType == UserType::voter ? "=== Voter Login ===" : "=== Admin Login ===") << "\n";

    int attempts = 3;
    while (attempts-- > 0) {
        std::string username, password;

        std::cout << "Username: ";
        std::getline(std::cin, username);
        if (username.empty()) {
            std::cout << "Login cancelled." << std::endl;
            return;
        }

        std::cout << "Password: ";
        std::getline(std::cin, password);
        if (password.empty()) {
            std::cout << "Login cancelled." << std::endl;
            return;
        }

        currentUser = attemptLogin(username, password);

        if (currentUser) {
            std::cout << "\nLogin successful! Welcome, " << currentUser->getUsername() << ".\n";
            pauseScreen();

            if (userType == UserType::voter) {
                VoterMenu voterMenu(currentUser);
                voterMenu.display();
            } else {
                AdminMenu adminMenu(currentUser);
                adminMenu.display();
            }
            return;
        }

        const std::shared_ptr<User> existingUser = UserService::getInstance().getUserByUsername(username);
        bool specificMessageAlreadyShown = false;

        if (existingUser && (existingUser->isDisabled() || existingUser->getType() != userType))
            specificMessageAlreadyShown = true;

        if (!specificMessageAlreadyShown) {
            std::cout << "\nInvalid username or password.";
        }

        std::cout << " Attempts left: " << attempts << "\n";
        pauseScreen();
        clearScreen();
        std::cout << (userType == UserType::voter ? "=== Voter Login ===\n" : "=== Admin Login ===\n");
    }

    std::cout << "\nToo many failed attempts. Returning to main menu...\n";
    pauseScreen();
}
