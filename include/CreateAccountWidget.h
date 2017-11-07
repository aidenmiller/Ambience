#ifndef CREATE_ACCOUNT_WIDGET_H
#define CREATE_ACCOUNT_WIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WBreak>
#include <Wt/WLengthValidator>
#include <Wt/WRegExpValidator>

class CreateAccountWidget: public Wt::WContainerWidget
{
public:
    CreateAccountWidget(Wt::WContainerWidget *parent = 0);

    void update();
private:
    int writeCredentials(std::string username, std::string password);
    Wt::WLineEdit *username_; // account name text box
    Wt::WLineEdit *password_; // password text box
    Wt::WLineEdit *confirmPassword_; // confirm password text box
    Wt::WPushButton *createAccountButton_; // submit account creation
    Wt::WText *unsuccessfulPassword_;
    Wt::WText *unsuccessfulInput_;

    Wt::WRegExpValidator *usernameValidator_;
    Wt::WLengthValidator *passwordLengthValidator_;


    void submit();
    bool validatePassword();
    bool validateInputFields();
};

#endif //CREATE_ACCOUNT_WIDGET_H
