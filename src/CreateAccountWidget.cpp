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
    new WText("Create Account Page", this);
    new WText("User ID: ", this);
    username_ = new WLineEdit();
    addWidget(username_);
    new WBreak(this);

    new WText("Password: ", this);
    password_ = new WLineEdit();
    addWidget(password_);
    new WBreak(this);

    createAccountButton_ = new WPushButton("Create Account");
    addWidget(createAccountButton_);

    createAccountButton_->clicked().connect(this, &CreateAccountWidget::submit); // run login function when button is clicked
}

void CreateAccountWidget::submit(){

    CreateAccountWidget::writeCredentials(username_->text().toUTF8(), password_->text().toUTF8());

}

int CreateAccountWidget::writeCredentials(string username, string password) {
  ofstream writefile;
  string file = "credentials/" + username+".txt";

  writefile.open(file.c_str());

  writefile << Hash::sha256_hash(password);

  writefile.close();

  return 1;
}
