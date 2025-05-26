#ifndef TYPES_H
#define TYPES_H

enum class UserType {
    voter,
    admin
};

enum class ElectionLevel {
    national,
    regional,
    municipal,
    local,
    nonGovernment
};

enum class LocationType {
    base,
    region,
    municipality,
    locality,
    non_government
};

#endif // TYPES_H
