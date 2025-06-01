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
    LocationService();

    ~LocationService() = default;

    std::string locationsFilePath = "data/locations.json";

    std::vector<std::shared_ptr<Region> > regionsCache;
    std::vector<std::shared_ptr<Municipality> > municipalitiesCache;
    std::vector<std::shared_ptr<Locality> > localitiesCache;
    std::vector<std::shared_ptr<NonGovernment> > nonGovernmentEntitiesCache;

    void buildLocationHierarchy() const;

    void loadLocationsFromDataManager();

public:
    static LocationService &getInstance();

    LocationService(const LocationService &) = delete;

    LocationService &operator=(const LocationService &) = delete;

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

    [[nodiscard]] std::string getLocationName(ElectionLevel level, int entityId) const;
    [[nodiscard]] std::vector<int> getAllLocationIds(ElectionLevel level) const;
};

#endif
