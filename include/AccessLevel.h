#ifndef ACCESSLEVEL_H
#define ACCESSLEVEL_H

#include <string>
#include <nlohmann/json.hpp>
#include "Election.h"

class AccessLevel {
private:
    std::string regional;
    std::string municipal;
    std::string local;
    bool nonGov;

public:
    AccessLevel() = default;
    AccessLevel(std::string regional, std::string municipal, std::string local, bool nonGov);

    bool canAccess(const Election& election) const;

    const std::string& getRegional() const { return regional; }
    const std::string& getMunicipal() const { return municipal; }
    const std::string& getLocal() const { return local; }
    bool isNonGov() const { return nonGov; }

    nlohmann::json toJson() const;
    static AccessLevel fromJson(const nlohmann::json& j);
};



#endif
