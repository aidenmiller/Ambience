/**
 *  @file       WelcomeScreen.cpp
 *  @author     CS 3307 - Team 13
 *  @date       10/7/2017
 *  @version    1.0
 *
 *  @brief      CS 3307, Hue Light Application main screen which controls the user's view
 *
 *  @section    DESCRIPTION
 *
 *              TO BE FILLED
 *
 */

#include <string>
#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WTableCell>
#include <Wt/WServer>
#include <Wt/WStackedWidget>
#include <stdio.h>
#include "Account.h"
#include "CreateAccountWidget.h"
#include "LoginWidget.h"
#include "WelcomeScreen.h"
#include "BridgeScreenWidget.h"

using namespace Wt;
using namespace std;

WelcomeScreen::WelcomeScreen(WContainerWidget *parent):
WContainerWidget(parent),
create_(0),
login_(0),
bridgeScreen_(0)
{
    serverMessage_ = new WText("You are connected to the Team 13 Production Server", this);
    mainStack_ = new WStackedWidget();
    addWidget(mainStack_);

    links_ = new WContainerWidget();
    links_->show();
    addWidget(links_);

    createAnchor_ = new WAnchor("/create", "Create New Account ", links_);
    createAnchor_->setLink(WLink(WLink::InternalPath, "/create"));

    loginAnchor_ = new WAnchor("/login", "Login with Existing Account ", links_);
    loginAnchor_->setLink(WLink(WLink::InternalPath, "/login"));

    WApplication::instance()->internalPathChanged().connect(this, &WelcomeScreen::handleInternalPath);
}


void WelcomeScreen::handleInternalPath(const string &internalPath)
{
    if (true) { // change to if(loggedin = true)
        serverMessage_->setHidden(true);
        if (internalPath == "/create")
            createAccount();
        else if (internalPath == "/login")
            login();
        else if (internalPath == "/bridgescreen")
            bridgeScreen();
        else
            WApplication::instance()->setInternalPath("/create", true);
    }
}

void WelcomeScreen::createAccount()
{
    if (!create_) {
        create_ = new CreateAccountWidget(mainStack_, this);

    }

    mainStack_->setCurrentWidget(create_);

    create_->update();
}

void WelcomeScreen::login()
{
    WApplication::instance()->setInternalPath("/login", true);
    if (!login_) {
        login_ = new LoginWidget(mainStack_, this);
    }
    mainStack_->setCurrentWidget(login_);
    login_->update();
}

void WelcomeScreen::bridgeScreen()
{
    //john: this call is making the dashboard() funct run twice.. not good
    WApplication::instance()->setInternalPath("/bridgescreen", true);

    if (!bridgeScreen_) {
        bridgeScreen_ = new BridgeScreenWidget(mainStack_, this);
    }
    mainStack_->setCurrentWidget(bridgeScreen_);
    bridgeScreen_->update();
}
