#include <string>
#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WTableCell>
#include <Wt/WServer>
#include <Wt/WStackedWidget>
#include "Account.h"
#include "CreateAccountWidget.h"
#include "LoginWidget.h"
#include "WelcomeScreen.h"

using namespace Wt;
using namespace std;

WelcomeScreen::WelcomeScreen(WContainerWidget *parent):
  WContainerWidget(parent),
  create_(0),
  login_(0)
{
    mainStack_ = new WStackedWidget();
    addWidget(mainStack_);

    links_ = new WContainerWidget();
    links_->show();
    addWidget(links_);

    createAnchor_ = new WAnchor("/create", "Create New Account ", links_);
    createAnchor_->setLink(WLink(WLink::InternalPath, "/create"));


    loginAnchor_ = new WAnchor("/login", "Login with Existing Account ", links_);
    loginAnchor_->setLink(WLink(WLink::InternalPath, "/login"));

    WApplication::instance()->internalPathChanged().connect(this, &WelcomeScreen::handleInternalPath);
}

void WelcomeScreen::handleInternalPath(const string &internalPath)
{
    if (true) { // change to if(loggedin = true)
        if (internalPath == "/create")
            createAccount();
        else if (internalPath == "/login")
            login();
    else
        WApplication::instance()->setInternalPath("/login", true);
    }
}

void WelcomeScreen::createAccount()
{
    if (!create_) {
            create_ = new CreateAccountWidget(mainStack_);

    }


    mainStack_->setCurrentWidget(create_);

    this->
    create_->update();
}

void WelcomeScreen::login()
{
    if (!login_) {
            login_ = new LoginWidget(mainStack_);
    }
    mainStack_->setCurrentWidget(login_);
    login_->update();
}
