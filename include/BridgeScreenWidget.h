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
#include <Wt/WIntValidator>
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
    Wt::WLineEdit *location_;
    Wt::WLineEdit *ip_;
    Wt::WLineEdit *port_;
    Wt::WLineEdit *username_;

    WTable *bridgeTable_;

    Wt::WText *statusMessage_;
    Wt::WPushButton *registerBridgeButton_;

    Wt::WRegExpValidator *ipValidator_;
    Wt::WRegExpValidator *stringValidator_;
    Wt::WIntValidator *portValidator_;

    Bridge *bridge_;

    void updateBridgeTable();

    void registerBridge();
    void registerBridgeHttp(boost::system::error_code err, const Wt::Http::Message &response);
    
    void viewBridge(int pos);
    void viewBridgeHttp(int pos, boost::system::error_code err, const Wt::Http::Message &response);
    
    void removeBridge(int pos);

};

#endif //BRIDGE_SCREEN_WIDGET_H
