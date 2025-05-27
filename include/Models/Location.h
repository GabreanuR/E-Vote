#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <vector>
#include <memory>
#include "nlohmann/json.hpp"
#include "../Utils/Types.h"

using json = nlohmann::json;

class Municipality;
class Locality;

class Location {
    int id;
    std::string name;
public:
    Location();
    Location(int id, std::string  name);
    Location(const Location& other) = default;
    virtual ~Location() = default;

    Location& operator=(const Location& other);
    bool operator==(const Location& other) const;

    [[nodiscard]] int getId() const { return id; }
    [[nodiscard]] std::string getName() const { return name; }
    void setId(const int newId) { this->id = newId; }
    void setName(const std::string& newName) { this->name = newName; }

    [[nodiscard]] virtual LocationType getType() const = 0;
    [[nodiscard]] virtual json toJson() const = 0;
    virtual void fromJson(const json& data);

    virtual void print(std::ostream& out) const;
    virtual void read(std::istream& in);

    friend std::ostream& operator<<(std::ostream& os, const Location& loc);
    friend std::istream& operator>>(std::istream& is, Location& loc);
};

bool operator!=(const Location& ob1, const Location& ob2);

class Region final : public Location {
    std::vector<std::shared_ptr<Municipality>> municipalities;

public:
    Region() = default;
    Region(int id, const std::string& name);
    explicit Region(const json& data) { fromJson(data); }
    Region(const Region& other);
    ~Region() override = default;

    Region& operator=(const Region& other);

    void addMunicipality(const std::shared_ptr<Municipality>& municipality);
    void removeMunicipality(int municipalityId);
    [[nodiscard]] bool hasMunicipality(int municipalityId) const;
    [[nodiscard]] const std::vector<std::shared_ptr<Municipality>>& getMunicipalities() const;
    void clearMunicipalities() { municipalities.clear(); }

    [[nodiscard]] LocationType getType() const override { return LocationType::region; }
    [[nodiscard]] json toJson() const override;
    void fromJson(const json& data) override { Location::fromJson(data); }

    void print(std::ostream& os) const override;
    void read(std::istream& is) override { Location::read(is); }

    void displayRegionSummary() const;
};

class Municipality final : public Location {
    int regionId{};
    std::vector<std::shared_ptr<Locality>> localities;
public:
    Municipality() : regionId(0) {}
    Municipality(int id, const std::string& name, int regionId);
    explicit Municipality(const json& data) { fromJson(data); }
    Municipality(const Municipality& other);
    ~Municipality() override = default;

    Municipality& operator=(const Municipality& other);

    [[nodiscard]] int getRegionId() const { return regionId; }
    void setRegionId(const int newRegionId) { this->regionId = newRegionId; }
    void addLocality(const std::shared_ptr<Locality>& locality);
    void removeLocality(int localityId);
    [[nodiscard]] bool hasLocality(int localityId) const;
    [[nodiscard]] const std::vector<std::shared_ptr<Locality>>& getLocalities() const;
    void clearLocalities() { localities.clear(); }

    [[nodiscard]] LocationType getType() const override { return LocationType::municipality; }
    [[nodiscard]] json toJson() const override;
    void fromJson(const json& data) override;

    void print(std::ostream& os) const override;
    void read(std::istream& is) override;

    void displayMunicipalityDetails(const class LocationService& service) const;
};

class Locality final : public Location {
    int municipalityId{};

public:
    Locality() : municipalityId(0) {}
    Locality(int id, const std::string& name, int municipalityId);
    explicit Locality(const json& data) { fromJson(data); }
    Locality(const Locality& other) = default;
    ~Locality() override = default;

    Locality& operator=(const Locality& other);

    [[nodiscard]] int getMunicipalityId() const { return municipalityId; }
    void setMunicipalityId(const int newMunicipalityId) { this->municipalityId = newMunicipalityId; }

    [[nodiscard]] LocationType getType() const override { return LocationType::locality; }
    [[nodiscard]] json toJson() const override;
    void fromJson(const json& data) override;

    void print(std::ostream& os) const override;
    void read(std::istream& is) override;

    void showHierarchy(const LocationService& service) const;
};

class NonGovernment final : public Location {
    std::string entityType;

public:
    NonGovernment() = default;
    NonGovernment(int id, const std::string& name, std::string  entityType);
    explicit NonGovernment(const json& data) { fromJson(data); }
    NonGovernment(const NonGovernment& other) = default;
    ~NonGovernment() override = default;

    NonGovernment& operator=(const NonGovernment& other);

    [[nodiscard]] std::string getEntityType() const { return entityType; }
    void setEntityType(const std::string& newEntityType) { this->entityType = newEntityType; }

    [[nodiscard]] LocationType getType() const override { return LocationType::non_government; }
    [[nodiscard]] json toJson() const override;
    void fromJson(const json& data) override;

    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
};

#endif
