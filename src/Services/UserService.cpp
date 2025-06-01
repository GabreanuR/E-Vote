#include "../include/Services/UserService.h"
#include "../include/Utils/DataManager.h"
#include "../include/Models/User.h"
#include "../include/Menus/Meniu.h"
#include "../include/Utils/Types.h"
#include "../include/Services/LocationService.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <sstream>

UserService::UserService() {
    loadUsersFromDataManager();
    ensureAdminUserExists();
}

UserService &UserService::getInstance() {
    static UserService serviceInstance;
    return serviceInstance;
}

void UserService::loadUsersFromDataManager() {
    usersCache.clear();

    if (json usersJsonArray = DataManager::loadData(usersFilePath); usersJsonArray.is_array()) {
        for (const auto &userJson: usersJsonArray) {
            try {
                usersCache.push_back(std::make_shared<User>(userJson));
            } catch (const json::exception &e) {
                std::cerr << "Error parsing user JSON from " << usersFilePath << ": " << e.what() << std::endl;
            } catch (const std::exception &e) {
                std::cerr << "Error loading user from " << usersFilePath << ": " << e.what() << std::endl;
            }
        }
        std::cout << "Loaded " << usersCache.size() << " users from " << usersFilePath << std::endl;
    } else {
        std::cout << "No users data found or data is not an array in " << usersFilePath << ". Initializing empty list."
                << std::endl;
    }
}

bool UserService::isUsernameUnique(const std::string &username, const int currentUserId) const {
    return std::ranges::all_of(usersCache, [&](const auto &user) {
        return !user || user->getUsername() != username || (currentUserId != -1 && user->getId() == currentUserId);
    });
}

int UserService::calculateNextUserId() const {
    if (usersCache.empty()) {
        return superAdmin + 1;
    }
    int maxId = 0;
    for (const auto &user: usersCache) {
        if (user && user->getId() > maxId) {
            maxId = user->getId();
        }
    }
    return std::max(maxId + 1, superAdmin + 1);
}

void UserService::handleVoterRegionalAccess(const std::shared_ptr<User> &user, const LocationService &locService) {
    Meniu::clearScreen();
    std::cout << "--- Managing Regional Access for Voter: " << user->getUsername() << " ---\n" << std::endl;
    std::cout << "Voters can only have access to one specific region." << std::endl;
    std::cout << "Assigning a new region will clear existing regional, municipal, and local access." <<
            std::endl << std::endl;

    std::cout << "Available Regions:" << std::endl;
    const std::vector<int> all_region_ids = locService.getAllLocationIds(ElectionLevel::regional);
    std::vector<std::pair<int, std::string> > available_regions_display;
    int list_idx_disp = 1;
    for (int region_id_val: all_region_ids) {
        std::string name = locService.getLocationName(ElectionLevel::regional, region_id_val);
        available_regions_display.emplace_back(region_id_val, name);
        std::cout << list_idx_disp++ << ". " << name << " (ID: " << region_id_val << ")" << std::endl;
    }

    if (available_regions_display.empty()) {
        std::cout << "No regions available or defined." << std::endl;
    } else {
        std::cout << "0. Clear Regional Access (and sub-levels)" << std::endl;
    }

    std::cout << "Enter choice (number from list, or ID directly, empty to cancel): ";
    std::string region_choice_input;
    std::getline(std::cin, region_choice_input);
    if (region_choice_input.empty()) {
        return;
    }

    int chosenRegionId = -2;
    try {
        if (const int parsed_input_value = std::stoi(region_choice_input); parsed_input_value == 0) {
            chosenRegionId = 0;
        } else if (parsed_input_value > 0 && parsed_input_value <= static_cast<int>(available_regions_display.size())) {
            chosenRegionId = available_regions_display[parsed_input_value - 1].first;
        } else if (parsed_input_value > 0) {
            chosenRegionId = parsed_input_value;
        } else {
            std::cerr << "Invalid choice. Please enter a number from the list, a direct positive ID, or 0." <<
                    std::endl;
        }
    } catch (const std::invalid_argument &) {
        std::cerr << "Invalid input: Not a valid number." << std::endl;
    } catch (const std::out_of_range &) {
        std::cerr << "Invalid input: Number is too large." << std::endl;
    }

    if (chosenRegionId != -2) {
        if (chosenRegionId > 0) {
            bool found = false;
            for (const auto &key: available_regions_display | std::views::keys) {
                if (key == chosenRegionId) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cerr << "Region ID " << chosenRegionId << " not found in available regions." << std::endl;
                chosenRegionId = -2;
            }
        }
        if (chosenRegionId != -2) {
            std::cout << "Updating regional access..." << std::endl;
            user->grantAccess(ElectionLevel::municipal, 0);
            user->grantAccess(ElectionLevel::local, 0);
            user->grantAccess(ElectionLevel::regional, 0);
            if (chosenRegionId > 0) {
                user->grantAccess(ElectionLevel::regional, chosenRegionId);
                std::cout << "Granted access to region: " << locService.getLocationName(
                    ElectionLevel::regional, chosenRegionId) << std::endl;
            } else {
                std::cout << "Cleared regional access (and sub-levels)." << std::endl;
            }
        }
    }
}

