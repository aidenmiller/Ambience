 /**
 *  @file       ProfileWidget.cpp
 *  @author     CS 3307 - Team 13
 *  @date       11/7/2017
 *  @version    1.0
 *
 *  @brief      CS 3307, Hue Light Application screen for user profile management
 *
 *  @section    DESCRIPTION
 *
 *              This class represents the profile login screen. This screen accepts a username,
 *              password and confirms authentication.
 *
 */


#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WText>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WInPlaceEdit>
#include <string>
#include <stdio.h>
#include <fstream>
#include <openssl/sha.h>

#include "ProfileWidget.h"
#include "Account.h"

using namespace Wt;
using namespace std;

/**
*   @brief  Profile Widget constructor
*
*   @param  *parent is a pointer the the containerwidget that stores this widget
*   @param  *main is a pointer to the app's welcome screen
*/

ProfileWidget::ProfileWidget(WContainerWidget *parent, Account *account, WelcomeScreen *main):
  WContainerWidget(parent)
{
    setContentAlignment(AlignCenter);
    parent_ = main;
    account_ = account;
}

/**
*   @brief  Update function, clears the widget and re-populates with elements of the profile
*           screen
*
*   @return  void
*
*/

void ProfileWidget::update()
{
    clear(); // everytime you come back to page, reset the widgets

    // Page title
    WText *title = new WText("Your Profile", this);
    title->setStyleClass("title");
    new WBreak(this);
    new WBreak(this);

    new WText("First Name: ", this);
    editableFirstName_ = new WInPlaceEdit(account_->getFirstName(), this);
    editableFirstName_->setPlaceholderText("Enter your first name...");

    editableFirstName_->valueChanged().connect(this, &ProfileWidget::updateFirstName);

    new WBreak(this);
    new WText("Last Name: ", this);
    editableLastName_ = new WInPlaceEdit(account_->getLastName(), this);
    editableLastName_->setPlaceholderText("Enter your last name...");
    editableLastName_->valueChanged().connect(this, &ProfileWidget::updateLastName);
}

void ProfileWidget::updateFirstName() {
    account_->setFirstName(editableFirstName_->text().toUTF8());
}

void ProfileWidget::updateLastName() {
    account_->setLastName(editableLastName_->text().toUTF8());
}
