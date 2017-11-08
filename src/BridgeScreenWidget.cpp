#include <Wt/WText>
#include <Wt/WTable>
#include <fstream> // writing new accounts to a file
#include "BridgeScreenWidget.h"
#include "Bridge.h"
#include "Hash.h" // for password encryption
#include <string>

using namespace Wt;
using namespace std;


BridgeScreenWidget::BridgeScreenWidget(WContainerWidget *parent, WelcomeScreen *main):
WContainerWidget(parent)
{
    setContentAlignment(AlignCenter);
    parent_ = main;
}

void BridgeScreenWidget::update()
{
    clear(); // everytime you come back to page, reset the widgets
    
    //Bridge Name
    new WText("Bridge Name: ", this);
    bridgename_ = new WLineEdit();
    bridgename_->setTextSize(10); // to hold placeholder text
    bridgename_->setPlaceholderText("MyBridge"); // placeholder text
    addWidget(bridgename_);
    
    new WBreak(this);
    
    //ip address
    new WText("IP Address: ", this);
    ip_ = new WLineEdit();
    ip_->setTextSize(18); // to hold placeholder text
    ip_->setPlaceholderText("127.0.0.1"); // placeholder text
    addWidget(ip_);
    
    //ip address validator
    //ipValidator_ = new WRegExpValidator("[0-9]+.[0-9]+.[0-9]+.[0-9]",this);
    //ipValidator_->setInvalidNoMatchText("Invalid IP Address");
    //ipValidator_->setValidator(ipValidator_);
    
    new WBreak(this);
    
    //port number
    new WText("Port Number: ", this);
    port_ = new WLineEdit();
    port_->setTextSize(6); // to hold placeholder text
    port_->setPlaceholderText("80"); // placeholder text
    addWidget(port_);
    
    new WBreak(this);
    
    //username
    new WText("Username: ", this);
    username_ = new WLineEdit();
    username_->setTextSize(18); // to hold placeholder text
    username_->setPlaceholderText("newdeveloper"); // placeholder text
    addWidget(username_);
    
    new WBreak(this);
    
    createBridgeButton_ = new WPushButton("Create Bridge");
    addWidget(createBridgeButton_);
    
    new WBreak(this);
    
    createBridgeButton_->clicked().connect(this, &BridgeScreenWidget::addBridge);
}

void BridgeScreenWidget::addBridge(){
    
    Bridge *bridge_ = new Bridge(bridgename_->text().toUTF8(), ip_->text().toUTF8(), port_->text().toUTF8(), username_->text().toUTF8());
    
    bridge_->connect();
    
    //display info that was just created
    addWidget(new Wt::WText(bridge_->getName()));
    addWidget(new Wt::WText(bridge_->getIP()));
    addWidget(new Wt::WText(bridge_->getPort()));
    addWidget(new Wt::WText(bridge_->getUsername()));
    
    new WBreak(this);
}
