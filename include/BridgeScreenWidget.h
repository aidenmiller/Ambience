#ifndef BRIDGE_SCREEN_WIDGET_H
#define BRIDGE_SCREEN_WIDGET_H

#include <Wt/WResource>
#include <Wt/WApplication>
#include <Wt/Http/Response>
#include <Wt/Http/Client>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WBreak>
#include <Wt/WLengthValidator>
#include <Wt/WRegExpValidator>
#include "WelcomeScreen.h"
class BridgeScreenWidget: public Wt::WContainerWidget
{
public:
    BridgeScreenWidget(Wt::WContainerWidget *parent = 0,
                       Account *account = 0,
                       WelcomeScreen *main = 0);

    void update();
private:

    WelcomeScreen *parent_;
    Account *account_;
    
    Wt::WLineEdit *bridgename_;
    Wt::WLineEdit *ip_;
    Wt::WLineEdit *port_;
    Wt::WLineEdit *username_;
    
    Wt::WText *statusMessage_;
    Wt::WPushButton *createBridgeButton_;
    
    string json_;
    
    bool validateInputFields();
    Wt::WRegExpValidator *ipValidator_;
    
    void addBridge();
    
    void connectBridge(Bridge *bridge);
    void handleHttpResponse(Bridge *bridge, boost::system::error_code err, const Wt::Http::Message &response);
    
};

#endif //BRIDGE_SCREEN_WIDGET_H
