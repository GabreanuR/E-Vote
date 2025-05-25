#include "../../include/Services/LocationService.h"
#include "../../include/Utils/DataManager.h"
#include "../../include/Models/User.h"
#include "../../include/Utils/Types.h"
#include "../../include/Menus/Meniu.h"
#include <iostream>

LocationService* LocationService::instance = nullptr;

LocationService& LocationService::getInstance() {
    if (instance == nullptr) {
        instance = new LocationService();
    }
    return *instance;
}

void LocationService::addRegion() {
    Meniu::clearScreen();
    std::cout << "===== Add New Region =====\n\n";

    // Load current data
    json data = DataManager::getInstance().loadData("locations");
    json counters = DataManager::getInstance().loadData("counters");
    
    // Get next region ID
    const int regionId = counters.value("regionId", 1);
    
    // Get region name
    std::string name;
    std::cout << "Enter region name (empty to go back): ";
    std::getline(std::cin, name);
    if (name.empty()) {
        return;
    }
    
    // Add new region
    data["regions"][std::to_string(regionId)] = {
        {"name", name},
        {"municipalities", json::array()}
    };
    
    // Update counter
    counters["regionId"] = regionId + 1;
    
    // Save changes
    if (DataManager::getInstance().saveData("locations", data) &&
        DataManager::getInstance().saveData("counters", counters)) {
        // Update admin access for the new region
        User::updateAdminAccess(ElectionLevel::regional, regionId);
        std::cout << "\nRegion added successfully!\n";
    } else {
        std::cout << "\nError adding region.\n";
    }
    
    Meniu::pauseScreen();
}

void LocationService::addMunicipality() {
    Meniu::clearScreen();
    std::cout << "===== Add New Municipality =====\n\n";

    // Load current data
    json data = DataManager::getInstance().loadData("locations");
    json counters = DataManager::getInstance().loadData("counters");
    
    // Get next municipality ID
    const int municipalityId = counters.value("municipalityId", 1);
    
    // Get municipality name
    std::string name;
    std::cout << "Enter municipality name (empty to go back): ";
    std::getline(std::cin, name);
    if (name.empty()) {
        return;
    }
    
    // Display available regions
    std::cout << "\nAvailable regions:\n";
    for (const auto& [id, region] : data["regions"].items()) {
        std::cout << id << ": " << region["name"] << "\n";
    }
    
    // Get region ID
    std::string regionId;
    std::cout << "\nEnter region ID (empty to go back): ";
    std::getline(std::cin, regionId);
    if (regionId.empty()) {
        return;
    }
    
    // Validate region ID
    if (!data["regions"].contains(regionId)) {
        std::cout << "\nInvalid region ID.\n";
        Meniu::pauseScreen();
        return;
    }
    
    // Add new municipality
    data["municipalities"][std::to_string(municipalityId)] = {
        {"name", name},
        {"region", regionId},
        {"localities", json::array()}
    };
    
    // Add municipality to region
    data["regions"][regionId]["municipalities"].push_back(std::to_string(municipalityId));
    
    // Update counter
    counters["municipalityId"] = municipalityId + 1;
    
    // Save changes
    if (DataManager::getInstance().saveData("locations", data) &&
        DataManager::getInstance().saveData("counters", counters)) {
        // Update admin access for the new municipality
        User::updateAdminAccess(ElectionLevel::municipal, municipalityId);
        std::cout << "\nMunicipality added successfully!\n";
    } else {
        std::cout << "\nError adding municipality.\n";
    }
    
    Meniu::pauseScreen();
}

