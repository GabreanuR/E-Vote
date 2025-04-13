#ifndef USER_H
#define USER_H

#include <string>

class User {
protected:
    std::string id;
public:
    explicit User(const std::string& id) : id(id) {}
    virtual ~User() = default;
    virtual void showDashboard() = 0;
};

#endif
