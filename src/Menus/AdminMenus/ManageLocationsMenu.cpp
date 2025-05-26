#include <iostream>
#include "../include/Menus/AdminMenus/ManageLocationsMenu.h"
#include "../include/Services/LocationService.h"

void ManageLocationsMenu::printMenuText() {
    std::cout << "==================================================\n"
              << "Manage Locations\n"
              << "==================================================\n"
              << "1. Add Region\n"
              << "2. Add Municipality\n"
              << "3. Add Locality\n"
              << "4. Add Non-Government Entity\n"
              << "5. View All Locations\n"
              << "6. Back\n";
}

void ManageLocationsMenu::display() {
    while (true) {
        clearScreen();
        printMenuText();

        switch (getValidatedInput(1, 6, printMenuText)) {
            case 1:
                std::cout << "\nAdding Region...\n\n";
                pauseScreen();
                addRegion();
                break;
            case 2:
                std::cout << "\nAdding Municipality...\n\n";
                pauseScreen();
                addMunicipality();
                break;
            case 3:
                std::cout << "\nAdding Locality...\n\n";
                pauseScreen();
                addLocality();
                break;
            case 4:
                std::cout << "\nAdding NonGovernment Organization...\n\n";
                pauseScreen();
                addNonGovernment();
                break;
            case 5:
                displayLocations();
                break;
            case 6:
                return;
            default:
                break;
        }
    }
}

void ManageLocationsMenu::addRegion() {
    Meniu::clearScreen();
    std::cout << "===== Add New Region =====\n\n";

    std::string name;
    std::cout << "Enter region name (empty to go back): ";
    std::getline(std::cin, name);
    if (name.empty()) {
        return;
    }

    LocationService::getInstance().addRegion(name);
    std::cout << "\nRegion added successfully!\n";
    Meniu::pauseScreen();
}

void ManageLocationsMenu::addMunicipality() {
    Meniu::clearScreen();
    std::cout << "===== Add New Municipality =====\n\n";

    // Get municipality name
    std::string name;
    std::cout << "Enter municipality name (empty to go back): ";
    std::getline(std::cin, name);
    if (name.empty()) {
        return;
    }

    // Display available regions
    std::cout << "\nAvailable regions:\n";
    for (const auto& region : LocationService::getInstance().getAllRegions()) {
        std::cout << region->getId() << ": " << region->getName() << "\n";
    }

    // Get region ID
    std::string regionIdStr;
    std::cout << "\nEnter region ID (empty to go back): ";
    std::getline(std::cin, regionIdStr);
    if (regionIdStr.empty()) {
        return;
    }

    try {
        int regionId = std::stoi(regionIdStr);
        if (LocationService::getInstance().getRegion(regionId)) {
            LocationService::getInstance().addMunicipality(name, regionId);
            std::cout << "\nMunicipality added successfully!\n";
        } else {
            std::cout << "\nInvalid region ID.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "\nInvalid region ID.\n";
    }

    Meniu::pauseScreen();
}

void ManageLocationsMenu::addLocality() {
    Meniu::clearScreen();
    std::cout << "===== Add New Locality =====\n\n";

    // Get locality name
    std::string name;
    std::cout << "Enter locality name (empty to go back): ";
    std::getline(std::cin, name);
    if (name.empty()) {
        return;
    }

    // Display available municipalities
    std::cout << "\nAvailable municipalities:\n";
    for (const auto& municipality : LocationService::getInstance().getAllMunicipalities()) {
        std::cout << municipality->getId() << ": " << municipality->getName() << "\n";
    }

    // Get municipality ID
    std::string municipalityIdStr;
    std::cout << "\nEnter municipality ID (empty to go back): ";
    std::getline(std::cin, municipalityIdStr);
    if (municipalityIdStr.empty()) {
        return;
    }

    try {
        int municipalityId = std::stoi(municipalityIdStr);
        if (LocationService::getInstance().getMunicipality(municipalityId)) {
            LocationService::getInstance().addLocality(name, municipalityId);
            std::cout << "\nLocality added successfully!\n";
        } else {
            std::cout << "\nInvalid municipality ID.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "\nInvalid municipality ID.\n";
    }

    Meniu::pauseScreen();
}

void ManageLocationsMenu::addNonGovernment() {
    Meniu::clearScreen();
    std::cout << "===== Add New Non-Government Entity =====\n\n";

    // Get entity name
    std::string name;
    std::cout << "Enter entity name (empty to go back): ";
    std::getline(std::cin, name);
    if (name.empty()) {
        return;
    }

    // Get entity type
    std::string type;
    std::cout << "Enter entity type (e.g., company, university) (empty to go back): ";
    std::getline(std::cin, type);
    if (type.empty()) {
        return;
    }

    LocationService::getInstance().addNonGovernment(name, type);
    std::cout << "\nNon-government entity added successfully!\n";
    Meniu::pauseScreen();
}

void ManageLocationsMenu::displayLocations() {
    Meniu::clearScreen();
    std::cout << "===== All Locations =====\n\n";

    // Display regions and their municipalities
    std::cout << "Regions:\n";
    for (const auto& region : LocationService::getInstance().getAllRegions()) {
        std::cout << region->getId() << ": " << region->getName() << "\n";
        std::cout << "  Municipalities: ";
        if (region->getMunicipalities().empty()) {
            std::cout << "None\n";
        } else {
            for (const auto& m : region->getMunicipalities()) {
                std::cout << m->getId() << " (" << m->getName() << ") ";
            }
            std::cout << "\n";
        }
    }

    // Display municipalities and their localities
    std::cout << "\nMunicipalities:\n";
    for (const auto& municipality : LocationService::getInstance().getAllMunicipalities()) {
        std::cout << municipality->getId() << ": " << municipality->getName() 
                  << " (Region: " << municipality->getRegionId() << ")\n";
        std::cout << "  Localities: ";
        if (municipality->getLocalities().empty()) {
            std::cout << "None\n";
        } else {
            for (const auto& l : municipality->getLocalities()) {
                std::cout << l->getId() << " (" << l->getName() << ") ";
            }
            std::cout << "\n";
        }
    }

    // Display non-government entities
    std::cout << "\nNon-Government Entities:\n";
    for (const auto& entity : LocationService::getInstance().getAllNonGovernment()) {
        std::cout << entity->getId() << ": " << entity->getName() 
                  << " (" << entity->getEntityType() << ")\n";
    }

    Meniu::pauseScreen();
} 