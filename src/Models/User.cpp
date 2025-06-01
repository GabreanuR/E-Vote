#include "../include/Models/User.h"
#include "../include/Models/Election.h"

User::User(const json &data) {
    id = data.at("id").get<int>();
    username = data.at("username").get<std::string>();
    password = data.at("password").get<std::string>();
    type = data.at("type").get<std::string>() == "admin" ? UserType::admin : UserType::voter;
    disabled = data.at("disabled").get<bool>();

    restrictedAccess.clear();
    if (data.contains("access") && data.at("access").is_object()) {
        const auto &access = data.at("access");

        if (access.contains("national") && access.at("national").is_boolean() && access.at("national").get<bool>()) {
            restrictedAccess[ElectionLevel::national] = {};
        }

        if (access.contains("regions") && access.at("regions").is_array()) {
            std::set<int> regions;
            for (const auto &region_val: access.at("regions")) {
                if (region_val.is_number_integer()) regions.insert(region_val.get<int>());
            }
            if (!regions.empty() || (access.at("regions").empty() && type == UserType::admin)) {
                restrictedAccess[ElectionLevel::regional] = regions;
            }
        }

        if (access.contains("municipalities") && access.at("municipalities").is_array()) {
            std::set<int> municipalities;
            for (const auto &mun_val: access.at("municipalities")) {
                if (mun_val.is_number_integer()) municipalities.insert(mun_val.get<int>());
            }
            if (!municipalities.empty() || (access.at("municipalities").empty() && type == UserType::admin)) {
                restrictedAccess[ElectionLevel::municipal] = municipalities;
            }
        }

        if (access.contains("localities") && access.at("localities").is_array()) {
            std::set<int> localities;
            for (const auto &loc_val: access.at("localities")) {
                if (loc_val.is_number_integer()) localities.insert(loc_val.get<int>());
            }
            if (!localities.empty() || (access.at("localities").empty() && type == UserType::admin)) {
                restrictedAccess[ElectionLevel::local] = localities;
            }
        }

        if (access.contains("nonGovernment") && access.at("nonGovernment").is_array()) {
            std::set<int> nonGov;
            for (const auto &ng_val: access.at("nonGovernment")) {
                if (ng_val.is_number_integer()) nonGov.insert(ng_val.get<int>());
            }
            if (!nonGov.empty() || (access.at("nonGovernment").empty() && type == UserType::admin)) {
                restrictedAccess[ElectionLevel::non_governmental] = nonGov;
            }
        }
    }
}

bool User::grantAccess(const ElectionLevel level, const int entityId) {
    bool changed = false;
    if (entityId == -1) {
        if (const auto it = restrictedAccess.find(level); it == restrictedAccess.end() || !it->second.empty()) {
            restrictedAccess[level].clear();
            changed = true;
        }
        return changed;
    }

    if (entityId == 0) {
        if (restrictedAccess.erase(level) > 0) {
            changed = true;
        }

        if (type == UserType::admin) {
            switch (level) {
                case ElectionLevel::national:
                    if (restrictedAccess.erase(ElectionLevel::regional) > 0) changed = true;
                    if (restrictedAccess.erase(ElectionLevel::municipal) > 0) changed = true;
                    if (restrictedAccess.erase(ElectionLevel::local) > 0) changed = true;
                    break;
                case ElectionLevel::regional:
                    if (restrictedAccess.erase(ElectionLevel::municipal) > 0) changed = true;
                    if (restrictedAccess.erase(ElectionLevel::local) > 0) changed = true;
                    break;
                case ElectionLevel::municipal:
                    if (restrictedAccess.erase(ElectionLevel::local) > 0) changed = true;
                    break;
                default:
                    break;
            }
        }
        return changed;
    }

    return restrictedAccess[level].insert(entityId).second;
}

bool User::revokeAccess(const ElectionLevel level, const int entityId) {
    if (entityId <= 0) return false;

    if (const auto it = restrictedAccess.find(level); it != restrictedAccess.end()) {
        return it->second.erase(entityId) > 0;
    }
    return false;
}

bool User::hasAccessTo(const ElectionLevel level, const int entityId) const {
    if (type == UserType::admin) return true;

    const auto it = restrictedAccess.find(level);
    if (it == restrictedAccess.end()) {
        if (const auto national_it = restrictedAccess.find(ElectionLevel::national);
            national_it != restrictedAccess.end() && national_it->second.empty()) {
            return true;
        }
        return false;
    }

    if (it->second.empty()) return true;

    return it->second.contains(entityId);
}

json User::toJson() const {
    json data = {
        {"id", id},
        {"username", username},
        {"password", password},
        {"type", type == UserType::admin ? "admin" : "voter"},
        {"disabled", disabled}
    };

    json access_json_output;

    if (const auto it_national = restrictedAccess.find(ElectionLevel::national);
        it_national != restrictedAccess.end() && it_national->second.empty()) {
        access_json_output["national"] = true;
    } else {
        access_json_output["national"] = false;
    }

    const std::map<ElectionLevel, std::string> levelToString = {
        {ElectionLevel::regional, "regions"},
        {ElectionLevel::municipal, "municipalities"},
        {ElectionLevel::local, "localities"},
        {ElectionLevel::non_governmental, "nonGovernment"}
    };

    for (const auto &[fst, snd]: levelToString) {
        if (auto it = restrictedAccess.find(fst); it != restrictedAccess.end()) {
            if (it->second.empty()) {
                access_json_output[snd] = json::array();
            } else {
                access_json_output[snd] = json(it->second);
            }
        }
    }

    data["access"] = access_json_output;
    return data;
}