void UserService::handleVoterMunicipalAccess(const std::shared_ptr<User> &user, const LocationService &locService) {
    Meniu::clearScreen();
    std::cout << "--- Managing Municipal Access for Voter: " << user->getUsername() << " ---\n" << std::endl;
    std::cout << "Voters can only have access to one specific municipality within their selected region." << std::endl;
    std::cout << "Assigning a new municipality will clear existing municipal and local access." << std::endl <<
            std::endl;

    const auto &accessMap = user->getRestrictedAccess();
    const auto regionalAccessIt = accessMap.find(ElectionLevel::regional);

    if (regionalAccessIt == accessMap.end() || regionalAccessIt->second.empty() || regionalAccessIt->second.size() > 1
        || (*regionalAccessIt->second.begin() == 0) || (*regionalAccessIt->second.begin() == -1)) {
        std::cerr << "Error: Voter must have exactly one specific region selected to manage municipal access." <<
                std::endl;
        std::cerr << "Please set a specific region first." << std::endl;
        return;
    }

    const int selectedRegionId = *regionalAccessIt->second.begin();
    std::cout << "Currently selected Region: " << locService.getLocationName(ElectionLevel::regional, selectedRegionId)
            << " (ID: " << selectedRegionId << ")" << std::endl << std::endl;

    std::cout << "Available Municipalities in this Region:" << std::endl;
    std::vector<std::pair<int, std::string> > available_municipalities_display;
    const std::vector<int> all_mun_ids = locService.getAllLocationIds(ElectionLevel::municipal);
    int list_idx_disp_mun = 1;
    for (int mun_id_val: all_mun_ids) {
        if (const std::shared_ptr<Municipality> mun_ptr = locService.getMunicipality(mun_id_val);
            mun_ptr && mun_ptr->getRegionId() == selectedRegionId) {
            available_municipalities_display.emplace_back(mun_id_val, mun_ptr->getName());
            std::cout << list_idx_disp_mun++ << ". " << mun_ptr->getName() << " (ID: " << mun_id_val << ")" <<
                    std::endl;
        }
    }

    if (available_municipalities_display.empty()) {
        std::cout << "No municipalities available or defined for the selected region." << std::endl;
    } else {
        std::cout << "0. Clear Municipal Access (and clear local access)" << std::endl;
    }

    std::cout << "Enter choice (number from list, or ID directly, empty to cancel): ";
    std::string mun_choice_input;
    std::getline(std::cin, mun_choice_input);
    if (mun_choice_input.empty()) {
        return;
    }

    int chosenMunId = -2;
    try {
        if (const int parsed_input_value = std::stoi(mun_choice_input); parsed_input_value == 0) {
            chosenMunId = 0;
        } else if (parsed_input_value > 0 && parsed_input_value <= static_cast<int>(available_municipalities_display.
                       size())) {
            chosenMunId = available_municipalities_display[parsed_input_value - 1].first;
        } else if (parsed_input_value > 0) {
            chosenMunId = parsed_input_value;
        } else {
            std::cerr << "Invalid choice. Please enter a number from the list, a direct positive ID, or 0." <<
                    std::endl;
        }
    } catch (const std::invalid_argument &) {
        std::cerr << "Invalid input: Not a valid number." << std::endl;
    } catch (const std::out_of_range &) {
        std::cerr << "Invalid input: Number is too large." << std::endl;
    }

    if (chosenMunId != -2) {
        if (chosenMunId > 0) {
            bool found_in_list = false;
            for (const auto &key: available_municipalities_display | std::views::keys) {
                if (key == chosenMunId) {
                    found_in_list = true;
                    break;
                }
            }
            if (!found_in_list) {
                std::cerr << "Error: Municipality ID " << chosenMunId << " is not available in the selected region." <<
                        std::endl;
                chosenMunId = -2;
            }
        }
        if (chosenMunId != -2) {
            std::cout << "Updating municipal access..." << std::endl;
            user->grantAccess(ElectionLevel::local, 0);
            user->grantAccess(ElectionLevel::municipal, 0);
            if (chosenMunId > 0) {
                user->grantAccess(ElectionLevel::municipal, chosenMunId);
                std::cout << "Granted access to municipality: " << locService.getLocationName(
                    ElectionLevel::municipal, chosenMunId) << std::endl;
            } else {
                std::cout << "Cleared municipal access (and local access)." << std::endl;
            }
        }
    }
}

