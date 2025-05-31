#ifndef TYPES_H
#define TYPES_H

#include <stdexcept>

enum class UserType {
    voter,
    admin
};

enum class ElectionLevel {
    national,
    regional,
    municipal,
    local,
    non_governmental
};

enum class LocationType {
    base,
    region,
    municipality,
    locality,
    non_government
};

enum class ElectionStatus {
    created,
    open,
    closed
};

enum class VotingSystemType {
    majority,
    proportional
};

struct UserInputCancelledException final : std::runtime_error {
    UserInputCancelledException() : std::runtime_error("User cancelled input.") {
    }
};

#endif
