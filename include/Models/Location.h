#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Location {
private:
    int id;
    std::string name;
    std::string type;

public:
    // Constructors
    Location() : id(0) {}
    Location(int id, const std::string& name, const std::string& type)
        : id(id), name(name), type(type) {}
    explicit Location(const json& data);

    // Getters
    int getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getType() const { return type; }

    // Setters
    void setId(int newId) { id = newId; }
    void setName(const std::string& newName) { name = newName; }
    void setType(const std::string& newType) { type = newType; }

    // Serialization
    [[nodiscard]] json toJson() const;
};

#endif
