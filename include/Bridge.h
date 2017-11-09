/*
 * CS3307a Assignment 1
 *
 * Bridge.h
 * Header class for Bridge
 *
 * @author John Abed
 * @date 30/09/2017
 */

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
        name_ = "Default";
        ip_ = "0.0.0.0";
        port_ = "00";
        username_ = "default";
    };
    Bridge(string name, string ip, string port, string username = "newdeveloper");

    virtual ~Bridge();

    bool writeBridge(string email, string data);
    bool readBridge(string url);

    //GETTER METHODS
    string getName() {return name_;}
    string getIP() {return ip_;}
    string getPort() {return port_;}
    string getUsername() {return username_;}

    //SETTER METHODS
    void setName(string name) {name_ = name;}
    void setIP(string ip) {ip_ = ip;}
    void setPort(string port) {port_ = port;}
    void setUsername(string username) {username_ = username;}

private:
    string name_;
    string ip_;
    string port_;
    string username_;
};

#endif
