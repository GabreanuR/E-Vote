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

struct UserInputCancelledException final : std::runtime_error {
    UserInputCancelledException() : std::runtime_error("User cancelled input.") {}
};

#endif // TYPES_H
