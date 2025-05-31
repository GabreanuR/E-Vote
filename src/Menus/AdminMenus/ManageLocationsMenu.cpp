#include <iostream>
#include "../include/Menus/AdminMenus/ManageLocationsMenu.h"
#include "../include/Services/LocationService.h"
#include "../include/Models/Location.h"
#include "../include/Menus/Meniu.h"
#include <vector>
#include <memory>

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
                std::cout << "\nDisplaying Locations...\n\n";
                pauseScreen();
                displayLocations();
                break;
            case 6:
                std::cout << "\nGoing Back...\n\n";
                pauseScreen();
                return;
            default:
                break;
        }
    }
}

void ManageLocationsMenu::addRegion() {
    clearScreen();
    std::cout << "===== Add New Region =====\n\n";

    try {
        Region newRegion;
        std::cin >> newRegion;

        if (LocationService::getInstance().addRegion(newRegion.getName())) {
            std::cout << "\nRegion '" << newRegion.getName() << "' added successfully!\n";
        }
    } catch (const UserInputCancelledException &) {
        std::cout << "\nRegion creation cancelled by user.\n";
    }

    pauseScreen();
}

void ManageLocationsMenu::addMunicipality() {
    clearScreen();
    std::cout << "===== Add New Municipality =====\n\n";

    std::cout << "Available regions:\n";
    const auto &regions = LocationService::getInstance().getAllRegions();
    if (regions.empty()) {
        std::cout << "No regions available to assign to. Please add a region first.\n";
        pauseScreen();
        return;
    }
    for (const auto &region: regions) {
        std::cout << region->getId() << ": " << region->getName() << "\n";
    }
    std::cout << "\nEnter details for the new municipality: \n";

    try {
        Municipality newMunicipality;
        std::cin >> newMunicipality;

        if (LocationService::getInstance().addMunicipality(newMunicipality.getName(), newMunicipality.getRegionId())) {
            std::cout << "\nMunicipality '" << newMunicipality.getName() << "' added successfully!\n";
        }
    } catch (const UserInputCancelledException &) {
        std::cout << "\nMunicipality creation cancelled by user.\n";
    }
    pauseScreen();
}

void ManageLocationsMenu::addLocality() {
    clearScreen();
    std::cout << "===== Add New Locality =====\n\n";

    std::cout << "Available municipalities:\n";
    const auto &municipalities = LocationService::getInstance().getAllMunicipalities();
    if (municipalities.empty()) {
        std::cout << "No municipalities available to assign to. Please add a municipality first.\n";
        pauseScreen();
        return;
    }
    for (const auto &mun: municipalities) {
        std::cout << mun->getId() << ": " << mun->getName() << "\n";
    }
    std::cout << "\nEnter details for the new locality: \n";

    try {
        Locality newLocality;
        std::cin >> newLocality;

        if (LocationService::getInstance().addLocality(newLocality.getName(), newLocality.getMunicipalityId())) {
            std::cout << "\nLocality '" << newLocality.getName() << "' added successfully!\n";
        }
    } catch (const UserInputCancelledException &) {
        std::cout << "\nLocality creation cancelled by user.\n";
    }
    pauseScreen();
}

void ManageLocationsMenu::addNonGovernment() {
    clearScreen();
    std::cout << "===== Add New Non-Government Entity =====\n\n";
    std::cout << "Enter details for the new non-government entity: \n";

    try {
        NonGovernment newNonGov;
        std::cin >> newNonGov;

        if (LocationService::getInstance().addNonGovernment(newNonGov.getName(), newNonGov.getEntityType())) {
            std::cout << "\nNon-government entity '" << newNonGov.getName() << "' added successfully!\n";
        }
    } catch (const UserInputCancelledException &) {
        std::cout << "\nNon-government entity creation cancelled by user.\n";
    }
    pauseScreen();
}

void ManageLocationsMenu::displayLocations() {
    clearScreen();
    std::cout << "===== All Locations =====\n\n";

    std::cout << "--- Regions ---\n";
    if (const auto &regions = LocationService::getInstance().getAllRegions(); regions.empty()) {
        std::cout << "No regions to display.\n";
    } else {
        for (const auto &regionPtr: regions) {
            if (regionPtr) {
                std::cout << *regionPtr << "\n";
            }
        }
    }

    std::cout << "\n--- Municipalities ---\n";
    if (const auto &municipalities = LocationService::getInstance().getAllMunicipalities(); municipalities.empty()) {
        std::cout << "No municipalities to display.\n";
    } else {
        for (const auto &municipalityPtr: municipalities) {
            if (municipalityPtr) {
                std::cout << *municipalityPtr << "\n";
            }
        }
    }

    std::cout << "\n--- Localities ---\n";
    if (const auto &localities = LocationService::getInstance().getAllLocalities(); localities.empty()) {
        std::cout << "No localities to display.\n";
    } else {
        for (const auto &localityPtr: localities) {
            if (localityPtr) {
                std::cout << *localityPtr << "\n";
            }
        }
    }

    std::cout << "\n--- Non-Government Entities ---\n";
    if (const auto &nonGovs = LocationService::getInstance().getAllNonGovernment(); nonGovs.empty()) {
        std::cout << "No non-government entities to display.\n";
    } else {
        for (const auto &nonGovernmentEntityPtr: nonGovs) {
            if (nonGovernmentEntityPtr) {
                std::cout << *nonGovernmentEntityPtr << "\n";
            }
        }
    }
    std::cout << "\n";
    pauseScreen();
}
