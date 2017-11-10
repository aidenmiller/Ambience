 /**
 *  @file       LoginWidget.cpp
 *  @author     CS 3307 - Team 13
 *  @date       11/7/2017
 *  @version    1.0
 *
 *  @brief      CS 3307, Hue Light Application screen for login
 *
 *  @section    DESCRIPTION
 *
 *              This class represents the login screen. This screen accepts a username,
 *              password and confirms authentication.
 *
 */


#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WText>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <string>
#include <stdio.h>
#include <fstream>
#include <openssl/sha.h>

#include "LoginWidget.h"
#include "Hash.h"
#include "Account.h"
#include "Bridge.h"

using namespace Wt;
using namespace std;

/**
*   @brief  Login Widget constructor
*
*   @param  *parent is a pointer the the containerwidget that stores this widget
*   @param  *main is a pointer to the app's welcome screen
*/

LoginWidget::LoginWidget(WContainerWidget *parent, Account *account, WelcomeScreen *main):
  WContainerWidget(parent)
{
    setContentAlignment(AlignCenter);
    parent_ = main;
    account_ = account;
}

/**
*   @brief  Update function, clears the widget and re-populates with elements of the login
*           screen
*
*   @return  void
*
*/

void LoginWidget::update()
{
    clear(); // everytime you come back to page, reset the widgets

    // Page title
    WText *title = new WText("Login to account", this);
    title->setStyleClass("title");

    new WBreak(this);

    // Username box: enter a username that is a valid email address
    new WText("User ID: ", this);
    idEdit_ = new WLineEdit();
    addWidget(idEdit_);
    new WBreak(this);

    // Password box: enter a valid password
    new WText("Password: ", this);
    pwEdit_ = new WLineEdit();
    pwEdit_->setEchoMode(WLineEdit::EchoMode::Password); // hide password as you type and replace with *****
    addWidget(pwEdit_);
    new WBreak(this);

    // login with provided user and password
    loginButton_ = new WPushButton("Login");
    addWidget(loginButton_);
    new WBreak(this);

    // submit user details to log in
    loginButton_->clicked().connect(this, &LoginWidget::submit);

    // message that warns user incorrect user or password when login fails
    statusMessage_ = new WText("Incorrect username or password!",this);
    statusMessage_->setStyleClass("error");
    statusMessage_->setHidden(true); // becomes visible if wrong credentials
}

/**
*   @brief  submit() function, triggered when user presses login button, displays
*           any applicable warning messages and/or checks user database for authentication.
*           Finally, it redirects to the bridge screen after successful login.
*/
void LoginWidget::submit(){
    // checks if provided user account and password exist, shows error message if not found
    if(!LoginWidget::checkCredentials(idEdit_->text().toUTF8(),pwEdit_->text().toUTF8())){
        statusMessage_->setHidden(false);
    }
    else { // if successful, redirects to bridge page
        account_->setAuth(true);
        parent_->handleInternalPath("/bridges");
    }
}

/**
*   @brief  checkCredentials() function, checks for username.txt, then compares
*           encrypted version of the user's password
*   @param  username is a string representing the user's inputted username
*   @param  password is a string representing the user's inputted password
*   @return bool representing if login successful
*/
bool LoginWidget::checkCredentials(string username, string password) {
    ifstream inFile;
    string str;
    string filename = "credentials/" + username + ".txt";
    inFile.open(filename.c_str()); // opens username.txt
    string hashedPW = Hash::sha256_hash(password); // cryptographically hash password

    if (!inFile) {
        return(false); // file not found
    }

    getline(inFile, str); //read hashed password from username.txt
    if(str.compare(hashedPW) == 0) {
        account_->setPassword(hashedPW);
        account_->setEmail(username);
        getline(inFile, str);
        account_->setFirstName(str);
        getline(inFile, str);
        account_->setLastName(str);
        while(getline(inFile, str)) {
            Bridge *bridge = new Bridge();
            if(bridge->readBridge(str))
                account_->addBridge(*bridge);
        }
        account_->writeFile(); //quick fix
        inFile.close();
        return true;
    }
    inFile.close();
    return false;
}
