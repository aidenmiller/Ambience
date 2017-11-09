/**
 *  @file       CreateAccountWidget.cpp
 *  @author     CS 3307 - Team 13
 *  @date       11/7/2017
 *  @version    1.0
 *
 *  @brief      CS 3307, Hue Light Application screen for user account creation
 *
 *  @section    DESCRIPTION
 *
 *              This class represents the user account creation screen, before login
 *              the user can come to this screen or the login screen. This screen accepts
 *              a username, password, confirms password. And then stores the user's credentials
 *              to a file.
 *
 */


#include <Wt/WText>
#include <Wt/WTable>
#include <fstream> // writing new accounts to a file
#include "CreateAccountWidget.h"
#include "Hash.h" // for password encryption

using namespace Wt;
using namespace std;

/**
 *   @brief  Create Account Widget constructor
 *
 *   @param  *parent is a pointer the the containerwidget that stores this widget
 *   @param  *main is a pointer to the app's welcome screen
 */
CreateAccountWidget::CreateAccountWidget(WContainerWidget *parent, WelcomeScreen *main):
WContainerWidget(parent)
{
    setContentAlignment(AlignCenter);
    parent_ = main;
}


/**
 *   @brief  Update function, clears the widget and re-populates with elements of the account
 *           creation screen
 *
 *   @return  void
 *
 */
void CreateAccountWidget::update()
{
    clear(); // everytime you come back to page, reset the widgets

        // Page title
    WText *title = new WText("Create an account", this);
    title->setStyleClass("title");

    new WBreak(this);

    // Username box: create a username that is a valid email address
    new WText("User ID: ", this);
    username_ = new WLineEdit();
    username_->setTextSize(18); // to hold placeholder text
    username_->setPlaceholderText("person@xyz.com"); // placeholder text to increase intuitive-ness of application
    addWidget(username_);
    new WBreak(this);

    // username must be a valid email address
    usernameValidator_ = new WRegExpValidator("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}", this);
    usernameValidator_->setInvalidNoMatchText("Username must be valid email address");
    username_->setValidator(usernameValidator_);

    new WText("First Name: ", this);
    firstName_ = new WLineEdit();
    firstName_->setTextSize(18);
    addWidget(firstName_);
    new WBreak(this);


    new WText("Last Name: ", this);
    lastName_ = new WLineEdit();
    lastName_->setTextSize(18);
    addWidget(lastName_);
    new WBreak(this);


    // Password box: create a valid password
    new WText("Password: ", this);
    password_ = new WLineEdit();
    password_->setEchoMode(WLineEdit::EchoMode::Password); // hide password as you type and replace with *****
    addWidget(password_);

    // password must be 6-20 characters in length
    passwordLengthValidator_ = new WLengthValidator(6, 20, this);
    passwordLengthValidator_->setInvalidTooShortText("Password must be at least 6 characters");
    passwordLengthValidator_->setInvalidTooLongText("Password must be max 20 characters");
    password_->setValidator(passwordLengthValidator_);
    new WBreak(this);

    // user must confirm their password, and they must equal eachother
    new WText("Confirm Password: ", this);
    confirmPassword_ = new WLineEdit();
    confirmPassword_->setEchoMode(WLineEdit::EchoMode::Password);
    addWidget(confirmPassword_);
    new WBreak(this);


    // submit user details to create account
    createAccountButton_ = new WPushButton("Create Account");
    addWidget(createAccountButton_);
    new WBreak(this);

    // if passwords dont match show a warning text
    unsuccessfulPassword_ = new WText("Sorry, passwords do not match");
    unsuccessfulPassword_->setStyleClass("error");
    addWidget(unsuccessfulPassword_);
    unsuccessfulPassword_->setHidden(true); // password match warning text hidden by default
    unsuccessfulInput_ = new WText("Sorry, username/password invalid"); // if password not 6-20 characters or username does not follow name@domain.tld
    unsuccessfulInput_->setStyleClass("error");
    addWidget(unsuccessfulInput_);
    unsuccessfulInput_->setHidden(true);

    // user create account button connects with helper function
    createAccountButton_->clicked().connect(this, &CreateAccountWidget::submit); // run login function when button is clicked

}


/**
 *   @brief  submit() function, triggered when user presses create account button, displays
 *           any applicable warning messages and/or triggers the account credentials write-to-file.
 *           Finally, it redirects to the login screen.
 */
void CreateAccountWidget::submit(){


    // If currently showing the unsuccessful password/user text or the password not matching text, erase it
    if (!unsuccessfulPassword_->isHidden())
        unsuccessfulPassword_->setHidden(true);
    if (!unsuccessfulInput_->isHidden())
        unsuccessfulInput_->setHidden(true);
    if (!CreateAccountWidget::validatePassword()) { // if the password != confirmed password
        unsuccessfulPassword_->setHidden(false); // show user that passwords don't match
    }
    else if (!CreateAccountWidget::validateInputFields()) {
        unsuccessfulInput_->setHidden(false);
    }
    else { // if password and confirmed password match

        CreateAccountWidget::writeUserInfo(username_->text().toUTF8(), password_->text().toUTF8(), firstName_->text().toUTF8(), lastName_->text().toUTF8()); // write the user's credentials to file
        parent_->handleInternalPath("/login"); // go back to login screen and change the path back to /login

    }

}

/**
 *   @brief  writeCredentials() function, writes an encrypted version of the user's password
 *           into username.txt
 *   @param  username is a string representing the user's inputted username
 *   @param  password is a string representing the user's inputted password
 */
void CreateAccountWidget::writeUserInfo(string username, string password, string firstName, string lastName) {

    // creates credentials folder if one does not exist
    const int dir_err = system("mkdir -p credentials");
    if (-1 == dir_err)
    {
        cout << "ERROR - Could not create directory\n";
        exit(1);
    }

    ofstream writefile;
    string file = "credentials/" + username+".txt"; // password will be stored in username.txt

    writefile.open(file.c_str());

    writefile << Hash::sha256_hash(password) <<endl; // cryptographically hash password

    writefile<< firstName << endl;
    writefile<< lastName << endl;

    writefile.close();
    //TODO: Error handling in the file write

}

/**
 *   @brief  validatePassword() function, confirms the the user's inputted password == inputted confirmed password
 *   @return bool representing if the passwords are the same or not
 */
bool CreateAccountWidget::validatePassword() {
    return password_->text() == confirmPassword_->text(); // password must equal password in the confirm password box
}

/**
 *   @brief  validateInputFields(): ensures that username is valid email address and pass is between 6-20 chars
 *   @return bool true if the input fields are valid, false otherwise
 */
bool CreateAccountWidget::validateInputFields() {
    return username_->validate() && password_->validate(); //username must be valid user@domain.tld and password 6-20 chars in length
}
