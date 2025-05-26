#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <vector>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

// Forward declarations
class User;
class Election;
class Candidate;
class Location;
class Region;
class Municipality;
class Locality;
class NonGovernment;
class VoteLog;

using json = nlohmann::json;

class DataManager {
private:
    static DataManager* instance;
    DataManager() = default;

    // Data vectors
    std::vector<User> users;
    std::vector<Election> elections;
    std::vector<Candidate> candidates;
    std::vector<std::shared_ptr<Location>> locations;
    std::vector<VoteLog> voteLogs;

    // File paths
    const std::string users_file = "data/users.json";
    const std::string elections_file = "data/elections.json";
    const std::string candidates_file = "data/candidates.json";
    const std::string locations_file = "data/locations.json";
    const std::string votes_log_file = "data/votes_log.json";

    // Static file operations
    static bool saveToFile(const std::string& filename, const json& data);
    static json loadFromFile(const std::string& filename);

    // Helper methods
    void loadUsers();
    void loadElections();
    void loadCandidates();
    void loadLocations();
    void loadVoteLogs();

    void saveUsers();
    void saveElections();
    void saveCandidates();
    void saveLocations();
    void saveVoteLogs();

public:
    static DataManager& getInstance();
    
    // Prevent copying
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

    // Static data access methods
    static bool saveData(const std::string& key, const json& data);
    static json loadData(const std::string& key);
    
    // Load all data
    void loadAllData();
    
    // Save all data
    void saveAllData();
    
    // Getters for data vectors
    const std::vector<User>& getUsers() const { return users; }
    const std::vector<Election>& getElections() const { return elections; }
    const std::vector<Candidate>& getCandidates() const { return candidates; }
    const std::vector<std::shared_ptr<Location>>& getLocations() const { return locations; }
    const std::vector<VoteLog>& getVoteLogs() const { return voteLogs; }
    
    // Methods to add/update/remove data
    void addUser(const User& user);
    void updateUser(const User& user);
    void removeUser(int userId);
    
    void addElection(const Election& election);
    void updateElection(const Election& election);
    
    void addCandidate(const Candidate& candidate);
    
    void addLocation(const std::shared_ptr<Location>& location);
    
    void addVoteLog(const VoteLog& voteLog);
    
    // Helper methods to find data
    User* findUser(int userId);
    Election* findElection(int electionId);
    Candidate* findCandidate(int candidateId);
    std::shared_ptr<Location> findLocation(int locationId);
};

#endif //DATAMANAGER_H
