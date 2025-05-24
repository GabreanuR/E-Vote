#include "../../include/UserStructure/AccessLevel.h"

bool AccessLevel::canAccess(const Election& election) const {
    switch (election.getLevel()) {
        case ElectionLevel::Regional:
            return election.getName() == regional;
        case ElectionLevel::Municipal:
            return election.getName() == municipal;
        case ElectionLevel::Local:
            return election.getName() == local;
        case ElectionLevel::NonGov:
            return nonGov;
        case ElectionLevel::National:
            return true; // maybe global access?
        default:
            return false;
    }
}