void LocationService::addLocality() {
    Meniu::clearScreen();
    std::cout << "===== Add New Locality =====\n\n";

    // Load current data
    json data = DataManager::getInstance().loadData("locations");
    json counters = DataManager::getInstance().loadData("counters");
    
    // Get next locality ID
    const int localityId = counters.value("localityId", 1);
    
    // Get locality name
    std::string name;
    std::cout << "Enter locality name (empty to go back): ";
    std::getline(std::cin, name);
    if (name.empty()) {
        return;
    }
    
    // Display available municipalities
    std::cout << "\nAvailable municipalities:\n";
    for (const auto& [id, municipality] : data["municipalities"].items()) {
        std::cout << id << ": " << municipality["name"] << "\n";
    }
    
    // Get municipality ID
    std::string municipalityId;
    std::cout << "\nEnter municipality ID (empty to go back): ";
    std::getline(std::cin, municipalityId);
    if (municipalityId.empty()) {
        return;
    }
    
    // Validate municipality ID
    if (!data["municipalities"].contains(municipalityId)) {
        std::cout << "\nInvalid municipality ID.\n";
        Meniu::pauseScreen();
        return;
    }
    
    // Add new locality
    data["localities"][std::to_string(localityId)] = {
        {"name", name},
        {"municipality", municipalityId}
    };
    
    // Add locality to municipality
    data["municipalities"][municipalityId]["localities"].push_back(std::to_string(localityId));
    
    // Update counter
    counters["localityId"] = localityId + 1;
    
    // Save changes
    if (DataManager::getInstance().saveData("locations", data) &&
        DataManager::getInstance().saveData("counters", counters)) {
        // Update admin access for the new locality
        User::updateAdminAccess(ElectionLevel::local, localityId);
        std::cout << "\nLocality added successfully!\n";
    } else {
        std::cout << "\nError adding locality.\n";
    }
    
    Meniu::pauseScreen();
}

void LocationService::addNonGovernment() {
    Meniu::clearScreen();
    std::cout << "===== Add New Non-Government Entity =====\n\n";

    // Load current data
    json data = DataManager::getInstance().loadData("locations");
    json counters = DataManager::getInstance().loadData("counters");
    
    // Get next non-government ID
    const int nonGovId = counters.value("nonGovernmentId", 1);
    
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
    
    // Add new non-government entity
    data["nonGovernment"][std::to_string(nonGovId)] = {
        {"name", name},
        {"type", type}
    };
    
    // Update counter
    counters["nonGovernmentId"] = nonGovId + 1;
    
    // Save changes
    if (DataManager::getInstance().saveData("locations", data) &&
        DataManager::getInstance().saveData("counters", counters)) {
        // Update admin access for the new non-government entity
        User::updateAdminAccess(ElectionLevel::nonGovernment, nonGovId);
        std::cout << "\nNon-government entity added successfully!\n";
    } else {
        std::cout << "\nError adding non-government entity.\n";
    }
    
    Meniu::pauseScreen();
}

void LocationService::displayLocations() {
    Meniu::clearScreen();
    std::cout << "===== All Locations =====\n\n";

    // Load current data
    const json data = DataManager::getInstance().loadData("locations");
    
    // Display regions and their municipalities
    std::cout << "Regions:\n";
    for (const auto& [id, region] : data["regions"].items()) {
        std::cout << id << ": " << region["name"] << "\n";
        std::cout << "  Municipalities: ";
        if (region["municipalities"].empty()) {
            std::cout << "None\n";
        } else {
            for (const auto& munId : region["municipalities"]) {
                std::cout << munId << " (" << data["municipalities"][munId]["name"] << ") ";
            }
            std::cout << "\n";
        }
    }
    
    // Display municipalities and their localities
    std::cout << "\nMunicipalities:\n";
    for (const auto& [id, municipality] : data["municipalities"].items()) {
        std::cout << id << ": " << municipality["name"] << " (Region: " << municipality["region"] << ")\n";
        std::cout << "  Localities: ";
        if (municipality["localities"].empty()) {
            std::cout << "None\n";
        } else {
            for (const auto& locId : municipality["localities"]) {
                std::cout << locId << " (" << data["localities"][locId]["name"] << ") ";
            }
            std::cout << "\n";
        }
    }
    
    // Display non-government entities
    std::cout << "\nNon-Government Entities:\n";
    for (const auto& [id, entity] : data["nonGovernment"].items()) {
        std::cout << id << ": " << entity["name"] << " (" << entity["type"] << ")\n";
    }
    
    Meniu::pauseScreen();
} 