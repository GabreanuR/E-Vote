#include "../../include/Models/Location.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>

Location::Location() : id(0){}

Location::Location(const int id, std::string  name) : id(id), name(std::move(name)) {}

Location& Location::operator=(const Location& other) {
    if (this != &other) {
        id = other.id;
        name = other.name;
    }
    return *this;
}

bool Location::operator==(const Location& other) const {
    return this->id == other.id && this->name == other.name;
}

void Location::fromJson(const json& data) {
    if (data.contains("id")) {
        id = data["id"].get<int>();
    }
    if (data.contains("name")) {
        name = data["name"].get<std::string>();
    }
}

void Location::print(std::ostream& out) const {
    out << "ID: " << id << ", Name: " << name;
}

void Location::read(std::istream& in) {
    std::cout << "Enter Name (or press Enter to cancel): ";
    std::string line;
    if (in.peek() == '\n') {
        in.ignore();
    }
    std::getline(in, line);

    if (line.empty()) {
        throw UserInputCancelledException();
    }
    name = line;

    if (in.fail() && !in.eof()) {
        in.clear();
    }
}

std::ostream& operator<<(std::ostream& os, const Location& loc) {
    loc.print(os);
    return os;
}

std::istream& operator>>(std::istream& is, Location& loc) {
    loc.read(is);
    return is;
}

bool operator!=(const Location& ob1, const Location& ob2) {
    return !(ob1 == ob2);
}

Region::Region(const int id, const std::string& name) : Location(id, name) {}

Region::Region(const Region& other) : Location(other) {
    municipalities = other.municipalities;
}

Region& Region::operator=(const Region& other) {
    if (this != &other) {
        Location::operator=(other);
        municipalities = other.municipalities;
    }
    return *this;
}

void Region::addMunicipality(const std::shared_ptr<Municipality> &municipality) {
    if (municipality) {
        this->municipalities.push_back(municipality);
    }
}

void Region::removeMunicipality(int municipalityId) {
    std::erase_if(municipalities,
                  [municipalityId](const std::shared_ptr<Location>& m) {
                      return m->getId() == municipalityId;
                  });
}

bool Region::hasMunicipality(int municipalityId) const {
    return std::ranges::any_of(municipalities,
                               [municipalityId](const std::shared_ptr<Location>& m) {
                                   return m->getId() == municipalityId;
                               });
}

const std::vector<std::shared_ptr<Municipality>> & Region::getMunicipalities() const {
    return municipalities;
}

json Region::toJson() const {
    json data;
    data["name"] = getName();
    data["municipalities"] = json::array();
    for (const auto& m : municipalities) {
        data["municipalities"].push_back(m->getId());
    }
    return data;
}

