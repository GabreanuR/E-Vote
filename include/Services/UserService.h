#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <string>
#include <nlohmann/json.hpp>
#include "Models/User.h"

using json = nlohmann::json;

class UserService {
    static UserService* instance;
    UserService() = default;
    ~UserService() = default;

public:
    UserService(const UserService&) = delete;
    UserService& operator=(const UserService&) = delete;

    static UserService& getInstance();

    static void viewUsers();

    static void displayUserDetails(const User& user);
    static void addUser();

    static void toggleUserStatus();

    static void deleteUser();

    static void manageUserAccess();

    static bool validateUsername(const std::string& username);
    static bool validatePassword(const std::string& password);

    static bool usernameExists(const std::string& username);
};

#endif