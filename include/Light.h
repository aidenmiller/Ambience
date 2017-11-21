#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

struct xy_t {
    double x;
    double y;
};

struct state_t {
    string alert;
    int bri;
    string colormode;
    int ct;
    string effect;
    int hue;
    bool on;
    bool reachable;
    int sat;
    struct xy_t xy;
};

class Light {

public:
    Light(string type, string name, string modelid,
           string swversion, string uniqueid, struct state_t state);

    virtual ~Light();

    //GETTER METHODS
    string getType() {return type_;}
    string getName() {return name_;}
    string getModelid() {return modelid_;}
    string getSwversion() {return swversion_;}
    string getUniqueid() {return uniqueid_;}
    struct state_t getState() {return state_;}

    //SETTER METHODS
    void setType(string type) {type_ = type;}
    void setName(string name) {name_ = name;}
    void setModelid(string modelid) {modelid_ = modelid;}
    void setSwversion(string swversion) {swversion_ = swversion;}
    void setUniqueid(string uniqueid) {uniqueid_ = uniqueid;}
    void setState(struct state_t state) {state_ = state;}

private:
    string type_;
    string name_;
    string modelid_;
    string swversion_;
    string uniqueid_;
    struct state_t state_;
};

#endif //LIGHT_H
