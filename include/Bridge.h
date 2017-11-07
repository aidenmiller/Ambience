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
#include <string>

using namespace std;
using namespace Wt;

class Bridge: public Wt::WContainerWidget {

public:
    Bridge(Wt::WContainerWidget *parent = 0,
           WelcomeScreen *main = 0);
    
    virtual ~Bridge();
    
private:
    void handleHttpResponse(Wt::Http::Client *client,
                            boost::system::error_code err,
                            const Wt::Http::Message& response) const;
    WelcomeScreen *parent_;
};

#endif
