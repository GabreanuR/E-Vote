#ifndef ELECTIONLEVELUTILS_H
#define ELECTIONLEVELUTILS_H

#include <string>

using namespace std;

enum class ElectionLevel {
    National,
    Regional,
    Municipal,
    Local,
    NonGov
};

class ElectionLevelUtils {
public:
    static string toString(ElectionLevel level);
    static ElectionLevel fromString(const string& str);
};

#endif
