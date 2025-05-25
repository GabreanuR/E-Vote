#include "../../include/Models/Location.h"

Location::Location(const json& data) {
    id = data["id"];
    name = data["name"];
    type = data["type"];
}

json Location::toJson() const {
    return {
        {"id", id},
        {"name", name},
        {"type", type}
    };
}
