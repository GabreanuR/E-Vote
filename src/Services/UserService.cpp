#include "../../include/Services/UserService.h"
#include "../../include/Utils/DataManager.h"
#include "../../include/Models/User.h"
#include "../../include/Menus/Meniu.h"
#include <iostream>

UserService* UserService::instance = nullptr;

UserService& UserService::getInstance() {
    if (instance == nullptr) {
        instance = new UserService();
    }
    return *instance;
}

void UserService::viewUsers() {
    Meniu::clearScreen();
    std::cout << "===== All Users =====\n\n";

    const json users = DataManager::getInstance().loadData("users");
    
    for (const auto& userData : users) {
        const User user(userData);
        displayUserDetails(user);
    }
    
    Meniu::pauseScreen();
}

void UserService::displayUserDetails(const User& user) {
    std::cout << "ID: " << user.getId() << "\n"
              << "Username: " << user.getUsername() << "\n"
              << "Type: " << (user.getType() == UserType::admin ? "Admin" : "Voter") << "\n"
              << "Status: " << (user.isDisabled() ? "Disabled" : "Enabled") << "\n"
              << "Access Levels:\n";
    
    const auto& access = user.getRestrictedAccess();
    
    // National access is always accessible
    std::cout << "  National: Full Access\n";
    
    // Regional access
    std::cout << "  Regions: ";
    const auto regionalIt = access.find(ElectionLevel::regional);
    if (regionalIt == access.end() || regionalIt->second.empty()) {
        std::cout << "No Access\n";
    } else {
        std::cout << "Restricted to: ";
        for (const auto& id : regionalIt->second) {
            std::cout << id << " ";
        }
        std::cout << "\n";
    }
    
    // Municipal access
    std::cout << "  Municipalities: ";
    const auto municipalIt = access.find(ElectionLevel::municipal);
    if (municipalIt == access.end() || municipalIt->second.empty()) {
        std::cout << "No Access\n";
    } else {
        std::cout << "Restricted to: ";
        for (const auto& id : municipalIt->second) {
            std::cout << id << " ";
        }
        std::cout << "\n";
    }
    
    // Local access
    std::cout << "  Localities: ";
    const auto localIt = access.find(ElectionLevel::local);
    if (localIt == access.end() || localIt->second.empty()) {
        std::cout << "No Access\n";
    } else {
        std::cout << "Restricted to: ";
        for (const auto& id : localIt->second) {
            std::cout << id << " ";
        }
        std::cout << "\n";
    }
    
    // Non-government access
    std::cout << "  Non-Government: ";
    const auto nonGovIt = access.find(ElectionLevel::nonGovernment);
    if (nonGovIt == access.end() || nonGovIt->second.empty()) {
        std::cout << "No Access\n";
    } else {
        std::cout << "Restricted to: ";
        for (const auto& id : nonGovIt->second) {
            std::cout << id << " ";
        }
        std::cout << "\n";
    }
    
    std::cout << "----------------------------------------\n";
}

void UserService::addUser() {
    Meniu::clearScreen();
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
        Meniu::pauseScreen();
        return;
    }
    
    const UserType type = (choice == "2") ? UserType::admin : UserType::voter;
    
    // Get username
    std::string username;
    std::cout << "Enter username (empty to go back): ";
    std::getline(std::cin, username);
    if (username.empty()) return;
    
    // Validate username
    if (!validateUsername(username)) {
        std::cout << "\nUsername must be at least 3 characters long.\n";
        Meniu::pauseScreen();
        return;
    }
    
    // Check if username already exists
    if (usernameExists(username)) {
        std::cout << "\nUsername already exists.\n";
        Meniu::pauseScreen();
        return;
    }
    
    // Get password
    std::string password;
    std::cout << "Enter password (empty to go back): ";
    std::getline(std::cin, password);
    if (password.empty()) return;
    
    // Validate password
    if (!validatePassword(password)) {
        std::cout << "\nPassword must be at least 6 characters long.\n";
        Meniu::pauseScreen();
        return;
    }
    
    try {
        // Create new user
        User newUser(username, password, type);
        
        // Save user
        json users = DataManager::getInstance().loadData("users");
        users.push_back(newUser.toJson());
        
        if (DataManager::getInstance().saveData("users", users)) {
            std::cout << "\nUser added successfully!\n";
        } else {
            std::cout << "\nError adding user.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "\nError: " << e.what() << "\n";
    }
    
    Meniu::pauseScreen();
}