void UserService::handleVoterLocalAccess(const std::shared_ptr<User> &user, const LocationService &locService) {
    Meniu::clearScreen();
    std::cout << "--- Managing Local Access for Voter: " << user->getUsername() << " ---\n" << std::endl;
    std::cout << "Voters can only have access to one specific locality within their selected municipality." << std::endl
            << std::endl;

    const auto &accessMap = user->getRestrictedAccess();
    const auto municipalAccessIt = accessMap.find(ElectionLevel::municipal);
    if (municipalAccessIt == accessMap.end() || municipalAccessIt->second.empty() || municipalAccessIt->second.size() >
        1 || (*municipalAccessIt->second.begin() == 0) || (*municipalAccessIt->second.begin() == -1)) {
        std::cerr << "Error: Voter must have exactly one specific municipality selected to manage local access." <<
                std::endl;
        std::cerr << "Please set a specific municipality first." << std::endl;
        return;
    }

    const int selectedMunId = *municipalAccessIt->second.begin();
    std::cout << "Currently selected Municipality: " << locService.
            getLocationName(ElectionLevel::municipal, selectedMunId) << " (ID: " << selectedMunId << ")" << std::endl <<
            std::endl;

    std::cout << "Available Localities in this Municipality:" << std::endl;
    std::vector<std::pair<int, std::string> > available_localities_display;
    const std::vector<int> all_loc_ids = locService.getAllLocationIds(ElectionLevel::local);
    int list_idx_disp_loc = 1;
    for (int loc_id_val: all_loc_ids) {
        if (const std::shared_ptr<Locality> loc_ptr = locService.getLocality(loc_id_val);
            loc_ptr && loc_ptr->getMunicipalityId() == selectedMunId) {
            available_localities_display.emplace_back(loc_id_val, loc_ptr->getName());
            std::cout << list_idx_disp_loc++ << ". " << loc_ptr->getName() << " (ID: " << loc_id_val << ")" <<
                    std::endl;
        }
    }

    if (available_localities_display.empty()) {
        std::cout << "No localities available or defined for the selected municipality." << std::endl;
    } else {
        std::cout << "0. Clear Local Access" << std::endl;
    }

    std::cout << "Enter choice (number from list, or ID directly, empty to cancel): ";
    std::string loc_choice_input;
    std::getline(std::cin, loc_choice_input);
    if (loc_choice_input.empty()) {
        return;
    }

    int chosenLocId = -2;
    try {
        if (const int parsed_input_value = std::stoi(loc_choice_input); parsed_input_value == 0) {
            chosenLocId = 0;
        } else if (parsed_input_value > 0 && parsed_input_value <= static_cast<int>(available_localities_display.
                       size())) {
            chosenLocId = available_localities_display[parsed_input_value - 1].first;
        } else if (parsed_input_value > 0) {
            chosenLocId = parsed_input_value;
        } else {
            std::cerr << "Invalid choice. Please enter a number from the list, a direct positive ID, or 0." <<
                    std::endl;
        }
    } catch (const std::invalid_argument &) {
        std::cerr << "Invalid input: Not a valid number." << std::endl;
    } catch (const std::out_of_range &) {
        std::cerr << "Invalid input: Number is too large." << std::endl;
    }

    if (chosenLocId != -2) {
        if (chosenLocId > 0) {
            bool found_in_list = false;
            for (const auto &key: available_localities_display | std::views::keys) {
                if (key == chosenLocId) {
                    found_in_list = true;
                    break;
                }
            }
            if (!found_in_list) {
                std::cerr << "Error: Locality ID " << chosenLocId << " is not available in the selected municipality."
                        << std::endl;
                chosenLocId = -2;
            }
        }
        if (chosenLocId != -2) {
            std::cout << "Updating local access..." << std::endl;
            user->grantAccess(ElectionLevel::local, 0);
            if (chosenLocId > 0) {
                user->grantAccess(ElectionLevel::local, chosenLocId);
                std::cout << "Granted access to locality: " << locService.getLocationName(
                    ElectionLevel::local, chosenLocId) << std::endl;
            } else {
                std::cout << "Cleared local access." << std::endl;
            }
        }
    }
}

