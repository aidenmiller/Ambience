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
#include "WelcomeScreen.h"
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

class Bridge: public Wt::WContainerWidget {

public:
    Bridge(string name, string ip, string port, string username = "newdeveloper", Wt::WContainerWidget *parent = 0,
           WelcomeScreen *main = 0);

    virtual ~Bridge();

    bool writeBridge(string data);
    bool readBridge(string url);

    //GETTER METHODS
    string getName();
    string getIP();
    string getPort();
    string getUsername();

    //SETTER METHODS
    void setName(string bName);
    void setIP(string bIP);
    void setPort(string bPort);
    void setUsername(string bUsername);

    void connect();

private:
    void handleHttpResponse(boost::system::error_code err,
                            const Wt::Http::Message &response);

    WelcomeScreen *parent_;
    string name_;
    string ip_;
    string port_;
    string username_;
};

#endif
