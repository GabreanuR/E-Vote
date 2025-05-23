#include "../../include/ElectionModes/ElectionLevelUtils.h"
#include <algorithm>
#include <stdexcept>

string ElectionLevelUtils::toString(const ElectionLevel level) {
    switch (level) {
        case ElectionLevel::National:  return "National";
        case ElectionLevel::Regional:  return "Regional";
        case ElectionLevel::Municipal: return "Municipal";
        case ElectionLevel::Local:     return "Local";
        case ElectionLevel::NonGov:    return "NonGov";
        default:                       return "Unknown";
    }
}

ElectionLevel ElectionLevelUtils::fromString(const string& str) {
    string lower = str;
    ranges::transform(lower, lower.begin(), ::tolower);

    if (lower == "national")  return ElectionLevel::National;
    if (lower == "regional")  return ElectionLevel::Regional;
    if (lower == "municipal") return ElectionLevel::Municipal;
    if (lower == "local")     return ElectionLevel::Local;
    if (lower == "nongov")    return ElectionLevel::NonGov;

    throw invalid_argument("Invalid string for ElectionLevel: " + str);
}