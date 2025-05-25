#ifndef MANAGEUSERSMENU_H
#define MANAGEUSERSMENU_H

#include "../Meniu.h"
#include "../../Models/User.h"

class ManageUsersMenu final : public Meniu {
public:
    void display() override;

private:
    static void printMenuText();
    static void viewUsers();
    static void displayUserDetails(const User& user);
    static void addUser();
    static void toggleUserStatus();
    static void deleteUser();
    static void manageUserAccess();
};

#endif //MANAGEUSERSMENU_H
