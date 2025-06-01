#ifndef LOCATIONSERVICE_H
#define LOCATIONSERVICE_H

#include <memory>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "../Models/Location.h"

using json = nlohmann::json;

class User;
enum class ElectionLevel;

class LocationService {
    static LocationService *instance;

    LocationService();

    ~LocationService() = default;

    std::string locationsFilePath_ = "data/locations.json";

    std::vector<std::shared_ptr<Region> > regionsCache;
    std::vector<std::shared_ptr<Municipality> > municipalitiesCache;
    std::vector<std::shared_ptr<Locality> > localitiesCache;
    std::vector<std::shared_ptr<NonGovernment> > nonGovernmentEntitiesCache;

    static std::shared_ptr<Location> createLocationFromJson(const json &data);

    void buildLocationHierarchy() const;

    void loadLocationsFromDataManager();

    [[nodiscard]] int calculateNextLocationId() const;

public:
    static LocationService &getInstance();

    void saveLocationsToDataManager() const;

    bool addRegion(const std::string &name);

    bool addMunicipality(const std::string &name, int regionId);

    bool addLocality(const std::string &name, int municipalityId);

    bool addNonGovernment(const std::string &name, const std::string &entityType);

    [[nodiscard]] std::shared_ptr<Region> getRegion(int id) const;

    [[nodiscard]] std::shared_ptr<Municipality> getMunicipality(int id) const;

    [[nodiscard]] std::shared_ptr<Locality> getLocality(int id) const;

    [[nodiscard]] std::shared_ptr<NonGovernment> getNonGovernment(int id) const;

    [[nodiscard]] const std::vector<std::shared_ptr<Region> > &getAllRegions() const { return regionsCache; }

    [[nodiscard]] const std::vector<std::shared_ptr<Municipality> > &getAllMunicipalities() const {
        return municipalitiesCache;
    }

    [[nodiscard]] const std::vector<std::shared_ptr<Locality> > &getAllLocalities() const { return localitiesCache; }

    [[nodiscard]] const std::vector<std::shared_ptr<NonGovernment> > &getAllNonGovernment() const {
        return nonGovernmentEntitiesCache;
    }

    [[nodiscard]] std::vector<std::shared_ptr<Location> > getAllRegionsAsBase() const;

    [[nodiscard]] std::vector<std::shared_ptr<Location> > getAllMunicipalitiesAsBase() const;

    [[nodiscard]] std::vector<std::shared_ptr<Location> > getAllLocalitiesAsBase() const;

    [[nodiscard]] std::vector<std::shared_ptr<Location> > getAllNonGovernmentAsBase() const;
};

#endif