void UserService::handleGenericLevelAccess(const std::shared_ptr<User> &user, const LocationService &locService,
                                           const ElectionLevel chosenLevel) {
    Meniu::clearScreen();
    std::cout << "--- Managing " << electionLevelToString(chosenLevel) << " Access for " << user->getUsername() <<
            " ---\n" << std::endl;

    const auto &accessMap = user->getRestrictedAccess();
    const auto currentAccessIt = accessMap.find(chosenLevel);
    std::cout << "Current " << electionLevelToString(chosenLevel) << " Access: ";

    if (currentAccessIt == accessMap.end()) {
        if (user->getType() == UserType::admin) {
            std::cout << "(Default - Full access due to admin privileges)";
        } else if (chosenLevel == ElectionLevel::national) {
            std::cout << "Full National Access (Default)";
        } else {
            std::cout << "(None specific - default is no access unless National allows)";
        }
    } else if (currentAccessIt->second.contains(-1)) {
        std::cout << "All entities at this level (General Access Grant)";
    } else if (currentAccessIt->second.contains(0) && currentAccessIt->second.size() == 1) {
        if (user->getType() == UserType::admin) {
            std::cout << "(Default - Full access due to admin privileges or National setting being cleared to default)";
        } else if (chosenLevel == ElectionLevel::national) {
            std::cout << "Full National Access (Explicitly set or cleared to default)";
        } else {
            std::cout << "(None specific - access explicitly cleared for this level)";
        }
    } else if (currentAccessIt->second.empty()) {
        std::cout << "(None specific - no entities assigned)";
    } else {
        bool first = true;
        for (const int id: currentAccessIt->second) {
            if (!first) std::cout << ", ";
            std::cout << locService.getLocationName(chosenLevel, id) << " (ID:" << id << ")";
            first = false;
        }
        if (first) {
            std::cout << "(Error: Access set is present but appears empty of valid IDs)";
        }
    }
    std::cout << std::endl << std::endl;

    if (chosenLevel != ElectionLevel::national || user->getType() == UserType::admin) {
        if ((user->getType() == UserType::admin) || (
                user->getType() == UserType::voter && chosenLevel == ElectionLevel::non_governmental)) {
            std::cout << "Available " << electionLevelToString(chosenLevel) << " entities:" << std::endl;
            if (const std::vector<int> all_entity_ids_for_level = locService.getAllLocationIds(chosenLevel);
                all_entity_ids_for_level.empty()) {
                std::cout << "No specific entities listed or defined for this level." << std::endl;
            } else {
                int list_idx_gen = 1;
                for (const int entity_id_val: all_entity_ids_for_level) {
                    std::cout << list_idx_gen++ << ". " << locService.getLocationName(chosenLevel, entity_id_val) <<
                            " (ID: " << entity_id_val << ")" << std::endl;
                }
            }
            std::cout << std::endl;
        }
    }

    std::cout << "Actions:" << std::endl;
    if ((user->getType() == UserType::admin) || (
            user->getType() == UserType::voter && chosenLevel == ElectionLevel::non_governmental)) {
        std::cout << "  g <ID>  - Grant access to specific ID (e.g., g 101)" << std::endl;
        std::cout << "  r <ID>  - Revoke access from specific ID (e.g., r 101)" << std::endl;
    }
    std::cout << "  ga      - Grant All access at this level (for Admins, or National/Non-Gov for Voters)" << std::endl;
    std::cout << "  ca      - Clear All specific access at this level (resets to default for level)" << std::endl;
    std::cout << "  b       - Back to level selection" << std::endl;
    std::cout << "Enter action: ";
    std::string action_input;
    std::getline(std::cin, action_input);

    std::string command_str;
    int entityIdParam = 0;
    std::stringstream ss_action(action_input);
    ss_action >> command_str;
    if (command_str == "g" || command_str == "r") {
        if (!(ss_action >> entityIdParam)) {
            std::cerr << "Error: Missing or invalid ID for grant/revoke action." << std::endl;
            command_str = "";
        } else if (entityIdParam <= 0) {
            std::cerr <<
                    "Error: Entity ID for grant/revoke must be positive. Use 'ga' or 'ca' for general or cleared access."
                    << std::endl;
            command_str = "";
        }
    }

    if (command_str == "b" || command_str.empty()) {
    } else if (command_str == "ga") {
        if (user->getType() == UserType::voter && !(
                chosenLevel == ElectionLevel::national || chosenLevel == ElectionLevel::non_governmental)) {
            std::cerr << "Voters can only be granted 'All' access for National or Non-Governmental levels." <<
                    std::endl;
        } else if (user->grantAccess(chosenLevel, -1)) {
            std::cout << "Granted full access at " << electionLevelToString(chosenLevel) << " level." << std::endl;
        } else {
            std::cout << "Full access at " << electionLevelToString(chosenLevel) <<
                    " level was already set or no change made." << std::endl;
        }
    } else if (command_str == "ca") {
        if (user->grantAccess(chosenLevel, 0)) {
            std::cout << "Cleared all specific access at " << electionLevelToString(chosenLevel) <<
                    " level. User will have default access for this level." << std::endl;
        } else {
            std::cout << "Access at " << electionLevelToString(chosenLevel) <<
                    " level was already cleared or no change made." << std::endl;
        }
    } else if (command_str == "g" && entityIdParam > 0) {
        if (user->getType() == UserType::voter && !(
                chosenLevel == ElectionLevel::national || chosenLevel == ElectionLevel::non_governmental)) {
            std::cerr <<
                    "Voters cannot be granted specific ID access at Regional, Municipal, or Local levels here. Use the dedicated menus."
                    << std::endl;
        } else if (locService.getLocationName(chosenLevel, entityIdParam).rfind("Unknown", 0) == 0 && chosenLevel !=
                   ElectionLevel::national) {
            std::cerr << "Error: Entity ID " << entityIdParam << " does not seem to be a valid/known entity for level "
                    << electionLevelToString(chosenLevel) << ". Grant aborted." << std::endl;
        } else if (user->grantAccess(chosenLevel, entityIdParam)) {
            std::cout << "Granted access to ID " << entityIdParam << " (" << locService.
                    getLocationName(chosenLevel, entityIdParam) << ") at " << electionLevelToString(chosenLevel) << "."
                    << std::endl;
        } else {
            std::cout << "Failed to grant access to ID " << entityIdParam <<
                    " (check if ID is valid or already granted)." << std::endl;
        }
    } else if (command_str == "r" && entityIdParam > 0) {
        if (user->getType() == UserType::voter && !(
                chosenLevel == ElectionLevel::national || chosenLevel == ElectionLevel::non_governmental)) {
            std::cerr <<
                    "Voters cannot have specific ID access revoked at Regional, Municipal, or Local levels here. Use the dedicated menus or 'ca'."
                    << std::endl;
        } else if (user->revokeAccess(chosenLevel, entityIdParam)) {
            std::cout << "Revoked access from ID " << entityIdParam << " (" << locService.
                    getLocationName(chosenLevel, entityIdParam) << ") at " << electionLevelToString(chosenLevel) << "."
                    << std::endl;
        } else {
            std::cout << "Failed to revoke access from ID " << entityIdParam <<
                    " (ID might not exist in user specific access list or was already revoked)." << std::endl;
        }
    } else if (!command_str.empty()) {
        std::cerr << "Invalid action command: '" << command_str << "'." << std::endl;
    }
}

