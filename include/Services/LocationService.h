#ifndef LOCATIONSERVICE_H
#define LOCATIONSERVICE_H

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>
#include "../Models/Location.h"

using json = nlohmann::json;

class User;
enum class ElectionLevel;

class LocationService {
    static LocationService* instance;
    LocationService() = default;
    ~LocationService() = default;

    static std::shared_ptr<Location> createLocationFromJson(const json& data);
    void buildLocationHierarchy() const;

    std::vector<std::shared_ptr<Region>> regions;
    std::vector<std::shared_ptr<Municipality>> municipalities;
    std::vector<std::shared_ptr<Locality>> localities;
    std::vector<std::shared_ptr<NonGovernment>> nonGovernmentEntities;

public:
    static LocationService& getInstance();

    void loadLocations();
    void saveLocations() const;

    bool addRegion(const std::string& name);
    bool addMunicipality(const std::string& name, int regionId);
    bool addLocality(const std::string& name, int municipalityId);
    bool addNonGovernment(const std::string& name, const std::string& entityType);

    [[nodiscard]] std::shared_ptr<Region> getRegion(int id) const;
    [[nodiscard]] std::shared_ptr<Municipality> getMunicipality(int id) const;
    [[nodiscard]] std::shared_ptr<Locality> getLocality(int id) const;
    [[nodiscard]] std::shared_ptr<NonGovernment> getNonGovernment(int id) const;

    [[nodiscard]] const std::vector<std::shared_ptr<Region>>& getAllRegions() const { return regions; }
    [[nodiscard]] const std::vector<std::shared_ptr<Municipality>>& getAllMunicipalities() const { return municipalities; }
    [[nodiscard]] const std::vector<std::shared_ptr<Locality>>& getAllLocalities() const { return localities; }
    [[nodiscard]] const std::vector<std::shared_ptr<NonGovernment>>& getAllNonGovernment() const { return nonGovernmentEntities; }

};

#endif