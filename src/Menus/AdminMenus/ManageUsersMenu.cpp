#include <iostream>
#include <nlohmann/json.hpp>
#include "../include/Menus/AdminMenus/ManageUsersMenu.h"
#include "../include/Utils/DataManager.h"
#include "../include/Models/User.h"

using json = nlohmann::json;

void ManageUsersMenu::printMenuText() {
    std::cout << "==================================================\n"
              << "Manage Users\n"
              << "==================================================\n"
              << "1. View All Users\n"
              << "2. Add New User\n"
              << "3. Enable/Disable User\n"
              << "4. Delete User\n"
              << "5. Manage User Access\n"
              << "6. Back\n";
}

void ManageUsersMenu::display() {
    while (true) {
        clearScreen();
        printMenuText();

        switch (getValidatedInput(1, 6, printMenuText)) {
            case 1:
                std::cout << "\nViewing Users...\n\n";
                pauseScreen();
                viewUsers();
                break;
            case 2:
                std::cout << "\nAdding User...\n\n";
                pauseScreen();
                addUser();
                break;
            case 3:
                std::cout << "\nToggling User Status...\n\n";
                pauseScreen();
                toggleUserStatus();
                break;
            case 4:
                std::cout << "\nDeleting User...\n\n";
                pauseScreen();
                deleteUser();
                break;
            case 5:
                std::cout << "\nManaging User Access...\n\n";
                pauseScreen();
                manageUserAccess();
                break;
            case 6:
                return;
            default:
                break;
        }
    }
}

void ManageUsersMenu::viewUsers() {
    clearScreen();
    std::cout << "===== All Users =====\n\n";

    const json users = DataManager::getInstance().loadData("users.json");
    
    for (const auto& userData : users) {
        const User user(userData);
        displayUserDetails(user);
    }
    
    pauseScreen();
}

void ManageUsersMenu::displayUserDetails(const User& user) {
    std::cout << "ID: " << user.getId() << "\n"
              << "Username: " << user.getUsername() << "\n"
              << "Type: " << (user.getType() == UserType::admin ? "Admin" : "Voter") << "\n"
              << "Status: " << (user.isDisabled() ? "Disabled" : "Enabled") << "\n"
              << "Access Levels:\n";
    
    const auto& access = user.getRestrictedAccess();
    
    // National access
    auto nationalIt = access.find(ElectionLevel::national);
    std::cout << "  National: " << (nationalIt == access.end() || nationalIt->second.empty() ? "Full" : "Restricted") << "\n";
    
    // Regional access
    std::cout << "  Regions: ";
    auto regionalIt = access.find(ElectionLevel::regional);
    if (regionalIt == access.end() || regionalIt->second.empty()) {
        std::cout << "Full\n";
    } else {
        std::cout << "Restricted to: ";
        for (const auto& id : regionalIt->second) {
            std::cout << id << " ";
        }
        std::cout << "\n";
    }
    
    // Municipal access
    std::cout << "  Municipalities: ";
    auto municipalIt = access.find(ElectionLevel::municipal);
    if (municipalIt == access.end() || municipalIt->second.empty()) {
        std::cout << "Full\n";
    } else {
        std::cout << "Restricted to: ";
        for (const auto& id : municipalIt->second) {
            std::cout << id << " ";
        }
        std::cout << "\n";
    }
    
    // Local access
    std::cout << "  Localities: ";
    auto localIt = access.find(ElectionLevel::local);
    if (localIt == access.end() || localIt->second.empty()) {
        std::cout << "Full\n";
    } else {
        std::cout << "Restricted to: ";
        for (const auto& id : localIt->second) {
            std::cout << id << " ";
        }
        std::cout << "\n";
    }
    
    // Non-government access
    std::cout << "  Non-Government: ";
    auto nonGovIt = access.find(ElectionLevel::nonGovernment);
    if (nonGovIt == access.end() || nonGovIt->second.empty()) {
        std::cout << "Full\n";
    } else {
        std::cout << "Restricted to: ";
        for (const auto& id : nonGovIt->second) {
            std::cout << id << " ";
        }
        std::cout << "\n";
    }
    
    std::cout << "----------------------------------------\n";
}

