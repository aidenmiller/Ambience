#ifndef Bridge_H
#define Bridge_H

#include <Wt/WResource>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WBreak>
#include <Wt/WLengthValidator>
#include <Wt/WRegExpValidator>
#include <iostream>
#include <regex>
#include <Wt/Http/Response>
#include <Wt/Http/Client>
#include <sstream>
#include <fstream>
#include <string>
#include <Wt/Json/Object>

using namespace std;
using namespace Wt;

class Bridge {

public:
    Bridge() {
        bridgename_ = "Default Bridgename";
        location_ = "Default Location";
        ip_ = "0.0.0.0";
        port_ = "00";
        username_ = "Default Username";
    };
    Bridge(string name, string location, string ip,
           string port, string username = "newdeveloper");

    virtual ~Bridge();

    bool writeBridge(string data);
    bool readBridge(string url);

    //GETTER METHODS
    string getName() {return bridgename_;}
    string getLocation() {return location_;}
    string getIP() {return ip_;}
    string getPort() {return port_;}
    string getUsername() {return username_;}

    //SETTER METHODS
    void setName(string name) {bridgename_ = name;}
    void setLocation(string location) {location_ = location;}
    void setIP(string ip) {ip_ = ip;}
    void setPort(string port) {port_ = port;}
    void setUsername(string username) {username_ = username;}

private:
    string bridgename_;
    string location_;
    string ip_;
    string port_;
    string username_;
};

#endif
