#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include "User.h"
#include <string>

using namespace std;

class Authenticator {
    string userType;
public:
    explicit Authenticator(string type);
    [[nodiscard]] User* authenticate() const;
};

#endif