void ManageUsersMenu::addUser() {
    clearScreen();
    std::cout << "===== Add New User =====\n\n";

    // Get user type
    std::cout << "User Type:\n"
              << "1. Voter\n"
              << "2. Admin\n"
              << "Enter choice (empty to go back): ";
    
    std::string choice;
    std::getline(std::cin, choice);
    if (choice.empty()) return;
    
    if (choice != "1" && choice != "2") {
        std::cout << "\nInvalid choice. Please enter 1 or 2.\n";
        pauseScreen();
        return;
    }
    
    const UserType type = (choice == "2") ? UserType::admin : UserType::voter;
    
    // Get username
    std::string username;
    std::cout << "Enter username (empty to go back): ";
    std::getline(std::cin, username);
    if (username.empty()) return;
    
    // Validate username
    if (username.length() < 3) {
        std::cout << "\nUsername must be at least 3 characters long.\n";
        pauseScreen();
        return;
    }
    
    // Check if username already exists
    const json users = DataManager::getInstance().loadData("users.json");
    for (const auto& user : users) {
        if (user["username"] == username) {
            std::cout << "\nUsername already exists.\n";
            pauseScreen();
            return;
        }
    }
    
    // Get password
    std::string password;
    std::cout << "Enter password (empty to go back): ";
    std::getline(std::cin, password);
    if (password.empty()) return;
    
    // Validate password
    if (password.length() < 6) {
        std::cout << "\nPassword must be at least 6 characters long.\n";
        pauseScreen();
        return;
    }
    
    try {
        // Create new user
        User newUser(username, password, type);
        
        // Save to users file
        json updatedUsers = users;
        updatedUsers.push_back(newUser.toJson());
        
        // Save users file
        if (DataManager::getInstance().saveData("users.json", updatedUsers)) {
            std::cout << "\nUser added successfully!\n";
        } else {
            std::cout << "\nError adding user.\n";
        }
    } catch (const User::UserCreationError& e) {
        std::cout << "\nError: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "\nUnexpected error: " << e.what() << "\n";
    }
    
    pauseScreen();
}

