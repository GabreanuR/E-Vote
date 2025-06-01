#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "../Models/User.h"
#include "../Utils/Types.h"
#include "../Services/LocationService.h"

using json = nlohmann::json;

class UserService {
    std::vector<std::shared_ptr<User> > usersCache;
    std::string usersFilePath = "data/users.json";

    static constexpr int superAdmin = 1;

    UserService();

    void loadUsersFromDataManager();

    [[nodiscard]] bool isUsernameUnique(const std::string &username, int currentUserId = -1) const;

    [[nodiscard]] int calculateNextUserId() const;

    static void handleVoterRegionalAccess(const std::shared_ptr<User> &user, const LocationService &locService);

    static void handleVoterMunicipalAccess(const std::shared_ptr<User> &user, const LocationService &locService);

    static void handleVoterLocalAccess(const std::shared_ptr<User> &user, const LocationService &locService);

    static void handleGenericLevelAccess(const std::shared_ptr<User> &user, const LocationService &locService,
                                         ElectionLevel chosenLevel);

public:
    static UserService &getInstance();

    UserService(const UserService &) = delete;

    UserService &operator=(const UserService &) = delete;

    ~UserService() = default;

    void saveUsersToDataManager() const;

    static void viewUsers();

    static void displayUserDetails(const User &user);

    static void addUser();

    static void toggleUserStatus();

    static void deleteUser();

    static void manageUserAccess();

    std::shared_ptr<User> createUser(const std::string &username, const std::string &password, UserType type);

    bool deleteUserInstance(int userId);

    [[nodiscard]] std::shared_ptr<User> getUser(int userId) const;

    [[nodiscard]] std::shared_ptr<User> getUserByUsername(const std::string &username) const;

    [[nodiscard]] const std::vector<std::shared_ptr<User> > &getAllUsers() const;

    [[nodiscard]] bool toggleUserStatusInstance(int userId) const;

    void grantInitialAccessToSuperAdmin(ElectionLevel level, int entityId) const;

    static bool isUsernameValid(const std::string &username);

    static bool isPasswordValid(const std::string &password);

    void ensureAdminUserExists();

    static std::string electionLevelToString(ElectionLevel level);
};

#endif