void UserService::saveUsersToDataManager() const {
    json usersJsonArray = json::array();
    for (const auto &userPtr: usersCache) {
        if (userPtr) {
            usersJsonArray.push_back(userPtr->toJson());
        }
    }
    if (DataManager::saveData(usersFilePath, usersJsonArray)) {
        std::cout << "Successfully saved " << usersCache.size() << " users to " << usersFilePath << std::endl;
    } else {
        std::cerr << "Failed to save users to " << usersFilePath << std::endl;
    }
}

void UserService::viewUsers() {
    const auto &users = getInstance().getAllUsers();
    if (users.empty()) {
        std::cout << "No users found in the system.\n";
        Meniu::pauseScreen();
        return;
    }
    std::cout << "\n--- List of Users ---\n";
    for (const auto &user: users) {
        if (user) {
            std::cout << "ID: " << user->getId() << ", Username: " << user->getUsername()
                    << ", Type: " << (user->getType() == UserType::admin ? "Admin" : "Voter")
                    << ", Status: " << (user->isDisabled() ? "Disabled" : "Active") << std::endl;
        }
    }
    std::cout << "---------------------\n";
    Meniu::pauseScreen();
}

void UserService::displayUserDetails(const User &user) {
    std::cout << "\n--- User Details ---\n";
    std::cout << "ID: " << user.getId() << std::endl;
    std::cout << "Username: " << user.getUsername() << std::endl;
    std::cout << "Type: " << (user.getType() == UserType::admin ? "Admin" : "Voter") << std::endl;
    std::cout << "Status: " << (user.isDisabled() ? "Disabled" : "Active") << std::endl;

    std::cout << "Access Rights:\n";
    if (const auto &accessMap = user.getRestrictedAccess(); accessMap.empty() && user.getType() != UserType::admin) {
        std::cout << "  User has no specific access rights defined.\n";
    } else if (user.getType() == UserType::admin) {
        std::cout << "  Admin user has full access to all levels and entities.\n";
    } else {
        for (const auto &[fst, snd]: accessMap) {
            std::string levelName = electionLevelToString(fst);
            std::cout << "  Level: " << levelName << std::endl;
            if (snd.empty()) {
                std::cout << "    Access: All entities at this level\n";
            } else {
                std::cout << "    Access restricted to entities (IDs):\n";
                for (const int entityId: snd) {
                    std::string entityName = LocationService::getInstance().getLocationName(fst, entityId);
                    std::cout << "      - ID: " << entityId << " (" << entityName << ")\n";
                }
            }
        }
    }
    std::cout << "--------------------\n";
}

void UserService::addUser() {
    Meniu::clearScreen();
    std::string username, password, choice_str;
    UserType type_val;
    std::cout << "--- Add New User ---" << std::endl;
    std::cout << "(Press Enter at any input to cancel)" << std::endl << std::endl;

    std::cout << "Enter username (min 3 chars): ";
    std::getline(std::cin, username);
    if (username.empty()) {
        std::cout << "Cancelled adding user." << std::endl;
        Meniu::pauseScreen();
        return;
    }

    std::cout << "Enter password: ";
    std::getline(std::cin, password);
    if (password.empty()) {
        std::cout << "Cancelled adding user." << std::endl;
        Meniu::pauseScreen();
        return;
    }

    std::cout << "User Type:" << std::endl;
    std::cout << "  1. Voter" << std::endl;
    std::cout << "  2. Admin" << std::endl;
    std::cout << "Enter choice (1 or 2): ";
    std::getline(std::cin, choice_str);

    if (choice_str.empty()) {
        std::cout << "Cancelled adding user." << std::endl;
        Meniu::pauseScreen();
        return;
    }

    if (choice_str == "1") {
        type_val = UserType::voter;
    } else if (choice_str == "2") {
        type_val = UserType::admin;
    } else {
        std::cerr << "Invalid choice for user type. Please enter 1 or 2." << std::endl;
        Meniu::pauseScreen();
        return;
    }

    try {
        getInstance().createUser(username, password, type_val);
    } catch (const std::exception &e) {
        std::cerr << "Error adding user: " << e.what() << std::endl;
    }
    Meniu::pauseScreen();
}

