#ifndef LOCATIONSERVICE_H
#define LOCATIONSERVICE_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Forward declarations
class User;
enum class ElectionLevel;

class LocationService {
private:
    static LocationService* instance;
    LocationService() = default;
    ~LocationService() = default;

public:
    static LocationService& getInstance();

    // Exact same functions that were in ManageLocationsMenu
    static void addRegion();
    static void addMunicipality();
    static void addLocality();
    static void addNonGovernment();
    static void displayLocations();
};

#endif // LOCATIONSERVICE_H 