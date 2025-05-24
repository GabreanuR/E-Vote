#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class DataManager {
    static const std::string data_directory;

    DataManager();

    ~DataManager();

    static bool saveToFile(const std::string &filename, const json &data);

    static json loadFromFile(const std::string &filename);

public:
    static DataManager &getInstance();

    DataManager(const DataManager &) = delete;

    DataManager &operator=(const DataManager &) = delete;

    static bool saveData(const std::string &key, const json &data);

    static json loadData(const std::string &key);
};

#endif // DATAMANAGER_H
