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
                LocationService::getInstance().addRegion();
                break;
            case 2:
                std::cout << "\nAdding Municipality...\n\n";
                pauseScreen();
                LocationService::getInstance().addMunicipality();
                break;
            case 3:
                std::cout << "\nAdding Locality...\n\n";
                pauseScreen();
                LocationService::getInstance().addLocality();
                break;
            case 4:
                std::cout << "\nAdding NonGovernment Organization...\n\n";
                pauseScreen();
                LocationService::getInstance().addNonGovernment();
                break;
            case 5:
                LocationService::getInstance().displayLocations();
                break;
            case 6:
                return;
            default:
                break;
        }
    }
} 