void ManageUsersMenu::toggleUserStatus() {
    clearScreen();
    std::cout << "===== Enable/Disable User =====\n\n";

    // Load users
    json users = DataManager::getInstance().loadData("users.json");
    
    if (users.empty()) {
        std::cout << "No users found.\n";
        pauseScreen();
        return;
    }
    
    // Display users
    for (const auto& user : users) {
        std::cout << "ID: " << user["id"] << "\n"
                  << "Username: " << user["username"] << "\n"
                  << "Type: " << user["type"] << "\n"
                  << "Status: " << (user["disabled"].get<bool>() ? "Disabled" : "Enabled") << "\n"
                  << "----------------------------------------\n";
    }
    
    // Get user ID
    std::cout << "\nEnter user ID to toggle status (empty to go back): ";
    std::string idStr;
    std::getline(std::cin, idStr);
    if (idStr.empty()) return;
    
    try {
        const int userId = std::stoi(idStr);
        
        // Find user
        auto it = std::ranges::find_if(users,
            [userId](const json& user) { return user["id"] == userId; });
        
        if (it == users.end()) {
            std::cout << "\nUser not found.\n";
            pauseScreen();
            return;
        }
        
        // Toggle status
        (*it)["disabled"] = !(*it)["disabled"].get<bool>();
        
        // Save changes
        if (DataManager::getInstance().saveData("users.json", users)) {
            std::cout << "\nUser status updated successfully!\n";
        } else {
            std::cout << "\nError updating user status.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "\nError: " << e.what() << "\n";
    }
    
    pauseScreen();
}

void ManageUsersMenu::deleteUser() {
    clearScreen();
    std::cout << "===== Delete User =====\n\n";

    // Load users
    json users = DataManager::getInstance().loadData("users.json");
    
    if (users.empty()) {
        std::cout << "No users found.\n";
        pauseScreen();
        return;
    }
    
    // Display users
    for (const auto& user : users) {
        std::cout << "ID: " << user["id"] << "\n"
                  << "Username: " << user["username"] << "\n"
                  << "Type: " << user["type"] << "\n"
                  << "----------------------------------------\n";
    }
    
    // Get user ID
    std::cout << "\nEnter user ID to delete (empty to go back): ";
    std::string idStr;
    std::getline(std::cin, idStr);
    if (idStr.empty()) return;
    
    try {
        const int userId = std::stoi(idStr);
        
        // Confirm deletion
        std::cout << "\nAre you sure you want to delete this user? (y/n): ";
        std::string confirm;
        std::getline(std::cin, confirm);
        
        if (confirm != "y" && confirm != "Y") {
            std::cout << "\nDeletion cancelled.\n";
            pauseScreen();
            return;
        }
        
        // Delete user
        if (User::deleteUser(userId)) {
            std::cout << "\nUser deleted successfully!\n";
        } else {
            std::cout << "\nError deleting user.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "\nError: " << e.what() << "\n";
    }
    
    pauseScreen();
}

void ManageUsersMenu::manageUserAccess() {
    clearScreen();
    std::cout << "===== Manage User Access =====\n\n";

    // Load users
    json users = DataManager::getInstance().loadData("users.json");
    
    if (users.empty()) {
        std::cout << "No users found.\n";
        pauseScreen();
        return;
    }
    
    // Display users
    for (const auto& user : users) {
        if (user["type"] == "voter") {  // Only show voters
            std::cout << "ID: " << user["id"] << "\n"
                      << "Username: " << user["username"] << "\n"
                      << "----------------------------------------\n";
        }
    }
    
    // Get user ID
    std::cout << "\nEnter user ID to manage access (empty to go back): ";
    std::string idStr;
    std::getline(std::cin, idStr);
    if (idStr.empty()) return;
    
    try {
        const int userId = std::stoi(idStr);
        
        // Find user
        auto it = std::ranges::find_if(users,
            [userId](const json& user) { return user["id"] == userId; });
        
        if (it == users.end()) {
            std::cout << "\nUser not found.\n";
            pauseScreen();
            return;
        }
        
        if ((*it)["type"] != "voter") {
            std::cout << "\nOnly voter access can be managed.\n";
            pauseScreen();
            return;
        }
        
        // Create User object
        User user(*it);
        
        // Display current access
        std::cout << "\nCurrent Access Levels:\n";
        const auto& access = user.getRestrictedAccess();
        
        // National access
        std::cout << "1. National: " << (access.at(ElectionLevel::national).empty() ? "Full" : "Restricted") << "\n";
        
        // Regional access
        std::cout << "2. Regions: ";
        if (access.at(ElectionLevel::regional).empty()) {
            std::cout << "Full\n";
        } else {
            std::cout << "Restricted to: ";
            for (const auto& id : access.at(ElectionLevel::regional)) {
                std::cout << id << " ";
            }
            std::cout << "\n";
        }
        
        // Municipal access
        std::cout << "3. Municipalities: ";
        if (access.at(ElectionLevel::municipal).empty()) {
            std::cout << "Full\n";
        } else {
            std::cout << "Restricted to: ";
            for (const auto& id : access.at(ElectionLevel::municipal)) {
                std::cout << id << " ";
            }
            std::cout << "\n";
        }
        
        // Local access
        std::cout << "4. Localities: ";
        if (access.at(ElectionLevel::local).empty()) {
            std::cout << "Full\n";
        } else {
            std::cout << "Restricted to: ";
            for (const auto& id : access.at(ElectionLevel::local)) {
                std::cout << id << " ";
            }
            std::cout << "\n";
        }
        
        // Non-government access
        std::cout << "5. Non-Government: ";
        if (access.at(ElectionLevel::nonGovernment).empty()) {
            std::cout << "Full\n";
        } else {
            std::cout << "Restricted to: ";
            for (const auto& id : access.at(ElectionLevel::nonGovernment)) {
                std::cout << id << " ";
            }
            std::cout << "\n";
        }
        
        // Get access level to modify
        std::cout << "\nEnter access level to modify (1-5, empty to go back): ";
        std::string levelStr;
        std::getline(std::cin, levelStr);
        if (levelStr.empty()) return;
        
        const int level = std::stoi(levelStr);
        if (level < 1 || level > 5) {
            std::cout << "\nInvalid access level.\n";
            pauseScreen();
            return;
        }
        
        // Get entity ID
        std::cout << "Enter entity ID (empty for full access): ";
        std::string entityIdStr;
        std::getline(std::cin, entityIdStr);
        
        // Convert level to ElectionLevel
        ElectionLevel electionLevel;
        switch (level) {
            case 1: electionLevel = ElectionLevel::national; break;
            case 2: electionLevel = ElectionLevel::regional; break;
            case 3: electionLevel = ElectionLevel::municipal; break;
            case 4: electionLevel = ElectionLevel::local; break;
            case 5: electionLevel = ElectionLevel::nonGovernment; break;
            default:
                std::cout << "\nInvalid access level.\n";
                pauseScreen();
                return;
        }
        
        // Update access
        if (entityIdStr.empty()) {
            // Grant full access
            user.grantAccess(electionLevel, 0);  // 0 indicates full access
        } else {
            try {
                const int entityId = std::stoi(entityIdStr);
                user.grantAccess(electionLevel, entityId);
            } catch (const std::exception& e) {
                std::cout << "\nError: " << e.what() << "\n";
                pauseScreen();
                return;
            }
        }
        
        // Save changes
        (*it) = user.toJson();
        if (DataManager::getInstance().saveData("users.json", users)) {
            std::cout << "\nUser access updated successfully!\n";
        } else {
            std::cout << "\nError updating user access.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "\nError: " << e.what() << "\n";
    }
    
    pauseScreen();
}
