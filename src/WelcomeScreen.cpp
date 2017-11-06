#include <string>
#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WTableCell>
#include <Wt/WServer>
#include "Account.h"
#include "CreateAccountScreen.h"

using namespace Wt;
using namespace std;

class WelcomeScreen : public WApplication
{
public:
    WelcomeScreen(const WEnvironment& env);

    WLineEdit *accountEdit_; // account name text box
    WLineEdit *passwordEdit_; // password text box
private:
    WPushButton *login_; // clickable button to log in
    WPushButton *createAccount_; // clickable button to create new Account
    WPushButton *resetPassword_; // clickable button to reset password
    void login();
    void createAccount();
    void resetPassword();
};

// Create Wt display with a WLineEdit widget where user can enter number of people to generate
WelcomeScreen::WelcomeScreen(const WEnvironment& env)
    : WApplication(env)
{
    setTitle("Welcome Screen"); //Title of page

    root()->addWidget(new WText("ID: ")); // ask for id
    accountEdit_ = new WLineEdit(root()); // text box for id
    root()->addWidget(new WBreak());

    root()->addWidget(new WText("Password: ")); // ask for pw
    passwordEdit_ = new WLineEdit(root()); // text box for pw
    login_ = new WPushButton("Login!", root()); // button to log in
    root()->addWidget(new WBreak());

    createAccount_ = new WPushButton("Create new account", root()); // button to create new account
    resetPassword_ = new WPushButton("Reset Password", root()); // forgot password button

    login_->clicked().connect(this, &WelcomeScreen::login); // run login function when button is clicked
    createAccount_->clicked().connect(this, &WelcomeScreen::createAccount); // run createAccount function when button is clicked
    resetPassword_->clicked().connect(this, &WelcomeScreen::resetPassword); // run resetPassword function when button is clicked
}

void WelcomeScreen::login(){
    printf("Account: %s, Password: %s\n", accountEdit_->text(), passwordEdit_->text());
}

void WelcomeScreen::createAccount(){
    printf("Create Account clicked\n");
}

void WelcomeScreen::resetPassword(){
    printf("Reset Password clicked\n");
}

// Create new application function
WApplication *createApplication(const WEnvironment& env)
{
    return new CreateAccountScreen(env);
}

int main(int argc, char **argv)
{
    try {
        WServer server(argv[0], "");

        server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
        server.addEntryPoint(Application, createApplication);

        if (server.start()) {
            int sig = WServer::waitForShutdown(argv[0]);

            cerr << "Server Shutdown (SIGNAL: " << sig << ")" << "\n";
            server.stop();
        }
    }
    catch (WServer::Exception& ex) {
        cerr << "EXCEPTION: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
