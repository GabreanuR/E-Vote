#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

class Admin : public User {
public:
    explicit Admin(const std::string& id);
    void showDashboard() override;
};

#endif