void UserService::toggleUserStatus() {
    std::cout << "--- Toggle User Status ---\n" << std::endl;
    viewUsers();
    std::cout << "Enter user ID to enable/disable (empty to cancel): ";
    std::string id_str;
    std::getline(std::cin, id_str);
    if (id_str.empty()) {
        std::cout << "Cancelled." << std::endl;
        Meniu::pauseScreen();
        return;
    }

    try {
        if (const int userIdToToggle = std::stoi(id_str); getInstance().toggleUserStatusInstance(userIdToToggle)) {
        }
        Meniu::pauseScreen();
    } catch (const std::invalid_argument &) {
        std::cerr << "Invalid input: Not a valid number for user ID." << std::endl;
        Meniu::pauseScreen();
    } catch (const std::out_of_range &) {
        std::cerr << "Invalid input: User ID number too large." << std::endl;
        Meniu::pauseScreen();
    }
}

void UserService::deleteUser() {
    std::cout << "--- Delete User ---\n" << std::endl;
    viewUsers();
    std::cout << "Enter user ID to delete (empty to cancel): ";
    std::string id_str;
    std::getline(std::cin, id_str);
    if (id_str.empty()) {
        std::cout << "Cancelled." << std::endl;
        Meniu::pauseScreen();
        return;
    }

    try {
        const int userIdToDelete = std::stoi(id_str);
        std::cout << "Are you sure you want to delete user ID " << userIdToDelete << "? (y/n): ";
        std::string confirm_str;
        std::getline(std::cin, confirm_str);

        if (confirm_str == "y" || confirm_str == "Y") {
            getInstance().deleteUserInstance(userIdToDelete);
        } else {
            std::cout << "Deletion cancelled." << std::endl;
        }
    } catch (const std::invalid_argument &) {
        std::cerr << "Invalid input: Not a valid number for user ID." << std::endl;
    } catch (const std::out_of_range &) {
        std::cerr << "Invalid input: User ID number too large." << std::endl;
    }
}

void UserService::manageUserAccess() {
    Meniu::clearScreen();
    std::cout << "--- Manage User Access ---\n" << std::endl;

    viewUsers();
    std::cout << "Enter User ID to manage access (empty to cancel): ";
    std::string id_str_user;
    std::getline(std::cin, id_str_user);
    if (id_str_user.empty()) {
        std::cout << "Cancelled." << std::endl;
        Meniu::pauseScreen();
        return;
    }

    int userId;
    try {
        userId = std::stoi(id_str_user);
    } catch (const std::exception &) {
        std::cerr << "Invalid User ID format." << std::endl;
        Meniu::pauseScreen();
        return;
    }

    const auto user = getInstance().getUser(userId);
    if (!user) {
        std::cerr << "User with ID " << userId << " not found." << std::endl;
        Meniu::pauseScreen();
        return;
    }

    const LocationService &locService = LocationService::getInstance();

    if (user->getId() == superAdmin) {
        Meniu::clearScreen();
        std::cout << "--- Managing Access for Super Administrator ---\n"
                << "User: " << user->getUsername() << " (ID: " << user->getId() << ")\n\n"
                << "INFO: The Super Administrator inherently possesses full access to all levels and entities.\n"
                << "Modifications here typically ensure explicit listing of access or grant access to newly.\n"
                << "created entities if not covered by a 'grant all' rule. Revoking specific access.\n"
                << "might not limit capabilities due to overriding privileges.\n\n";
        Meniu::pauseScreen();
    }

    bool managing = true;
    while (managing) {
        Meniu::clearScreen();
        std::cout << "--- Manage Access for: " << user->getUsername() << " ---\n";
        displayUserDetails(*user);

        std::cout << "\nChoose level to manage:\n";
        std::cout << "1. Regional\n";
        std::cout << "2. Municipal\n";
        std::cout << "3. Local\n";
        std::cout << "4. Non-Governmental\n";
        std::cout << "0. Back\n";
        std::cout << "Enter choice: \n";
        std::string level_choice_str;
        std::getline(std::cin, level_choice_str);

        if (level_choice_str == "0") {
            managing = false;
            continue;
        }

        ElectionLevel chosenLevel = ElectionLevel::national;
        bool validLevelChoice = true;
        if (level_choice_str == "1") { chosenLevel = ElectionLevel::regional; } else if (
            level_choice_str == "2") { chosenLevel = ElectionLevel::municipal; } else if (level_choice_str == "3") {
            chosenLevel = ElectionLevel::local;
        } else if (level_choice_str == "4") { chosenLevel = ElectionLevel::non_governmental; } else {
            std::cerr << "Invalid level choice." << std::endl;
            validLevelChoice = false;
        }

        if (validLevelChoice) {
            if (user->getType() == UserType::voter) {
                if (chosenLevel == ElectionLevel::regional) {
                    handleVoterRegionalAccess(user, locService);
                } else if (chosenLevel == ElectionLevel::municipal) {
                    handleVoterMunicipalAccess(user, locService);
                } else if (chosenLevel == ElectionLevel::local) {
                    handleVoterLocalAccess(user, locService);
                } else {
                    handleGenericLevelAccess(user, locService, chosenLevel);
                }
            } else {
                handleGenericLevelAccess(user, locService, chosenLevel);
            }
        }
        Meniu::pauseScreen();
    }
    std::cout << "Finished managing access for " << user->getUsername() << "." << std::endl;
    Meniu::pauseScreen();
}

