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
createScreen_(0),
loginScreen_(0),
bridgeScreen_(0),
profileScreen_(0),
account_("","","","") {
    //Logged Out Navigation Bar - Login, Create Account
    loggedOutNavBar_ = new WNavigationBar(this);
    loggedOutNavBar_->setTitle("Ambience");
    loggedOutNavBar_->setResponsive(true);
    WMenu *loggedOutLeftMenu = new WMenu();
    loggedOutNavBar_->addMenu(loggedOutLeftMenu);

    loginMenuItem_ = new WMenuItem("Login");
    loginMenuItem_->setLink(WLink(WLink::InternalPath, "/login"));
    loggedOutLeftMenu->addItem(loginMenuItem_);
    
    createMenuItem_ = new WMenuItem("Create Account");
    createMenuItem_->setLink(WLink(WLink::InternalPath, "/create"));
    loggedOutLeftMenu->addItem(createMenuItem_);
    
    
    //Logged In Navigation Bar - Profile, Bridges, Logout
    loggedInNavBar_ = new WNavigationBar(this);
    loggedInNavBar_->setTitle("Ambience");
    loggedInNavBar_->setResponsive(true);
    //Left Menu contains Profile and Bridges
    WMenu *loggedInLeftMenu = new WMenu();
    loggedInNavBar_->addMenu(loggedInLeftMenu);
    profileMenuItem_ = new WMenuItem("Profile");
    profileMenuItem_->setLink(WLink(WLink::InternalPath, "/profile"));
    loggedInLeftMenu->addItem(profileMenuItem_);
    bridgesMenuItem_ = new WMenuItem("Bridges");
    bridgesMenuItem_->setLink(WLink(WLink::InternalPath, "/bridges"));
    loggedInLeftMenu->addItem(bridgesMenuItem_);
    //Right Menu contains Logout button
    WMenu *loggedInRightMenu = new WMenu();
    loggedInNavBar_->addMenu(loggedInRightMenu, AlignmentFlag::AlignRight);
    logoutMenuItem_ = new WMenuItem("Logout");
    logoutMenuItem_->setLink(WLink(WLink::InternalPath, "/logout"));
    loggedInRightMenu->addItem(logoutMenuItem_);
    
    //hide loggedInNavBar_ as this is the logged in menu
    loggedInNavBar_->setHidden(true);
    
    serverMessage_ = new WText("You are connected to the Team 13 Production Server", this);
    new WBreak(this);
    
    welcome_image_ = new WImage(WLink("images/login_lights.jpeg"));
    addWidget(welcome_image_);
    
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
void WelcomeScreen::handleInternalPath(const string &internalPath) {
    if (account_.isAuth()) {
        welcome_image_->setHidden(true);
        loggedOutNavBar_->setHidden(true);
        loggedInNavBar_->setHidden(false);
        serverMessage_->setHidden(true);
        
        updateProfileName();
        
        regex re("/bridges/(\\d{1,3})");
        
        if (internalPath == "/bridges") { // opens bridge page
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
            account_.logout();
            logoutMenuItem_->renderSelected(false);
            WApplication::instance()->setInternalPath("/login", true);
        }
    }
    else {
        welcome_image_->setHidden(true);
        loggedOutNavBar_->setHidden(false);
        loggedInNavBar_->setHidden(true);
        
        serverMessage_->setHidden(false);
        
        if (internalPath == "/create") {// opens create page
            createAccountScreen();
        }
        else if (internalPath == "/login") { // opens login page
            loginScreen();
        }
    }
}

void WelcomeScreen::lightManagementScreen(int index) {
    Bridge *bridge = account_.getBridgeAt(index);
    //WApplication::instance()->setInternalPath("/bridges/" + to_string(index), true);
    
    if (lightManage_[index] == NULL) {
        lightManage_[index] = new LightManagementWidget(mainStack_, bridge, this);
    }
    mainStack_->setCurrentWidget(lightManage_[index]);
    bridgesMenuItem_->renderSelected(true);
    lightManage_[index]->update();
}

/**
 *   @brief  Create Account function, updates the page to the
 *           creation screen
 *   @return void
 */
void WelcomeScreen::createAccountScreen() {
    if (!createScreen_) {
        createScreen_ = new CreateAccountWidget(mainStack_, &account_, this);
    }
    mainStack_->setCurrentWidget(createScreen_);
    createMenuItem_->renderSelected(true);
    createScreen_->update();
}

/**
 *   @brief  profile screen function, updates the page to the
 *           profile management screen
 *   @return void
 */
void WelcomeScreen::profileScreen() {
    if (!profileScreen_) {
        profileScreen_ = new ProfileWidget(mainStack_, &account_, this);
    }
    mainStack_->setCurrentWidget(profileScreen_);
    profileMenuItem_->renderSelected(true);
    profileScreen_->update();
}

/**
 *   @brief  Login function, updates the page to the
 *           login screen
 *   @return void
 */
void WelcomeScreen::loginScreen() {
    if (!loginScreen_) {
        loginScreen_ = new LoginWidget(mainStack_, &account_, this);
    }
    mainStack_->setCurrentWidget(loginScreen_);
    loginMenuItem_->renderSelected(true);
    loginScreen_->update();
}

/**
 *   @brief  Bridge function, updates the page to the
 *           bridge control screen
 *   @return void
 */
void WelcomeScreen::bridgeScreen() {
    if (!bridgeScreen_) {
        bridgeScreen_ = new BridgeScreenWidget(mainStack_, &account_, this);
    }
    mainStack_->setCurrentWidget(bridgeScreen_);
    bridgesMenuItem_->renderSelected(true);
    bridgeScreen_->update();
}

/**
 *   @brief  Updates name of logged in user on the top menu bar
 *
 *   @return void
 */
void WelcomeScreen::updateProfileName() {
    profileMenuItem_->setText("Profile (" + account_.getFirstName() + " " + account_.getLastName() + ")");
}


