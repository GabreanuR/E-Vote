#include <iostream>
#include <string>
#include "voters.h"

using namespace std;

class Voter {
    static int counter_id;
    const int voter_id = counter_id++;
    string name;
    const int age;
public:

};

class Admin : public Voter {

};

class Student: public Voter {

};

class Professional: public Voter {

};
