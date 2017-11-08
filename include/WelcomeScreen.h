#ifndef WELCOME_SCREEN_H
#define WELCOME_SCREEN_H

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WText>
#include <Wt/WPushButton>
#include <string>
#include "Account.h"

namespace Wt {
  class WStackedWidget;
  class WAnchor;
}

class LoginWidget;
class CreateAccountWidget;
class BridgeScreenWidget;

class WelcomeScreen : public Wt::WContainerWidget
{
public:
    WelcomeScreen(Wt::WContainerWidget *parent = 0);
    void handleInternalPath(const std::string &internalPath);
    
    Account getAccount() {return account_;};
    void setAccount(Account account) {account_ = account;};

private:
    Wt::WText *serverMessage_;
    Wt::WStackedWidget *mainStack_; // main stack of the application
    Wt::WContainerWidget *links_; // links container that stores anchors
    LoginWidget *login_; // login widget
    CreateAccountWidget *create_; // create widget
    BridgeScreenWidget *bridgeScreen_; // bridge widget
    Wt::WAnchor *createAnchor_; // anchor for create link
    Wt::WAnchor *loginAnchor_; // anchor for login link
    void login();
    void createAccount();
    void bridgeScreen();
    
    Account account_;
};

#endif //WELCOME_SCREEN_H
