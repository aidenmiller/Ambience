#include <Wt/WText>
#include <Wt/WTable>
#include <fstream> // writing new accounts to a file
#include "CreateAccountWidget.h"
#include "Hash.h" // for password encryption

using namespace Wt;
using namespace std;


CreateAccountWidget::CreateAccountWidget(WContainerWidget *parent):
  WContainerWidget(parent)
{
  setContentAlignment(AlignCenter);
}

void CreateAccountWidget::update()
{
    clear(); // everytime you come back to page, reset the widgets

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


    unsuccessfulPassword_ = new WText("Sorry, passwords do not match");
    addWidget(unsuccessfulPassword_);
    unsuccessfulPassword_->setHidden(true);
    unsuccessfulInput_ = new WText("Sorry, username/password invalid");
    addWidget(unsuccessfulInput_);
    unsuccessfulInput_->setHidden(true);

    // user create account button connects with helper function
    createAccountButton_->clicked().connect(this, &CreateAccountWidget::submit); // run login function when button is clicked

}


void CreateAccountWidget::submit(){

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

        CreateAccountWidget::writeCredentials(username_->text().toUTF8(), password_->text().toUTF8());

    }

}

int CreateAccountWidget::writeCredentials(string username, string password) {
  ofstream writefile;
  string file = "credentials/" + username+".txt";

  writefile.open(file.c_str());

  writefile << Hash::sha256_hash(password);

  writefile.close();

  return 1;
}

bool CreateAccountWidget::validatePassword() {
    return password_->text() == confirmPassword_->text();
}

bool CreateAccountWidget::validateInputFields() {
    return username_->validate() && password_->validate();
}