void UserService::toggleUserStatus() {
    Meniu::clearScreen();
    std::cout << "===== Toggle User Status =====\n\n";

    // Load users
    json users = DataManager::getInstance().loadData("users");

    // Display users
    for (const auto& user : users) {
        std::cout << "ID: " << user["id"] << "\n"
                  << "Username: " << user["username"] << "\n"
                  << "Type: " << user["type"] << "\n"
                  << "Status: " << (user["disabled"].get<bool>() ? "Disabled" : "Enabled") << "\n"
                  << "----------------------------------------\n";
    }
    
    // Get user ID
    std::cout << "\nEnter user ID to toggle (empty to go back): ";
    std::string idStr;
    std::getline(std::cin, idStr);
    if (idStr.empty()) return;
    
    try {
        const int userId = std::stoi(idStr);

        // Prevent changing status of user with ID 1
        if (userId == 1) {
            std::cout << "\nCannot change status of the primary admin user (ID: 1).\n";
            Meniu::pauseScreen();
            return;
        }
        
        // Find user
        auto it = std::ranges::find_if(users,
            [userId](const json& user) { return user["id"] == userId; });

        if (it == users.end()) {
            std::cout << "\nUser not found.\n";
            Meniu::pauseScreen();
            return;
        }
        
        // Toggle status
        (*it)["disabled"] = !(*it)["disabled"].get<bool>();

        // Save changes
        if (DataManager::getInstance().saveData("users", users)) {
            std::cout << "\nUser status updated successfully!\n";
        } else {
            std::cout << "\nError updating user status.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "\nError: " << e.what() << "\n";
    }
    
    Meniu::pauseScreen();
}

void UserService::deleteUser() {
    Meniu::clearScreen();
    std::cout << "===== Delete User =====\n\n";

    // Load users
    json users = DataManager::getInstance().loadData("users");

    if (users.empty()) {
        std::cout << "No users found.\n";
        Meniu::pauseScreen();
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

        // Prevent deleting user with ID 1
        if (userId == 1) {
            std::cout << "\nCannot delete the primary admin user (ID: 1).\n";
            Meniu::pauseScreen();
            return;
        }
        
        // Find user
        auto it = std::ranges::find_if(users,
            [userId](const json& user) { return user["id"] == userId; });

        if (it == users.end()) {
            std::cout << "\nUser not found.\n";
            Meniu::pauseScreen();
            return;
        }
        
        // Confirm deletion
        std::cout << "\nAre you sure you want to delete this user? (y/n): ";
        std::string confirm;
        std::getline(std::cin, confirm);

        if (confirm != "y" && confirm != "Y") {
            std::cout << "\nDeletion cancelled.\n";
            Meniu::pauseScreen();
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

    Meniu::pauseScreen();
}

void UserService::manageUserAccess() {
    Meniu::clearScreen();
    std::cout << "===== Manage User Access =====\n\n";

    // Load users
    json users = DataManager::getInstance().loadData("users");

    if (users.empty()) {
        std::cout << "No users found.\n";
        Meniu::pauseScreen();
        return;
    }

    // Display users
    for (const auto& user : users) {
        if (user["type"] == "voter") {  // Only show voters
            std::cout << "ID: " << user["id"] << "\n"
                      << "Username: " << user["username"] << "\n"
                      << "Status: " << (user["disabled"].get<bool>() ? "Disabled" : "Enabled") << "\n"
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
            Meniu::pauseScreen();
            return;
        }

        if ((*it)["type"] != "voter") {
            std::cout << "\nOnly voter access can be managed.\n";
            Meniu::pauseScreen();
            return;
        }

        // Create User object
        User user(*it);

        // Display current access
        std::cout << "\nCurrent Access Levels:\n";
        std::cout << "User Status: " << (user.isDisabled() ? "Disabled (No Voting Rights)" : "Enabled (Can Vote)") << "\n\n";

        const auto& access = user.getRestrictedAccess();

        // Regional access
        const auto regionalIt = access.find(ElectionLevel::regional);
        std::cout << "1. Regions: ";
        if (regionalIt == access.end() || regionalIt->second.empty()) {
            std::cout << "No Access\n";
        } else {
            std::cout << "Restricted to: ";
            for (const auto& id : regionalIt->second) {
                std::cout << id << " ";
            }
            std::cout << "\n";
        }

        // Municipal access
        const auto municipalIt = access.find(ElectionLevel::municipal);
        std::cout << "2. Municipalities: ";
        if (municipalIt == access.end() || municipalIt->second.empty()) {
            std::cout << "No Access\n";
        } else {
            std::cout << "Restricted to: ";
            for (const auto& id : municipalIt->second) {
                std::cout << id << " ";
            }
            std::cout << "\n";
        }

        // Local access
        const auto localIt = access.find(ElectionLevel::local);
        std::cout << "3. Localities: ";
        if (localIt == access.end() || localIt->second.empty()) {
            std::cout << "No Access\n";
        } else {
            std::cout << "Restricted to: ";
            for (const auto& id : localIt->second) {
                std::cout << id << " ";
            }
            std::cout << "\n";
        }

        // Non-government access
        const auto nonGovIt = access.find(ElectionLevel::nonGovernment);
        std::cout << "4. Non-Government: ";
        if (nonGovIt == access.end() || nonGovIt->second.empty()) {
            std::cout << "No Access\n";
        } else {
            std::cout << "Restricted to: ";
            for (const auto& id : nonGovIt->second) {
                std::cout << id << " ";
            }
            std::cout << "\n";
        }

        // Get access level to modify
        std::cout << "\nEnter access level to modify (1-4, empty to go back): ";
        std::string levelStr;
        std::getline(std::cin, levelStr);
        if (levelStr.empty()) return;

        const int level = std::stoi(levelStr);
        if (level < 1 || level > 4) {
            std::cout << "\nInvalid access level.\n";
            Meniu::pauseScreen();
            return;
        }

        // Convert level to ElectionLevel
        ElectionLevel electionLevel = {};
        switch (level) {
            case 1: electionLevel = ElectionLevel::regional; break;
            case 2: electionLevel = ElectionLevel::municipal; break;
            case 3: electionLevel = ElectionLevel::local; break;
            case 4: electionLevel = ElectionLevel::nonGovernment; break;
            default: ;
        }

        // For regional level, show available regions
        if (electionLevel == ElectionLevel::regional) {
            // Load regions data
            json locations = DataManager::getInstance().loadData("locations");

            std::cout << "\nAvailable Regions:\n";
            if (locations.contains("regions")) {
                for (const auto& [id, region] : locations["regions"].items()) {
                    std::cout << "ID: " << id << " - "
                             << region["name"].get<std::string>() << "\n";
                }
            }
            std::cout << "\nNote: Voters can only have access to one region or none.\n"
                      << "Selecting a region will clear access to all municipalities and localities.\n";

            std::cout << "\nEnter region ID:\n"
                      << "   0: No access to any region (and sub-levels)\n"
                      << "  >0: Select specific region (clears sub-levels)\n"
                      << "Enter choice: ";
            std::string entityIdStr;
            std::getline(std::cin, entityIdStr);

            try {
                const int entityId = std::stoi(entityIdStr);

                if (entityId == 0) {
                    // Remove access to region and all sub-levels
                    user.grantAccess(ElectionLevel::regional, 0);
                    user.grantAccess(ElectionLevel::municipal, 0);
                    user.grantAccess(ElectionLevel::local, 0);
                } else if (entityId > 0) {
                    // Verify the region exists
                    if (!locations["regions"].contains(std::to_string(entityId))) {
                        std::cout << "\nInvalid region ID.\n";
                        Meniu::pauseScreen();
                        return;
                    }

                    // Grant access to specific region and clear sub-levels
                    user.grantAccess(ElectionLevel::regional, entityId);
                    user.grantAccess(ElectionLevel::municipal, 0);
                    user.grantAccess(ElectionLevel::local, 0);
                } else {
                    std::cout << "\nInvalid region ID. Must be 0 or positive.\n";
                    Meniu::pauseScreen();
                    return;
                }

                // Save changes
                (*it) = user.toJson();
                if (DataManager::getInstance().saveData("users", users)) {
                    std::cout << "\nUser access updated successfully!\n";
                } else {
                    std::cout << "\nError updating user access.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "\nError: " << e.what() << "\n";
            }

            Meniu::pauseScreen();
            return;
        }
        // For municipal level, check regional access first
        else if (electionLevel == ElectionLevel::municipal) {
            const auto regionalIt = access.find(ElectionLevel::regional);
            if (regionalIt == access.end() || regionalIt->second.empty()) {
                std::cout << "\nYou must have access to a specific region before managing municipal access.\n";
                std::cout << "\nInvalid choice. Please try again.\n";
                Meniu::pauseScreen();
                return;
            }

            // Get the region ID the user has access to
            const int regionId = *regionalIt->second.begin();

            // Load municipalities data
            json locations = DataManager::getInstance().loadData("locations");

            std::cout << "\nAvailable Municipalities in Region " << regionId << ":\n";
            if (locations.contains("municipalities")) {
                bool foundAny = false;
                for (const auto& [id, municipality] : locations["municipalities"].items()) {
                    if (municipality["region"].get<std::string>() == std::to_string(regionId)) {
                        std::cout << "ID: " << id << " - "
                                 << municipality["name"].get<std::string>() << "\n";
                        foundAny = true;
                    }
                }
                if (!foundAny) {
                    std::cout << "No municipalities found in this region.\n";
                }
            }

            std::cout << "\nEnter municipality ID:\n"
                      << "   0: No access to municipalities\n"
                      << "  >0: Select specific municipality from this region\n"
                      << "Enter choice: ";
            std::string entityIdStr;
            std::getline(std::cin, entityIdStr);

            try {
                const int entityId = std::stoi(entityIdStr);

                if (entityId == 0) {
                    // Remove access to municipalities and localities
                    user.grantAccess(ElectionLevel::municipal, 0);
                    user.grantAccess(ElectionLevel::local, 0);
                } else if (entityId > 0) {
                    // Verify the municipality belongs to the user's region
                    bool validMunicipality = false;
                    if (locations.contains("municipalities")) {
                        const auto& municipality = locations["municipalities"][std::to_string(entityId)];
                        if (!municipality.is_null() &&
                            municipality["region"].get<std::string>() == std::to_string(regionId)) {
                            validMunicipality = true;
                        }
                    }

                    if (!validMunicipality) {
                        std::cout << "\nInvalid municipality ID or municipality not in region " << regionId << ".\n";
                        Meniu::pauseScreen();
                        return;
                    }

                    // Grant access to specific municipality and clear local access
                    user.grantAccess(ElectionLevel::municipal, entityId);
                    user.grantAccess(ElectionLevel::local, 0);
                } else {
                    std::cout << "\nInvalid municipality ID. Must be 0 or positive.\n";
                    Meniu::pauseScreen();
                    return;
                }

                // Save changes
                (*it) = user.toJson();
                if (DataManager::getInstance().saveData("users", users)) {
                    std::cout << "\nUser access updated successfully!\n";
                } else {
                    std::cout << "\nError updating user access.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "\nError: " << e.what() << "\n";
            }

            Meniu::pauseScreen();
            return;
        }
        // For local level, check municipal access first
        else if (electionLevel == ElectionLevel::local) {
            const auto municipalIt = access.find(ElectionLevel::municipal);
            if (municipalIt == access.end() || municipalIt->second.empty()) {
                std::cout << "\nYou must have access to a specific municipality before managing local access.\n";
                Meniu::pauseScreen();
                return;
            }

            // Get the municipality ID the user has access to
            const int municipalityId = *municipalIt->second.begin();

            // Load localities data
            json locations = DataManager::getInstance().loadData("locations");

            std::cout << "\nAvailable Localities in Municipality " << municipalityId << ":\n";
            if (locations.contains("localities")) {
                bool foundAny = false;
                for (const auto& [id, locality] : locations["localities"].items()) {
                    if (locality["municipality"].get<std::string>() == std::to_string(municipalityId)) {
                        std::cout << "ID: " << id << " - "
                                 << locality["name"].get<std::string>() << "\n";
                        foundAny = true;
                    }
                }
                if (!foundAny) {
                    std::cout << "No localities found in this municipality.\n";
                }
            }

            std::cout << "\nEnter locality ID:\n"
                      << "   0: No access to localities\n"
                      << "  >0: Select specific locality from this municipality\n"
                      << "Enter choice: ";
            std::string entityIdStr;
            std::getline(std::cin, entityIdStr);

            try {
                const int entityId = std::stoi(entityIdStr);

                if (entityId == 0) {
                    // Remove access to localities
                    user.grantAccess(ElectionLevel::local, 0);
                } else if (entityId > 0) {
                    // Verify the locality belongs to the user's municipality
                    bool validLocality = false;
                    if (locations.contains("localities")) {
                        const auto& locality = locations["localities"][std::to_string(entityId)];
                        if (!locality.is_null() &&
                            locality["municipality"].get<std::string>() == std::to_string(municipalityId)) {
                            validLocality = true;
                        }
                    }

                    if (!validLocality) {
                        std::cout << "\nInvalid locality ID or locality not in municipality " << municipalityId << ".\n";
                        Meniu::pauseScreen();
                        return;
                    }

                    // Grant access to specific locality
                    user.grantAccess(ElectionLevel::local, entityId);
                } else {
                    std::cout << "\nInvalid locality ID. Must be 0 or positive.\n";
                    Meniu::pauseScreen();
                    return;
                }

                // Save changes
                (*it) = user.toJson();
                if (DataManager::getInstance().saveData("users", users)) {
                    std::cout << "\nUser access updated successfully!\n";
                } else {
                    std::cout << "\nError updating user access.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "\nError: " << e.what() << "\n";
            }

            Meniu::pauseScreen();
            return;
        }
        // For non-government level
        else if (electionLevel == ElectionLevel::nonGovernment) {
            // Load non-government data
            json locations = DataManager::getInstance().loadData("locations");

            std::cout << "\nAvailable Non-Government Entities:\n";
            if (locations.contains("nonGovernment")) {
                for (const auto& [id, entity] : locations["nonGovernment"].items()) {
                    std::cout << "ID: " << id << " - "
                             << entity["name"].get<std::string>() << "\n";
                }
            }

            std::cout << "\nEnter entity ID:\n"
                      << "   0: Clear all non-government access\n"
                      << "  >0: Add specific entity to access list\n"
                      << "Enter choice: ";
            std::string entityIdStr;
            std::getline(std::cin, entityIdStr);

            try {
                const int entityId = std::stoi(entityIdStr);

                if (entityId == 0) {
                    // Clear all non-government access
                    user.grantAccess(ElectionLevel::nonGovernment, 0);
                } else if (entityId > 0) {
                    // Verify the entity exists
                    bool validEntity = false;
                    if (locations.contains("nonGovernment")) {
                        const auto& entity = locations["nonGovernment"][std::to_string(entityId)];
                        if (!entity.is_null()) {
                            validEntity = true;
                        }
                    }

                    if (!validEntity) {
                        std::cout << "\nInvalid entity ID.\n";
                        Meniu::pauseScreen();
                        return;
                    }

                    // Add the entity to the access list (don't clear existing ones)
                    const auto& currentAccess = user.getRestrictedAccess();
                    const auto nonGovIt = currentAccess.find(ElectionLevel::nonGovernment);
                    std::set<int> newAccess;
                    if (nonGovIt != currentAccess.end()) {
                        newAccess = nonGovIt->second;
                    }
                    newAccess.insert(entityId);

                    // Update the access
                    user.grantAccess(ElectionLevel::nonGovernment, -1);  // Clear first
                    for (const int id : newAccess) {
                        user.grantAccess(ElectionLevel::nonGovernment, id);
                    }
                } else {
                    std::cout << "\nInvalid entity ID. Must be 0 or positive.\n";
                    Meniu::pauseScreen();
                    return;
                }

                // Save changes
                (*it) = user.toJson();
                if (DataManager::getInstance().saveData("users", users)) {
                    std::cout << "\nUser access updated successfully!\n";
                } else {
                    std::cout << "\nError updating user access.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "\nError: " << e.what() << "\n";
            }

            Meniu::pauseScreen();
            return;
        }
    } catch (const std::exception& e) {
        std::cout << "\nError: " << e.what() << "\n";
    }

    
    Meniu::pauseScreen();
}

bool UserService::validateUsername(const std::string& username) {
    // Username must be between 3 and 20 characters
    if (username.length() < 3 || username.length() > 20) {
        std::cout << "Username must be between 3 and 20 characters.\n";
        return false;
    }

    // Username can only contain letters, numbers, and underscores
    for (char c : username) {
        if (!std::isalnum(c) && c != '_') {
            std::cout << "Username can only contain letters, numbers, and underscores.\n";
            return false;
        }
    }

    // Username must start with a letter
    if (!std::isalpha(username[0])) {
        std::cout << "Username must start with a letter.\n";
        return false;
    }

    return true;
}

bool UserService::validatePassword(const std::string& password) {
    // Password must be at least 8 characters
    if (password.length() < 8) {
        std::cout << "Password must be at least 8 characters long.\n";
        return false;
    }

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for (char c : password) {
        if (std::isupper(c)) hasUpper = true;
        else if (std::islower(c)) hasLower = true;
        else if (std::isdigit(c)) hasDigit = true;
        else hasSpecial = true;
    }

    if (!hasUpper) {
        std::cout << "Password must contain at least one uppercase letter.\n";
        return false;
    }
    if (!hasLower) {
        std::cout << "Password must contain at least one lowercase letter.\n";
        return false;
    }
    if (!hasDigit) {
        std::cout << "Password must contain at least one number.\n";
        return false;
    }
    if (!hasSpecial) {
        std::cout << "Password must contain at least one special character.\n";
        return false;
    }

    return true;
}

bool UserService::usernameExists(const std::string& username) {
    try {
        json users = DataManager::getInstance().loadData("users");
        for (const auto& user : users) {
            if (user["username"] == username) {
                return true;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error checking username: " << e.what() << std::endl;
    }
    return false;
} 