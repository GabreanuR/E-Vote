#ifndef USER_H
#define USER_H

#include <string>
#include <utility>

class User {
protected:
    std::string id;
public:
    explicit User(std::string  id) : id(std::move(id)) {}
    virtual ~User() = default;
    virtual void showDashboard() = 0;
};

#endif
