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
 *              This class represents the main screen of the application. All internal path changes
 *              as well as any persistent widgets will be handled here.
 */

#include "WelcomeScreen.h"
#include <string>
#include <stdio.h>
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
#include "CreateAccountWidget.h"
#include "LoginWidget.h"
#include "BridgeScreenWidget.h"
#include "ProfileWidget.h"

using namespace Wt;
using namespace std;

/**
*   @brief  Main Screen constructor
*
*   @param  *parent is a pointer the the containerwidget that stores this widget
*/
WelcomeScreen::WelcomeScreen(WContainerWidget *parent):
WContainerWidget(parent),
create_(0),
login_(0),
bridgeScreen_(0),
profileScreen_(0),
account_("","","","")
{
    serverMessage_ = new WText("You are connected to the Team 13 Production Server", this);
    new WBreak(this);
    profileAnchor_ = new WAnchor("/profile", "Edit your profile", this);
    profileAnchor_->setLink(WLink(WLink::InternalPath, "/profile"));
    profileAnchor_->setHidden(true);

    homeAnchor_ = new WAnchor("/bridges", "Go back", this);
    homeAnchor_->setLink(WLink(WLink::InternalPath, "/bridges"));
    homeAnchor_->setHidden(true);

    mainStack_ = new WStackedWidget();
    addWidget(mainStack_);

    // links container
    links_ = new WContainerWidget();
    links_->show();
    addWidget(links_);

    // create anchor will re-direct to internal path /create when clicked, stored in links container
    createAnchor_ = new WAnchor("/create", "Create New Account ", links_);
    createAnchor_->setLink(WLink(WLink::InternalPath, "/create"));

    // login anchor will re-direct to internal path /login when clicked, stored in links container
    loginAnchor_ = new WAnchor("/login", "Login with Existing Account ", links_);
    loginAnchor_->setLink(WLink(WLink::InternalPath, "/login"));
    
    // login anchor will re-direct to internal path /login when clicked, stored in links container
    logoutAnchor_ = new WAnchor("/logout", "Logout", links_);
    logoutAnchor_->setLink(WLink(WLink::InternalPath, "/logout"));
    logoutAnchor_->setHidden(true);

    // detects any changes to the internal path and sends to the handle internal path function
    WApplication::instance()->internalPathChanged().connect(this, &WelcomeScreen::handleInternalPath);
}

/**
*   @brief  Handle Internal Path function, checks for any changes to the internal
*           path and redirects the page according to internalPath
*   @param  internalPath is the page name to be re-directed to
*   @return  void
*/
void WelcomeScreen::handleInternalPath(const string &internalPath)
{
    if (account_.isAuth()) { // change to if(loggedin = true)
        serverMessage_->setText("Hello, " + account_.getFirstName() + " " + account_.getLastName());
        profileAnchor_->setHidden(false);
        homeAnchor_->setHidden(true);
        createAnchor_->setHidden(true);
        loginAnchor_->setHidden(true);
        logoutAnchor_->setHidden(false);
        if (internalPath == "/bridges") // opens bridge page
            bridgeScreen();
        else if (internalPath == "/profile") {
            profileScreen();
            homeAnchor_->setHidden(false);
            profileAnchor_->setHidden(true);
        }
        else if (internalPath == "/logout") {
            logout();
        }
        else // opens create page by default for any other path changes
            WApplication::instance()->setInternalPath("/bridges", true);
    }
    else {
        serverMessage_->setText("You are connected to the Team 13 Production Server");
        createAnchor_->setHidden(false);
        loginAnchor_->setHidden(false);
        logoutAnchor_->setHidden(true);
        profileAnchor_->setHidden(true);
        homeAnchor_->setHidden(true);
        if (internalPath == "/create") // opens create page
            createAccount();
        else if (internalPath == "/login") // opens login page
            login();
        else
            WApplication::instance()->setInternalPath("/create", true);
    }
}

/**
*   @brief  Create Account function, updates the page to the
*           creation screen
*   @return void
*/
void WelcomeScreen::createAccount()
{
    if (!create_) {
        create_ = new CreateAccountWidget(mainStack_, &account_, this);
    }

    mainStack_->setCurrentWidget(create_);

    create_->update();
}

/**
*   @brief  profile screen function, updates the page to the
*           profile management screen
*   @return void
*/
void WelcomeScreen::profileScreen()
{
    if (!profileScreen_) {
        profileScreen_ = new ProfileWidget(mainStack_, &account_, this);
    }


    mainStack_->setCurrentWidget(profileScreen_);

    profileScreen_->update();
}

/**
*   @brief  Login function, updates the page to the
*           login screen
*   @return void
*/
void WelcomeScreen::login()
{
    WApplication::instance()->setInternalPath("/login", true);
    if (!login_) {
        login_ = new LoginWidget(mainStack_, &account_, this);
    }
    mainStack_->setCurrentWidget(login_);
    login_->update();
}

/**
 *   @brief  Logout function, updates the page to the
 *           login screen
 *   @return void
 */
void WelcomeScreen::logout()
{
    account_.logout();
    WApplication::instance()->setInternalPath("/login", true);
}

/**
*   @brief  Bridge function, updates the page to the
*           bridge control screen
*   @return void
*/
void WelcomeScreen::bridgeScreen()
{
    //john: this call is making the dashboard() funct run twice.. not good
    WApplication::instance()->setInternalPath("/bridges", true);

    if (!bridgeScreen_) {
        bridgeScreen_ = new BridgeScreenWidget(mainStack_, &account_, this);
    }
    mainStack_->setCurrentWidget(bridgeScreen_);
    bridgeScreen_->update();
}
