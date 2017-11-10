#include <Wt/WText>
#include <Wt/WTable>
#include <fstream> // writing new accounts to a file
#include "BridgeScreenWidget.h"
#include "Bridge.h"
#include "Hash.h" // for password encryption
#include <string>
#include "Account.h"

using namespace Wt;
using namespace std;


BridgeScreenWidget::BridgeScreenWidget(WContainerWidget *parent, Account *account, WelcomeScreen *main):
WContainerWidget(parent)
{
    setContentAlignment(AlignCenter);
    parent_ = main;
    account_ = account;
}

void BridgeScreenWidget::update()
{
    clear(); // everytime you come back to page, reset the widgets
    
    // Page title
    WText *title = new WText("Register a bridge", this);
    title->setStyleClass("title");
    
    new WBreak(this);
    
    //Bridge Name
    new WText("Bridge Name: ", this);
    bridgename_ = new WLineEdit();
    bridgename_->setTextSize(10); // to hold placeholder text
    bridgename_->setPlaceholderText("MyBridge"); // placeholder text
    addWidget(bridgename_);
    
    new WBreak(this);
    
    //Bridge Location
    new WText("Location: ", this);
    location_ = new WLineEdit();
    location_->setTextSize(10); // to hold placeholder text
    location_->setPlaceholderText("Bedroom"); // placeholder text
    addWidget(location_);
    
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
    
    statusMessage_ = new WText("", this);
    statusMessage_->setStyleClass("error");
    statusMessage_->setHidden(true);
    
    new WBreak(this);
    
    // Page title
    WText *title2 = new WText("Your Bridges", this);
    title2->setStyleClass("title");
    
    new WBreak(this);
    
    for(auto &bridge : account_->getBridges()) {
        //display info that was just created
        addWidget(new Wt::WText(bridge.getName()));
        addWidget(new Wt::WText(bridge.getLocation()));
        addWidget(new Wt::WText(bridge.getIP()));
        addWidget(new Wt::WText(bridge.getPort()));
        addWidget(new Wt::WText(bridge.getUsername()));
        new WBreak(this);
    }
    
    createBridgeButton_->clicked().connect(this, &BridgeScreenWidget::addBridge);
}

void BridgeScreenWidget::addBridge(){
    
    Bridge bridge_(bridgename_->text().toUTF8(), location_->text().toUTF8(), ip_->text().toUTF8(), port_->text().toUTF8(), username_->text().toUTF8());
    
    BridgeScreenWidget::connectBridge(&bridge_);
}

void BridgeScreenWidget::connectBridge(Bridge *bridge) {
    //string url_ = "http://172.30.75.112:80/api/newdeveloper";
    string url_ = "http://" + bridge->getIP() + ":" + bridge->getPort() + "/api/" + bridge->getUsername();
    
    cout << "\n\nBegin connect to: "  + url_ + "\n\n\n";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(1000000);
    client->done().connect(boost::bind(&BridgeScreenWidget::handleHttpResponse, this, bridge, _1, _2));
    
    if(client->get(url_)) {
        WApplication::instance()->deferRendering();
        
        bridge->writeBridge(account_->getEmail(), json_);
        account_->addBridge(*bridge);
        
        BridgeScreenWidget::update();
        //todo: instead of updating maybe i should just append this new bridge to a WTable defined initially when i looped through the vector. that would be much better than updating the entire screen i think
    }
    
}

void BridgeScreenWidget::handleHttpResponse(Bridge *bridge, boost::system::error_code err, const Wt::Http::Message &response)
{
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        cout << response.body() << "\n";
        
        statusMessage_->setText("Successfully connected!");
        statusMessage_->setHidden(false);
        
        json_ = response.body();
    }
    else {
        cerr << "Error: " << err.message() << ", " << response.status()
        << "\n";
        // message that warns user of failed connection
        statusMessage_->setText("Error: Unable to connect to Bridge (" + err.message() + ").");
        statusMessage_->setHidden(false);
    }
}
