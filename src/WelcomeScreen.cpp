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
#include <regex>
#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WMenu>
#include <Wt/WTableCell>
#include <Wt/WServer>
#include <Wt/WStackedWidget>
#include "CreateAccountWidget.h"
#include "LoginWidget.h"
#include "BridgeScreenWidget.h"
#include "ProfileWidget.h"
#include "LightManagementWidget.h"

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
    loggedOutNavBar_ = new WNavigationBar(this);
    loggedOutNavBar_->setTitle("Smart Lights");
    loggedOutNavBar_->setResponsive(true);
    WMenu *loggedOutLeftMenu = new WMenu();
    loggedOutNavBar_->addMenu(loggedOutLeftMenu);
    loggedOutLeftMenu->addItem("Login")->setLink(WLink(WLink::InternalPath, "/login"));
    loggedOutLeftMenu->addItem("Create Account")->setLink(WLink(WLink::InternalPath, "/create"));

    navBar_ = new WNavigationBar(this);
    navBar_->setTitle("Smart Lights");
    navBar_->setResponsive(true);


    leftMenu_ = new WMenu();
    navBar_->addMenu(leftMenu_);
    profileMenuItem_ = new WMenuItem("Profile");
    profileMenuItem_->setLink(WLink(WLink::InternalPath, "/profile"));
    leftMenu_->addItem(profileMenuItem_);
    leftMenu_->addItem("Bridges")->setLink(WLink(WLink::InternalPath, "/bridges"));

    rightMenu_ = new WMenu();
    navBar_->addMenu(rightMenu_, AlignmentFlag::AlignRight);
    rightMenu_->addItem("Logout")->setLink(WLink(WLink::InternalPath, "/logout"));
    navBar_->setHidden(true);


    serverMessage_ = new WText("You are connected to the Team 13 Production Server", this);

    mainStack_ = new WStackedWidget();
    addWidget(mainStack_);


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
    if (account_.isAuth()) {
        loggedOutNavBar_->setHidden(true);
        navBar_->setHidden(false);

        profileMenuItem_->setText(account_.getFirstName() + " " + account_.getLastName());
        serverMessage_->setText("Hello, " + account_.getFirstName() + " " + account_.getLastName());
        
        regex re("/bridges/(\\d{1,3})");
        
        if (internalPath == "/bridges") { // opens bridge page
            leftMenu_->select(1);
            bridgeScreen();
        }
        else if (regex_match(internalPath, re)) {
            smatch match;
            regex_match(internalPath, match, re);
            string result = match.str(1);
            lightManagementScreen(stoi(result));
        }
        else if (internalPath == "/profile") {
            profileScreen();
        }
        else if (internalPath == "/logout") {
            logout();
        }
        else  { // defaults to profile page
            WApplication::instance()->setInternalPath("/profile", true);
        }
    }
    else {
        loggedOutNavBar_->setHidden(false);
        navBar_->setHidden(true);
        
        serverMessage_->setText("You are connected to the Team 13 Production Server");
        
        if (internalPath == "/create") // opens create page
            createAccount();
        else if (internalPath == "/login") // opens login page
            login();
        else { // opens create page by default for any other path changes
            WApplication::instance()->setInternalPath("/create", true);
        }
    }
}

void WelcomeScreen::lightManagementScreen(int index) {
    WApplication::instance()->setInternalPath("/bridges/" + to_string(index), true);
    if (lightManage_[index] == NULL) {
        lightManage_[index] = new LightManagementWidget(mainStack_, &account_, this);
    }
    mainStack_->setCurrentWidget(lightManage_[index]);
    lightManage_[index]->update();
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
    WApplication::instance()->setInternalPath("/bridges", true);

    if (!bridgeScreen_) {
        bridgeScreen_ = new BridgeScreenWidget(mainStack_, &account_, this);
    }
    mainStack_->setCurrentWidget(bridgeScreen_);
    bridgeScreen_->update();
}








