#ifndef LOGIN_WIDGET_H
#define LOGIN_WIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include "WelcomeScreen.h"

class LoginWidget: public Wt::WContainerWidget
{
public:
    LoginWidget(Wt::WContainerWidget *parent = 0, WelcomeScreen *main = 0);
    void update();

private:
    Wt::WLineEdit *idEdit_;
    Wt::WLineEdit *pwEdit_;
    Wt::WPushButton *loginButton_;
    Wt::WText *statusMessage_;
    
    WelcomeScreen *parent_;

    void submit();
    bool checkCredentials(std::string username, std::string password);
};

#endif //LOGIN_WIDGET_H
