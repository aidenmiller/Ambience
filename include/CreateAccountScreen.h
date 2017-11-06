
/*
Header for CreateAccountScreen.cpp
*/

#ifndef CREATESCREEN_H
#define CREATESCREEN_H

#include <Wt/WApplication>

class CreateAccountScreen : public Wt::WApplication
{
public:
    CreateAccountScreen(const Wt::WEnvironment& env);

private:
    int writeCredentials(std::string username, std::string password);
    Wt::WLineEdit *username_; // account name text box
    Wt::WLineEdit *password_; // password text box
    Wt::WPushButton *submit_; // submit account creation
    void submit();
};

#endif // CREATESCREEN_H
