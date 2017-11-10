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
#include <Wt/WDialog>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WInPlaceEdit>
#include <Wt/WDialog>
#include <Wt/WLabel>
#include <string>
#include <stdio.h>
#include <fstream>
#include <openssl/sha.h>


#include "ProfileWidget.h"
#include "Account.h"
#include "Hash.h"

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

    editableFirstName_->setStyleClass("inplace");
    editableLastName_->setStyleClass("inplace");

    new WBreak(this);
    updatePassword_ = new WPushButton("Update password");
    addWidget(updatePassword_);

    new WBreak(this);
    passwordError_ = new WText();

    passwordError_->setStyleClass("error");
    passwordError_->setHidden(true);
    addWidget(passwordError_);

    passwordSuccess_ = new WText("Successfully updated password!");
    passwordSuccess_->setHidden(true);
    addWidget(passwordSuccess_);

    // submit user details to log in
    updatePassword_->clicked().connect(this, &ProfileWidget::showPasswordDialog);

}

void ProfileWidget::updateFirstName() {
    account_->setFirstName(editableFirstName_->text().toUTF8());
}

void ProfileWidget::updateLastName() {
    account_->setLastName(editableLastName_->text().toUTF8());
}

void ProfileWidget::showPasswordDialog() {
    passwordDialog_ = new WDialog("Update Password");


    WLabel *passLabel = new WLabel("Current Password: ", passwordDialog_->contents());

    currentPass_ = new WLineEdit(passwordDialog_->contents());
    currentPass_->setEchoMode(WLineEdit::EchoMode::Password);

    passLabel->setBuddy(currentPass_);

    new WBreak(passwordDialog_->contents());


    WLabel *newPassLabel = new WLabel("New Password: ", passwordDialog_->contents());
    newPass_ = new WLineEdit(passwordDialog_->contents());
    newPass_->setEchoMode(WLineEdit::EchoMode::Password);



     // password must be 6-20 characters in length
    passwordLengthValidator_ = new WLengthValidator(6, 20, this);
    passwordLengthValidator_->setInvalidTooShortText("Password must be at least 6 characters");
    passwordLengthValidator_->setInvalidTooLongText("Password must be max 20 characters");
    newPass_->setValidator(passwordLengthValidator_);



    newPassLabel->setBuddy(newPass_);
    new WBreak(passwordDialog_->contents());

    WLabel *confirmNewPassLabel = new WLabel("Confirm new Password: ", passwordDialog_->contents());
    confirmNewPass_ = new WLineEdit(passwordDialog_->contents());
    confirmNewPass_->setEchoMode(WLineEdit::EchoMode::Password);
    inputNotEmpty_ = new WValidator(this);
    inputNotEmpty_->setMandatory(true);
    confirmNewPass_->setValidator(inputNotEmpty_);

    confirmNewPassLabel->setBuddy(confirmNewPass_);
    new WBreak(passwordDialog_->contents());



    WPushButton *ok = new WPushButton("OK", passwordDialog_->contents());
    WPushButton *cancel = new WPushButton("Cancel", passwordDialog_->contents());

    newPass_->enterPressed().connect(passwordDialog_, &WDialog::accept);
    ok->clicked().connect(passwordDialog_, &WDialog::accept);
    cancel->clicked().connect(passwordDialog_, &WDialog::reject);

    passwordDialog_->finished().connect(this, &ProfileWidget::updatePassword);
    passwordDialog_->show();
}

void ProfileWidget::updatePassword() {
    string currentPassword = currentPass_->text().toUTF8();
    string newPassword = newPass_->text().toUTF8();
    string confirmNewPassword = confirmNewPass_->text().toUTF8();

    passwordError_->setHidden(true);
    passwordSuccess_->setHidden(true);

    if (checkValidPassword(currentPassword)) {
        if (newPass_->validate() && (!confirmNewPass_->text().toUTF8().compare("") == 0)) {
            if (newPassword.compare(confirmNewPassword) == 0) { // if password confirmed
                account_->setPassword(Hash::sha256_hash(newPassword));
                passwordSuccess_->setHidden(false);
                }
            else {
                passwordError_->setText("Sorry, the passwords you have entered do not match.");
                passwordError_->setHidden(false);
            }
        }
        else {
            passwordError_->setText("New password must be 6-20 characters in length");
            passwordError_->setHidden(false);
        }
    }
    else {
        passwordError_->setText("You have not entered a valid password");
        passwordError_->setHidden(false);
    }
}

bool ProfileWidget::checkValidPassword(string password) {
    string hashedPass = Hash::sha256_hash(password);
    return (hashedPass.compare(account_->getPassword()) == 0);
}