void Region::print(std::ostream& os) const {
    Location::print(os);
    os << ", Municipalities: [";
    for (size_t i = 0; i < municipalities.size(); ++i) {
        os << (municipalities[i] ? municipalities[i]->getName() : "<null>");
        if (i < municipalities.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
}

void Region::displayRegionSummary() const {
    std::cout << "Region Summary for '" << getName() << "' (ID: " << getId() << "):\n";
    if (municipalities.empty()) {
        std::cout << "  No municipalities.\n";
    } else {
        std::cout << "  Municipalities:\n";
        for (const auto& mun : municipalities) {
            std::cout << "    - " << mun->getName() << " (ID: " << mun->getId() << ")\n";
        }
    }
}

Municipality::Municipality(const int id, const std::string& name, const int regionId) : Location(id, name), regionId(regionId) {}

Municipality::Municipality(const Municipality& other) : Location(other), regionId(other.regionId) {
    localities = other.localities;
}

Municipality& Municipality::operator=(const Municipality& other) {
    if (this != &other) {
        Location::operator=(other);
        regionId = other.regionId;
        localities = other.localities;
    }
    return *this;
}

void Municipality::addLocality(const std::shared_ptr<Locality> &locality) {
    if (locality) {
        this->localities.push_back(locality);
    }
}

void Municipality::removeLocality(int localityId) {
    std::erase_if(localities,
                  [localityId](const std::shared_ptr<Location>& l) {
                      return l->getId() == localityId;
                  });
}

bool Municipality::hasLocality(int localityId) const {
    return std::ranges::any_of(localities,
                               [localityId](const std::shared_ptr<Location>& l) {
                                   return l->getId() == localityId;
                               });
}

const std::vector<std::shared_ptr<Locality>> & Municipality::getLocalities() const {
    return localities;
}

json Municipality::toJson() const {
    json data;
    data["name"] = getName();
    data["regionId"] = regionId;
    data["localities"] = json::array();
    for (const auto& l : localities) {
        data["localities"].push_back(l->getId());
    }
    return data;
}

void Municipality::fromJson(const json& data) {
    Location::fromJson(data);
    if (data.contains("regionId")) {
        regionId = data["regionId"].get<int>();
    }
}

void Municipality::print(std::ostream& os) const {
    Location::print(os);
    os << ", Region ID: " << regionId;
    os << ", Localities: [";
    for (size_t i = 0; i < localities.size(); ++i) {
        os << (localities[i] ? localities[i]->getName() : "<null>");
        if (i < localities.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
}

void Municipality::read(std::istream& is) {
    Location::read(is);

    std::string line;
    std::cout << "Enter Region ID for this Municipality (or press Enter to cancel): ";

    std::getline(is, line);
    if (line.empty()) {
        throw UserInputCancelledException();
    }

    while (true) {
        try {
            regionId = std::stoi(line);
            break;
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid input. Not a number. Please enter a numeric Region ID (or press Enter to cancel): ";
        } catch (const std::out_of_range&) {
            std::cout << "Invalid input. Number out of range. Please enter a numeric Region ID (or press Enter to cancel): ";
        }

        std::getline(is, line);
        if (line.empty()) {
            throw UserInputCancelledException();
        }
    }
}

void Municipality::displayMunicipalityDetails(const LocationService&) const {
    std::cout << "Municipality Details for '" << getName() << "' (ID: " << getId() << "):\n";
    std::cout << "  Region ID: " << this->regionId << "\n";
    if (localities.empty()) {
        std::cout << "  No localities.\n";
    } else {
        std::cout << "  Localities:\n";
        for (const auto& loc : localities) {
            std::cout << "    - " << loc->getName() << " (ID: " << loc->getId() << ")\n";
        }
    }
}

Locality::Locality(const int id, const std::string& name, const int municipalityId) : Location(id, name), municipalityId(municipalityId) {}

Locality& Locality::operator=(const Locality& other) {
    if (this != &other) {
        Location::operator=(other);
        municipalityId = other.municipalityId;
    }
    return *this;
}

json Locality::toJson() const {
    json data;
    data["name"] = getName();
    data["municipalityId"] = municipalityId;
    return data;
}

void Locality::fromJson(const json& data) {
    Location::fromJson(data);
    if (data.contains("municipalityId")) {
        municipalityId = data["municipalityId"].get<int>();
    }
}

void Locality::print(std::ostream& os) const {
    Location::print(os);
    os << ", Municipality ID: " << municipalityId;
}

void Locality::read(std::istream& is) {
    Location::read(is);

    std::string line;
    std::cout << "Enter Municipality ID for this Locality (or press Enter to cancel): ";
    std::getline(is, line);
    if (line.empty()) {
        throw UserInputCancelledException();
    }

    while (true) {
        try {
            municipalityId = std::stoi(line);
            break;
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid input. Not a number. Please enter a numeric Municipality ID (or press Enter to cancel): ";
        } catch (const std::out_of_range&) {
            std::cout << "Invalid input. Number out of range. Please enter a numeric Municipality ID (or press Enter to cancel): ";
        }
        std::getline(is, line);
        if (line.empty()) {
            throw UserInputCancelledException();
        }
    }
}

void Locality::showHierarchy(const LocationService&) const {
    std::cout << "Locality Hierarchy for '" << getName() << "' (ID: " << getId() << "):\n";
    std::cout << "  Municipality ID: " << this->municipalityId << "\n";
}

NonGovernment::NonGovernment(const int id, const std::string& name, std::string  entityType) : Location(id, name), entityType(std::move(entityType)) {}

NonGovernment& NonGovernment::operator=(const NonGovernment& other) {
    if (this != &other) {
        Location::operator=(other);
        entityType = other.entityType;
    }
    return *this;
}

json NonGovernment::toJson() const {
    json data;
    data["name"] = getName();
    data["entityType"] = entityType;
    return data;
}

void NonGovernment::fromJson(const json& data) {
    Location::fromJson(data);
    if (data.contains("entityType")) {
        entityType = data["entityType"].get<std::string>();
    }
}

void NonGovernment::print(std::ostream& os) const {
    Location::print(os);
    os << ", Entity Type: " << entityType;
}

void NonGovernment::read(std::istream& is) {
    Location::read(is);

    std::cout << "Enter Entity Type (or press Enter to cancel): ";
    std::string line;
    std::getline(is, line);
    if (line.empty()) {
        throw UserInputCancelledException();
    }
    entityType = line;

    if (is.fail() && !is.eof()) {
        is.clear();
    }
}