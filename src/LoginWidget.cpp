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

using namespace Wt;
using namespace std;

LoginWidget::LoginWidget(WContainerWidget *parent, WelcomeScreen *main):
  WContainerWidget(parent)
{
  setContentAlignment(AlignCenter);
    parent_ = main;
}

void LoginWidget::update()
{
    clear();
    new WText("User ID: ", this);
    idEdit_ = new WLineEdit();
    addWidget(idEdit_);
    new WBreak(this);

    new WText("Password: ", this);
    pwEdit_ = new WLineEdit();
    addWidget(pwEdit_);
    new WBreak(this);

    loginButton_ = new WPushButton("Login");
    addWidget(loginButton_);

    loginButton_->clicked().connect(this, &LoginWidget::submit);
    statusMessage_ = new WText("Incorrect username or password!",this);
    statusMessage_->setHidden(true);
}

void LoginWidget::submit(){
    if(!LoginWidget::checkCredentials(idEdit_->text().toUTF8(),pwEdit_->text().toUTF8())){
        statusMessage_->setHidden(false);
    }
    else {
        parent_->handleInternalPath("/dashboard");
    }
}

bool LoginWidget::checkCredentials(string username, string password) {
    ifstream inFile;
    string str;
    string filename = "credentials/" + username+".txt";
    inFile.open(filename.c_str());
    string hashedPW = Hash::sha256_hash(password);

    if (!inFile) {
        return(false);
    }

    while (getline(inFile, str))
    {
        if (str.compare(hashedPW)==0){
            //redirect to profile page
            //john: should close inFile here so we ensure it closes
            return true;
        }
    }
    inFile.close();
    return false;
}
