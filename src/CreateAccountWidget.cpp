#include <Wt/WText>
#include <Wt/WTable>
#include <fstream>
#include <openssl/sha.h>

#include "CreateAccountWidget.h"
#include "Hash.h"

using namespace Wt;
using namespace std;

CreateAccountWidget::CreateAccountWidget(WContainerWidget *parent):
  WContainerWidget(parent)
{
  setContentAlignment(AlignCenter);
}

void CreateAccountWidget::update()
{
    clear();

    new WText("User ID: ", this);
    username_ = new WLineEdit();
    username_->setTextSize(18);
    username_->setPlaceholderText("person@xyz.com");
    addWidget(username_);
    new WBreak(this);

    usernameValidator_ = new WRegExpValidator("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}", this);
    usernameValidator_->setInvalidNoMatchText("Username must be valid email address");
    username_->setValidator(usernameValidator_);


    new WText("Password: ", this);
    password_ = new WLineEdit();
    password_->setEchoMode(WLineEdit::EchoMode::Password);
    addWidget(password_);

    passwordLengthValidator_ = new WLengthValidator(6, 20, this);
    passwordLengthValidator_->setInvalidTooShortText("Password must be at least 6 characters");
    passwordLengthValidator_->setInvalidTooLongText("Password must be max 20 characters");
    password_->setValidator(passwordLengthValidator_);
    new WBreak(this);

    new WText("Confirm Password: ", this);
    confirmPassword_ = new WLineEdit();
    confirmPassword_->setEchoMode(WLineEdit::EchoMode::Password);
    addWidget(confirmPassword_);
    new WBreak(this);


    createAccountButton_ = new WPushButton("Create Account");
    addWidget(createAccountButton_);
    new WBreak(this);
    unsuccessfulPassword_ = new WText("Sorry, passwords do not match");
    addWidget(unsuccessfulPassword_);
    unsuccessfulPassword_->setHidden(true);

    createAccountButton_->clicked().connect(this, &CreateAccountWidget::submit); // run login function when button is clicked

}

void CreateAccountWidget::submit(){

    if (!CreateAccountWidget::validatePassword()) {
        unsuccessfulPassword_->setHidden(false);
      }
    else {
        if (!unsuccessfulPassword_->isHidden())
                unsuccessfulPassword_->setHidden(true);

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
