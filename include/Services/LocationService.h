#ifndef LOCATIONSERVICE_H
#define LOCATIONSERVICE_H

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>
#include "../Models/Location.h"

using json = nlohmann::json;

// Forward declarations
class User;
enum class ElectionLevel;

class LocationService {
private:
    static LocationService* instance;
    LocationService() = default;
    ~LocationService() = default;

    // Helper methods
    std::shared_ptr<Location> createLocationFromJson(const json& data);
    void buildLocationHierarchy();
    void saveLocationHierarchy();

    // Location collections
    std::vector<std::shared_ptr<Region>> regions;
    std::vector<std::shared_ptr<Municipality>> municipalities;
    std::vector<std::shared_ptr<Locality>> localities;
    std::vector<std::shared_ptr<NonGovernment>> nonGovernmentEntities;

public:
    static LocationService& getInstance();

    // Location management methods
    void addRegion(const std::string& name);
    void addMunicipality(const std::string& name, int regionId);
    void addLocality(const std::string& name, int municipalityId);
    void addNonGovernment(const std::string& name, const std::string& entityType);

    // Location retrieval methods
    std::shared_ptr<Region> getRegion(int id) const;
    std::shared_ptr<Municipality> getMunicipality(int id) const;
    std::shared_ptr<Locality> getLocality(int id) const;
    std::shared_ptr<NonGovernment> getNonGovernment(int id) const;

    void displayLocations();

    // Location listing methods
    const std::vector<std::shared_ptr<Region>>& getAllRegions() const { return regions; }
    const std::vector<std::shared_ptr<Municipality>>& getAllMunicipalities() const { return municipalities; }
    const std::vector<std::shared_ptr<Locality>>& getAllLocalities() const { return localities; }
    const std::vector<std::shared_ptr<NonGovernment>>& getAllNonGovernment() const { return nonGovernmentEntities; }

    // Data persistence methods
    void loadLocations();
    void saveLocations();
};

#endif // LOCATIONSERVICE_H 