std::shared_ptr<User> UserService::createUser(const std::string &username, const std::string &password, UserType type) {
    if (type == UserType::admin && username == "admin") {
        if (const auto rootAdmin = getUser(superAdmin); rootAdmin && rootAdmin->getUsername() == username) {
            throw User::UserCreationError(
                "Username 'admin' is reserved for the root administrator (ID: " + std::to_string(
                    superAdmin) + ").");
        }
    }
    if (!isUsernameValid(username)) {
        throw User::UserCreationError("Invalid username. Must be at least 3 characters.");
    }
    if (!isPasswordValid(password)) {
        throw User::UserCreationError("Invalid password. Must be at least 6 characters.");
    }
    if (!isUsernameUnique(username)) {
        throw User::UserCreationError("Username '" + username + "' already exists.");
    }

    int newId = calculateNextUserId();
    while (getUser(newId)) {
        newId++;
    }

    auto newUser = std::make_shared<User>(newId, username, password, type, false);

    if (type == UserType::admin) {
        std::cout << "Setting up default access for new admin: " << newUser->getUsername() << std::endl;
        newUser->grantAccess(ElectionLevel::national, -1);

        const auto &locService = LocationService::getInstance();

        if (const std::vector<int> regional_ids = locService.getAllLocationIds(ElectionLevel::regional); regional_ids.
            empty()) { newUser->grantAccess(ElectionLevel::regional, -1); } else {
            for (const int id: regional_ids) newUser->grantAccess(ElectionLevel::regional, id);
        }

        if (const std::vector<int> municipal_ids = locService.getAllLocationIds(ElectionLevel::municipal); municipal_ids
            .empty()) { newUser->grantAccess(ElectionLevel::municipal, -1); } else {
            for (const int id: municipal_ids) newUser->grantAccess(ElectionLevel::municipal, id);
        }

        if (const std::vector<int> local_ids = locService.getAllLocationIds(ElectionLevel::local); local_ids.empty()) {
            newUser->grantAccess(ElectionLevel::local, -1);
        } else {
            for (const int id: local_ids) newUser->grantAccess(ElectionLevel::local, id);
        }

        if (const std::vector<int> nonGov_ids = locService.getAllLocationIds(ElectionLevel::non_governmental);
            nonGov_ids.empty()) { newUser->grantAccess(ElectionLevel::non_governmental, -1); } else {
            for (const int id: nonGov_ids) newUser->grantAccess(ElectionLevel::non_governmental, id);
        }

        std::cout << "Admin default access setup complete." << std::endl;
    } else if (type == UserType::voter) {
        newUser->grantAccess(ElectionLevel::national, -1);
        std::cout << "Setting up default national access for new voter: " << newUser->getUsername() << std::endl;
    }

    usersCache.push_back(newUser);
    std::ranges::sort(usersCache,
                      [](const auto &a, const auto &b) { return a->getId() < b->getId(); });

    std::cout << "User '" << username << "' (ID: " << newId << ") created successfully." << std::endl;
    return newUser;
}

bool UserService::deleteUserInstance(int userId) {
    if (userId == superAdmin) {
        std::cerr << "Error: The root admin user (ID: " << superAdmin << ") cannot be deleted." <<
                std::endl;
        return false;
    }

    const auto it = std::ranges::find_if(usersCache,
                                         [userId](const std::shared_ptr<User> &u) {
                                             return u && u->getId() == userId;
                                         });

    if (it != usersCache.end()) {
        if ((*it)->getType() == UserType::admin) {
            int adminCount = 0;
            for (const auto &usr: usersCache) {
                if (usr && usr->getType() == UserType::admin) adminCount++;
            }
            if (adminCount <= 1) {
                std::cerr << "Cannot delete the last admin user." << std::endl;
                return false;
            }
        }
        std::cout << "User '" << (*it)->getUsername() << "' (ID: " << userId << ") deleted." << std::endl;
        usersCache.erase(it);
        return true;
    }
    std::cerr << "Delete failed: User with ID " << userId << " not found." << std::endl;
    return false;
}

std::shared_ptr<User> UserService::getUser(int userId) const {
    const auto it = std::ranges::find_if(usersCache,
                                         [userId](const std::shared_ptr<User> &u) {
                                             return u && u->getId() == userId;
                                         });
    return (it != usersCache.end()) ? *it : nullptr; //
}

std::shared_ptr<User> UserService::getUserByUsername(const std::string &username) const {
    const auto it = std::ranges::find_if(usersCache,
                                         [&username](const std::shared_ptr<User> &u) {
                                             return u && u->getUsername() == username;
                                         });
    return (it != usersCache.end()) ? *it : nullptr;
}

const std::vector<std::shared_ptr<User> > &UserService::getAllUsers() const {
    return usersCache;
}

