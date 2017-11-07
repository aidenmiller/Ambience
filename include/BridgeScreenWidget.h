#ifndef BRIDGE_SCREEN_WIDGET_H
#define BRIDGE_SCREEN_WIDGET_H

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
    BridgeScreenWidget(Wt::WContainerWidget *parent = 0, WelcomeScreen *main = 0);

    void update();
private:

    WelcomeScreen *parent_;
    Wt::WLineEdit *bridgename_;
    Wt::WLineEdit *ip_;
    Wt::WLineEdit *port_;
    Wt::WLineEdit *username_;
    
    Wt::WPushButton *createBridgeButton_;
    
    bool validateInputFields();
    Wt::WRegExpValidator *ipValidator_;
    
    void addBridge();
    
};

#endif //BRIDGE_SCREEN_WIDGET_H
