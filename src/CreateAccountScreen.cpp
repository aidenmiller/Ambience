#include <string>

#include <Wt/WBreak>
#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WTableCell>
#include <Wt/WServer>
#include<fstream>
#include <openssl/sha.h>

#include "Account.h"
#include "CreateAccountScreen.h"
#include "Hash.h"

using namespace Wt;
using namespace std;


// Create Wt display with a WLineEdit widget where user can enter number of people to generate
CreateAccountScreen::CreateAccountScreen(const WEnvironment& env)
    : WApplication(env)
{
    setTitle("Create an Account"); //Title of page

    root()->addWidget(new WText("Username: ")); // ask for id
    username_ = new WLineEdit(root()); // text box for id
    root()->addWidget(new WBreak());

    root()->addWidget(new WText("Password: ")); // ask for pw
    password_ = new WLineEdit(root()); // text box for pw
    submit_ = new WPushButton("Submit", root()); // button to log in
    root()->addWidget(new WBreak());


    submit_->clicked().connect(this, &CreateAccountScreen::submit); // run login function when button is clicked

}

void CreateAccountScreen::submit(){
    printf("submit clicked\n");

    CreateAccountScreen::writeCredentials(username_->text().toUTF8(), password_->text().toUTF8());


}

int CreateAccountScreen::writeCredentials(string username, string password) {
  ofstream writefile;
  string file = username+".txt";

  writefile.open(file.c_str());

  writefile << Hash::sha256_hash(password);

  writefile.close();

  return 1;
}


