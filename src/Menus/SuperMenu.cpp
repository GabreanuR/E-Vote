#include "../include/Menus/SuperMenu.h"
#include "../include/Menus/MainMenu.h"
#include "../include/Menus/Meniu.h"
#include <iostream>

SuperMenu::SuperMenu() : mainMenu(std::make_unique<MainMenu>()) {}

void SuperMenu::printSuperMenuOptions() {
    clearScreen();
    std::cout << "--- Super Menu ---" << std::endl;
    std::cout << "1. Enter Program" << std::endl;
    std::cout << "2. Exit" << std::endl;
}

void SuperMenu::display() {
    bool running = true;
    while (running) {
        printSuperMenuOptions();

        switch (getValidatedInput(1, 2, [this]() { this->printSuperMenuOptions(); })) {
            case 1:
                if (mainMenu) {
                    pauseScreen();
                    mainMenu->display();
                }
                break;
            case 2:
                std::cout << "Exiting program." << std::endl;
                pauseScreen();
                running = false;
                break;
            default:
                std::cerr << "Invalid choice. Please try again." << std::endl;
                pauseScreen();
                break;
        }
    }
} 