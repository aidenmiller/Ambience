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
 *              This class represents the profile management screen, the user can change their name or password
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

    //shows the user's first name, and allows them to click to edit the name
    new WText("First Name: ", this);
    editableFirstName_ = new WInPlaceEdit(account_->getFirstName(), this);
    editableFirstName_->setPlaceholderText("Enter your first name...");

    editableFirstName_->valueChanged().connect(this, &ProfileWidget::updateFirstName); // if you change the value of first name, call the updatefirstname function
    new WBreak(this);

    // shows the user's last name, and allows them to click to edit the name
    new WText("Last Name: ", this);
    editableLastName_ = new WInPlaceEdit(account_->getLastName(), this);
    editableLastName_->setPlaceholderText("Enter your last name...");
    editableLastName_->valueChanged().connect(this, &ProfileWidget::updateLastName); // if you change the value of the last name, call the updatelastname function

    //special CSS for the edit-able elements to intuitively show user that it can be edited
    editableFirstName_->setStyleClass("inplace");
    editableLastName_->setStyleClass("inplace");

    new WBreak(this);

    // button to launch the update password dialog box
    updatePassword_ = new WPushButton("Update password");
    addWidget(updatePassword_);

    new WBreak(this);

    // error and success messages hidden by default
    passwordError_ = new WText();

    passwordError_->setStyleClass("error");
    passwordError_->setHidden(true);
    addWidget(passwordError_);

    passwordSuccess_ = new WText("Successfully updated password!");
    passwordSuccess_->setHidden(true);
    addWidget(passwordSuccess_);

    // if you click the update password button, it shows the dialog box
    updatePassword_->clicked().connect(this, &ProfileWidget::showPasswordDialog);

}

/**
*   @brief  updateFirstName function, called if user puts in a new first name, calls the setter function of Account
*           screen
*   @return  void
*/
void ProfileWidget::updateFirstName() {
    account_->setFirstName(editableFirstName_->text().toUTF8()); // call setter to change account first name
    account_->writeFile(); //update credentials file
    parent_->updateProfileName(); //display new name
}


/**
*   @brief  updateLastName function, called if user puts in a new last name, calls the setter function of Account
*           screen
*   @return  void
*/
void ProfileWidget::updateLastName() {
    account_->setLastName(editableLastName_->text().toUTF8()); // call setter to change the account last name
    account_->writeFile(); //update credentials file
    parent_->updateProfileName(); //display new name
}

/**
*   @brief  showPasswordDialog function, called if user chooses to update their password.
*           Opens a WDialog box asking them to confirm their current password, and choose a new password
*           , goes through same validation process as the create account page
*   @return  void
*/
void ProfileWidget::showPasswordDialog() {

    passwordDialog_ = new WDialog("Update Password"); // title


    // user enter's their current password
    WLabel *passLabel = new WLabel("Current Password: ", passwordDialog_->contents());
    currentPass_ = new WLineEdit(passwordDialog_->contents());
    currentPass_->setEchoMode(WLineEdit::EchoMode::Password); // star **** out the password text

    passLabel->setBuddy(currentPass_);

    new WBreak(passwordDialog_->contents());

    // user enter's their desired new password
    WLabel *newPassLabel = new WLabel("New Password: ", passwordDialog_->contents());
    newPass_ = new WLineEdit(passwordDialog_->contents());
    newPass_->setEchoMode(WLineEdit::EchoMode::Password);



     // new password must be 6-20 characters in length
    passwordLengthValidator_ = new WLengthValidator(6, 20, this);
    passwordLengthValidator_->setInvalidTooShortText("Password must be at least 6 characters");
    passwordLengthValidator_->setInvalidTooLongText("Password must be max 20 characters");
    newPass_->setValidator(passwordLengthValidator_);


    newPassLabel->setBuddy(newPass_);
    new WBreak(passwordDialog_->contents());

    // user must confirm their new password
    WLabel *confirmNewPassLabel = new WLabel("Confirm new Password: ", passwordDialog_->contents());
    confirmNewPass_ = new WLineEdit(passwordDialog_->contents());
    confirmNewPass_->setEchoMode(WLineEdit::EchoMode::Password);

    // the new password cannot be empty (WLengthValidator does not validate for empty string)
    inputNotEmpty_ = new WValidator(this);
    inputNotEmpty_->setMandatory(true);
    confirmNewPass_->setValidator(inputNotEmpty_);

    confirmNewPassLabel->setBuddy(confirmNewPass_);
    new WBreak(passwordDialog_->contents());


    // make okay and cancel buttons, cancel sends a reject dialogstate, okay sends an accept
    WPushButton *ok = new WPushButton("OK", passwordDialog_->contents());
    WPushButton *cancel = new WPushButton("Cancel", passwordDialog_->contents());


    ok->clicked().connect(passwordDialog_, &WDialog::accept);
    cancel->clicked().connect(passwordDialog_, &WDialog::reject);

    // when the user is finished, call the updatePassword function
    passwordDialog_->finished().connect(this, &ProfileWidget::updatePassword);
    passwordDialog_->show();
}

/**
*   @brief  updatePassword function, called upon submission of the update password dialog box. Validates password
*           and sets the Account's new password using Account setter
*   @return  void
*/
void ProfileWidget::updatePassword() {

    // if user clicked "cancel" in dialog box, don't do anything
    if (passwordDialog_->result() == WDialog::DialogCode::Rejected)
        return;

    // otherwise, update their password and do validation
    string currentPassword = currentPass_->text().toUTF8();
    string newPassword = newPass_->text().toUTF8();
    string confirmNewPassword = confirmNewPass_->text().toUTF8();

    // error/success messages hidden by default
    passwordError_->setHidden(true);
    passwordSuccess_->setHidden(true);

    if (checkValidPassword(currentPassword)) { // if the password they entered as past password is correct
        if (newPass_->validate() && (!confirmNewPass_->text().toUTF8().compare("") == 0)) { // if length requirements of new password are met
            if (newPassword.compare(confirmNewPassword) == 0) { // if password is equal to the confirmed password
                account_->setPassword(Hash::sha256_hash(newPassword)); // then hash their inputed password, and update Account details
                account_->writeFile(); //update credentials file
                passwordSuccess_->setHidden(false); // show the success message
                }
            else {
                passwordError_->setText("Sorry, the passwords you have entered do not match."); // else show error message
                passwordError_->setHidden(false);
            }
        }
        else {
            passwordError_->setText("New password must be 6-20 characters in length"); // else show error message
            passwordError_->setHidden(false);
        }
    }
    else {
        passwordError_->setText("You have not entered a valid password"); // else show error message
        passwordError_->setHidden(false);
    }
}

/**
*   @brief  checkValidPassword function, ensures that the password that the user has entered is the password connected to their account
*   @return  bool true if the password is correct, false otherwise
*/
bool ProfileWidget::checkValidPassword(string password) {
    string hashedPass = Hash::sha256_hash(password); // hash pass and compare to file
    return (hashedPass.compare(account_->getPassword()) == 0);
}

