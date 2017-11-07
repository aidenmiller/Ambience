#ifndef WELCOME_SCREEN_H
#define WELCOME_SCREEN_H

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WText>
#include <Wt/WPushButton>
#include <string>

namespace Wt {
  class WStackedWidget;
  class WAnchor;
}

class LoginWidget;
class CreateAccountWidget;

class WelcomeScreen : public Wt::WContainerWidget
{
public:
    WelcomeScreen(Wt::WContainerWidget *parent = 0);


    void handleInternalPath(const std::string &internalPath);


private:
    Wt::WText *serverMessage_;
    Wt::WStackedWidget *mainStack_;
    Wt::WContainerWidget *links_;
    LoginWidget *login_;
    CreateAccountWidget *create_;
    Wt::WAnchor *createAnchor_;
    Wt::WAnchor *loginAnchor_;
    void login();
    void createAccount();
    void dashboard();
};

#endif //WELCOME_SCREEN_H
