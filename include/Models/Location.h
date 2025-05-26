#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <vector>
#include <memory>
#include "nlohmann/json.hpp"
#include <stdexcept>
#include "../Utils/Types.h"

using json = nlohmann::json;

class Municipality;
class Locality;

struct UserInputCancelledException final : std::runtime_error {
    UserInputCancelledException() : std::runtime_error("User cancelled input.") {}
};

class Location {
    int id;
    std::string name;
public:
    Location();
    Location(int id, std::string  name);
    Location(const Location& other);
    virtual ~Location() = default;

    Location& operator=(const Location& other);
    bool operator==(const Location& other) const;

    [[nodiscard]] int Location::getId() const { return id; }
    [[nodiscard]] std::string Location::getName() const { return name; }
    void Location::setId(const int newId) { this->id = newId; }
    void Location::setName(const std::string& newName) { this->name = newName; }

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
    explicit Region(const json& data);
    Region(const Region& other);
    ~Region() override = default;

    Region& operator=(const Region& other);

    void addMunicipality(const std::shared_ptr<Municipality>& municipality);
    void removeMunicipality(int municipalityId);
    [[nodiscard]] bool hasMunicipality(int municipalityId) const;
    [[nodiscard]] const std::vector<std::shared_ptr<Municipality>>& getMunicipalities() const;
    void clearMunicipalities();

    [[nodiscard]] LocationType getType() const override;
    [[nodiscard]] json toJson() const override;
    void fromJson(const json& data) override;

    void print(std::ostream& os) const override;
    void read(std::istream& is) override;

    void displayRegionSummary() const;
};

class Municipality final : public Location {
    int regionId;
    std::vector<std::shared_ptr<Locality>> localities;
public:
    Municipality() : regionId(0) {}
    Municipality(int id, const std::string& name, int regionId);
    explicit Municipality(const json& data);
    Municipality(const Municipality& other);
    ~Municipality() override = default;

    Municipality& operator=(const Municipality& other);

    [[nodiscard]] int getRegionId() const;
    void setRegionId(int regionId);
    void addLocality(const std::shared_ptr<Locality>& locality);
    void removeLocality(int localityId);
    [[nodiscard]] bool hasLocality(int localityId) const;
    [[nodiscard]] const std::vector<std::shared_ptr<Locality>>& getLocalities() const;
    void clearLocalities();

    [[nodiscard]] LocationType getType() const override;
    [[nodiscard]] json toJson() const override;
    void fromJson(const json& data) override;

    void print(std::ostream& os) const override;
    void read(std::istream& is) override;

    void displayMunicipalityDetails(const class LocationService& service) const;
};

class Locality final : public Location {
    int municipalityId;

public:
    Locality() : municipalityId(0) {}
    Locality(int id, const std::string& name, int municipalityId);
    explicit Locality(const json& data);
    Locality(const Locality& other);
    ~Locality() override = default;

    Locality& operator=(const Locality& other);

    [[nodiscard]] int getMunicipalityId() const;
    void setMunicipalityId(int municipalityId);

    [[nodiscard]] LocationType getType() const override;
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
    NonGovernment(int id, const std::string& name, const std::string& entityType);
    explicit NonGovernment(const json& data);
    NonGovernment(const NonGovernment& other);
    ~NonGovernment() override = default;

    NonGovernment& operator=(const NonGovernment& other);

    [[nodiscard]] std::string getEntityType() const;
    void setEntityType(const std::string& entityType);

    [[nodiscard]] LocationType getType() const override;
    [[nodiscard]] json toJson() const override;
    void fromJson(const json& data) override;

    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
};

#endif
