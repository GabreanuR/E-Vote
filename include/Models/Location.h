#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
class Municipality;
class Locality;

// Base class for all locations
class Location {
protected:
    int id;
    std::string name;

public:
    // Constructors
    Location() : id(0) {}
    Location(int id, const std::string& name) : id(id), name(name) {}
    virtual ~Location() = default;

    // Getters
    int getId() const { return id; }
    const std::string& getName() const { return name; }

    // Setters
    void setId(int newId) { id = newId; }
    void setName(const std::string& newName) { name = newName; }

    // Virtual methods
    virtual std::string getType() const = 0;
    virtual json toJson() const = 0;
    virtual void fromJson(const json& data) = 0;
};

// Region class
class Region : public Location {
private:
    std::vector<std::shared_ptr<Municipality>> municipalities;

public:
    Region() = default;
    Region(int id, const std::string& name) : Location(id, name) {}
    explicit Region(const json& data);

    // Getters
    const std::vector<std::shared_ptr<Municipality>>& getMunicipalities() const { return municipalities; }
    std::string getType() const override { return "region"; }

    // Methods
    void addMunicipality(const std::shared_ptr<Municipality>& municipality);
    void removeMunicipality(int municipalityId);
    bool hasMunicipality(int municipalityId) const;
    void clearMunicipalities() { municipalities.clear(); }

    // Serialization
    json toJson() const override;
    void fromJson(const json& data) override;
};

// Municipality class
class Municipality : public Location {
private:
    int regionId;
    std::vector<std::shared_ptr<Locality>> localities;

public:
    Municipality() : regionId(0) {}
    Municipality(int id, const std::string& name, int regionId) 
        : Location(id, name), regionId(regionId) {}
    explicit Municipality(const json& data);

    // Getters
    int getRegionId() const { return regionId; }
    const std::vector<std::shared_ptr<Locality>>& getLocalities() const { return localities; }
    std::string getType() const override { return "municipality"; }

    // Setters
    void setRegionId(int newRegionId) { regionId = newRegionId; }

    // Methods
    void addLocality(const std::shared_ptr<Locality>& locality);
    void removeLocality(int localityId);
    bool hasLocality(int localityId) const;
    void clearLocalities() { localities.clear(); }

    // Serialization
    json toJson() const override;
    void fromJson(const json& data) override;
};

// Locality class
class Locality : public Location {
private:
    int municipalityId;

public:
    Locality() : municipalityId(0) {}
    Locality(int id, const std::string& name, int municipalityId) 
        : Location(id, name), municipalityId(municipalityId) {}
    explicit Locality(const json& data);

    // Getters
    int getMunicipalityId() const { return municipalityId; }
    std::string getType() const override { return "locality"; }

    // Setters
    void setMunicipalityId(int newMunicipalityId) { municipalityId = newMunicipalityId; }

    // Serialization
    json toJson() const override;
    void fromJson(const json& data) override;
};

// NonGovernment class
class NonGovernment : public Location {
private:
    std::string entityType;

public:
    NonGovernment() = default;
    NonGovernment(int id, const std::string& name, const std::string& entityType) 
        : Location(id, name), entityType(entityType) {}
    explicit NonGovernment(const json& data);

    // Getters
    const std::string& getEntityType() const { return entityType; }
    std::string getType() const override { return "nonGovernment"; }

    // Setters
    void setEntityType(const std::string& newType) { entityType = newType; }

    // Serialization
    json toJson() const override;
    void fromJson(const json& data) override;
};

#endif
