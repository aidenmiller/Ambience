#ifndef LOGIN_WIDGET_H
#define LOGIN_WIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>

class LoginWidget: public Wt::WContainerWidget
{
public:
    LoginWidget(Wt::WContainerWidget *parent = 0);
    void update();

private:
    Wt::WLineEdit *idEdit;
    Wt::WLineEdit *pwEdit;
    Wt::WPushButton *loginButton;
};

#endif //LOGIN_WIDGET_H