bool UserService::toggleUserStatusInstance(const int userId) const {
    if (userId == superAdmin) {
        std::cerr << "Error: The root admin user (ID: " << superAdmin <<
                ") cannot be disabled or enabled via this method." << std::endl;
        return false;
    }
    if (const auto user = getUser(userId)) {
        if (user->getType() == UserType::admin && !user->isDisabled()) {
            int adminCount = 0;
            int enabledAdminCount = 0;
            for (const auto &u: usersCache) {
                if (u && u->getType() == UserType::admin) {
                    adminCount++;
                    if (!u->isDisabled()) enabledAdminCount++;
                }
            }
            if (enabledAdminCount <= 1) {
                std::cerr << "Cannot disable the last enabled admin user." << std::endl;
                return false;
            }
        }
        user->setDisabled(!user->isDisabled());
        std::cout << "User '" << user->getUsername() << "' status set to: " << (user->isDisabled()
            ? "Disabled"
            : "Enabled") << std::endl;
        return true;
    }
    std::cerr << "Toggle status failed: User ID " << userId << " not found." << std::endl;
    return false;
}

void UserService::grantInitialAccessToSuperAdmin(const ElectionLevel level, const int entityId) const {
    if (const auto rootAdmin = getUser(superAdmin)) {
        if (rootAdmin->getType() == UserType::admin) {
            rootAdmin->grantAccess(level, entityId);
        } else {
            std::cerr << "Error in grantInitialAccessToSuperAdmin: User ID " << superAdmin
                    << " is not an admin. Cannot grant initial access for entity " << entityId << "." << std::endl;
        }
    } else {
        std::cerr << "Error in grantInitialAccessToSuperAdmin: Root admin (ID: " << superAdmin
                << ") not found. Cannot grant initial access for entity " << entityId << "." << std::endl;
    }
}

bool UserService::isUsernameValid(const std::string &username) {
    return !username.empty() && username.length() >= 3;
}

bool UserService::isPasswordValid(const std::string &password) {
    return !password.empty() && password.length() >= 6;
}

void UserService::ensureAdminUserExists() {
    if (const auto rootAdmin = getUser(superAdmin); !rootAdmin) {
        std::cout << "Root admin user (ID: " << superAdmin << ") not found. Creating default root admin."
                << std::endl;
        try {
            std::string defaultAdminUsername = "admin";
            std::string defaultAdminPassword = "admin123";

            if (const auto existingUserByUsername = getUserByUsername(defaultAdminUsername);
                existingUserByUsername && existingUserByUsername->getId() != superAdmin) {
                std::cerr << "Error: Username '" << defaultAdminUsername << "' is taken by another user (ID: "
                        << existingUserByUsername->getId() << "). Cannot create root admin." << std::endl;
                return;
            }

            const auto newUser = std::make_shared<User>(superAdmin, defaultAdminUsername, defaultAdminPassword,
                                                        UserType::admin, false);
            newUser->grantAccess(ElectionLevel::national, -1);

            const auto &locService = LocationService::getInstance();

            if (const std::vector<int> regional_ids = locService.getAllLocationIds(ElectionLevel::regional);
                regional_ids.empty()) { newUser->grantAccess(ElectionLevel::regional, -1); } else {
                for (const int id: regional_ids) newUser->grantAccess(ElectionLevel::regional, id);
            }

            if (const std::vector<int> municipal_ids = locService.getAllLocationIds(ElectionLevel::municipal);
                municipal_ids.empty()) { newUser->grantAccess(ElectionLevel::municipal, -1); } else {
                for (const int id: municipal_ids) newUser->grantAccess(ElectionLevel::municipal, id);
            }

            if (const std::vector<int> local_ids = locService.getAllLocationIds(ElectionLevel::local); local_ids.
                empty()) { newUser->grantAccess(ElectionLevel::local, -1); } else {
                for (const int id: local_ids) newUser->grantAccess(ElectionLevel::local, id);
            }

            if (const std::vector<int> nonGov_ids = locService.getAllLocationIds(ElectionLevel::non_governmental);
                nonGov_ids.empty()) { newUser->grantAccess(ElectionLevel::non_governmental, -1); } else {
                for (const int id: nonGov_ids) newUser->grantAccess(ElectionLevel::non_governmental, id);
            }

            usersCache.push_back(newUser);
            std::ranges::sort(usersCache, [](const auto &a, const auto &b) {
                return a->getId() < b->getId();
            });
            std::cout << "Default root admin user '" << defaultAdminUsername << "' (ID: " << superAdmin <<
                    ") created." << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "An unexpected error occurred while creating default root admin: " << e.what() << std::endl;
        }
    } else {
        if (rootAdmin->getType() != UserType::admin) {
            std::cerr << "User with ID " << superAdmin <<
                    " exists but is NOT an admin. Manual intervention required." << std::endl;
        }
    }
}

std::string UserService::electionLevelToString(const ElectionLevel level) {
    switch (level) {
        case ElectionLevel::national: return "National";
        case ElectionLevel::regional: return "Regional";
        case ElectionLevel::municipal: return "Municipal";
        case ElectionLevel::local: return "Local";
        case ElectionLevel::non_governmental: return "Non-Governmental";
        default: return "Unknown Level";
    }
}
