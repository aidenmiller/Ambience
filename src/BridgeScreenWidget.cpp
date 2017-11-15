/**
 *  @file       BridgeScreenWidget.cpp
 *  @author     CS 3307 - Team 13
 *  @date       10/7/2017
 *  @version    1.0
 *
 *  @brief      CS 3307, Hue Light Application screen for viewing, adding, and removing bridges
 *
 *  @section    DESCRIPTION
 *
 *              This class represents the bridge view screen, the user can view the bridges
 *              already connected to their account, register a new bridge, or remove an existing
 *              bridge.
 */

#include <Wt/WText>
#include <Wt/WTable>
#include <fstream> // writing new accounts to a file
#include "BridgeScreenWidget.h"
#include "Bridge.h"
#include "Hash.h" // for password encryption
#include <string>
#include <vector>
#include "Account.h"
#include <Wt/Json/Value>
#include <Wt/Json/Object>
#include <Wt/Json/Parser>

using namespace Wt;
using namespace std;

/**
 *   @brief  Bridge Screen Widget constructor
 *
 *   @param  *parent is a pointer the the containerwidget that stores this widget
 *   @param  *account is a pointer to the user account object
 *   @param  *main is a pointer to the app's welcome screen
 */
BridgeScreenWidget::BridgeScreenWidget(WContainerWidget *parent, Account *account, WelcomeScreen *main):
WContainerWidget(parent)
{
    setContentAlignment(AlignCenter);
    parent_ = main;
    account_ = account;
}

/**
 *   @brief  Update function, clears the widget and re-populates with elements of the bridge screen
 *
 *   @return  void
 *
 */
void BridgeScreenWidget::update()
{
    clear(); // everytime you come back to page, reset the widgets
    
    inputNotEmpty_ = new WValidator(this);
    inputNotEmpty_->setMandatory(true);
    
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
    bridgename_->setValidator(inputNotEmpty_);
    
    new WBreak(this);
    
    //Bridge Location
    new WText("Location: ", this);
    location_ = new WLineEdit();
    location_->setTextSize(10); // to hold placeholder text
    location_->setPlaceholderText("Bedroom"); // placeholder text
    addWidget(location_);
    location_->setValidator(inputNotEmpty_);
    
    new WBreak(this);
    
    //ip address
    new WText("IP Address: ", this);
    ip_ = new WLineEdit();
    ip_->setTextSize(18); // to hold placeholder text
    ip_->setPlaceholderText("127.0.0.1"); // placeholder text
    addWidget(ip_);
    ip_->setValidator(inputNotEmpty_);
    
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
    port_->setValidator(inputNotEmpty_);
    
    new WBreak(this);
    
    //username
    new WText("Username: ", this);
    username_ = new WLineEdit();
    username_->setTextSize(18); // to hold placeholder text
    username_->setPlaceholderText("newdeveloper"); // placeholder text
    addWidget(username_);
    username_->setValidator(inputNotEmpty_);
    
    new WBreak(this);
    
    //button for registering a bridge
    createBridgeButton_ = new WPushButton("Register Bridge");
    addWidget(createBridgeButton_);
    createBridgeButton_->clicked().connect(this, &BridgeScreenWidget::connectBridge);
    
    new WBreak(this);
    
    statusMessage_ = new WText("", this);
    statusMessage_->setStyleClass("error");
    statusMessage_->setHidden(true);
    
    new WBreak(this);
    
    // Page title
    WText *title2 = new WText("Your Bridges", this);
    title2->setStyleClass("title");
    
    new WBreak(this);
    
    //create WTable and call updateBridgeTable
    bridgeTable_ = new WTable(this);
    bridgeTable_->setHeaderCount(1); //set first row as header
    BridgeScreenWidget::updateBridgeTable();
    
    new WBreak(this);
    
    //input field for removing a bridge
    bridgeIndex_ = new WLineEdit();
    bridgeIndex_->setInputMask("99");
    addWidget(bridgeIndex_);
    bridgeIndex_->setValidator(inputNotEmpty_);
    
    //button for removing a bridge
    removeBridgeButton_ = new WPushButton("Remove Bridge");
    addWidget(removeBridgeButton_);
    removeBridgeButton_->clicked().connect(this, &BridgeScreenWidget::removeBridge);
}

/**
 *   @brief  Function called when Register Bridge button is pressed to test a connection to the
 *           provided URL
 *
 *   @return  void
 *
 */

void BridgeScreenWidget::connectBridge() {
    string url_ = "http://" + ip_->text().toUTF8() + ":" + port_->text().toUTF8() + "/api/" + username_->text().toUTF8();
    
    cout << "BRIDGE: Connecting to URL " << url_ << "\n";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(1000000);
    client->done().connect(boost::bind(&BridgeScreenWidget::handleHttpResponse, this, _1, _2));
    
    if(client->get(url_)) {
        WApplication::instance()->deferRendering();
    }
    else {
        cerr << "BRIDGE: Error in client->get(url_) call\n";
        statusMessage_->setText("Unable to connect to Bridge");
        statusMessage_->setHidden(false);
    }
}

/**
 *   @brief  Function to handle the Http response generated by the Wt Http Client object in the
 *           connectBridge() function.
 *
 *   @param  *err stores the error code generated by an Http request, null if request was successful
 *   @param  &response stores the response message generated by the Http request
 *
 *   @return  void
 *
 */
void BridgeScreenWidget::handleHttpResponse(boost::system::error_code err, const Wt::Http::Message &response)
{
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        statusMessage_->setText("Successfully connected!");
        statusMessage_->setHidden(false);
        
        //add Bridge to user account
        account_->addBridge(bridgename_->text().toUTF8(), location_->text().toUTF8(), ip_->text().toUTF8(), port_->text().toUTF8(), username_->text().toUTF8());
        account_->writeFile(); //update credentials file
        
        
        cout << "\nJSON OUTPUT TESTING\n";
        Json::Object result;
        Json::parse(response.body(), result);
        
        Json::Object config = result.get("config");
        cout << "ISNULL: " << config.isNull("ipaddress") << "\n";
        cout << "CONTAINS: " << config.contains("ipaddress") << "\n";
        cout << "TYPE: " << config.type("ipaddress") << "\n";
        Json::Value jsonValue = config.get("ipaddress");
        cout << "VALUETYPE: " << jsonValue.type() << "\n";
        WString s = config.get("ipaddress");
        cout << "IP ADDRESS: " << s << "\n";
        
        cout << "\nPARSING LIGHTS IN THE BRIDGE\n";
        Json::Object lights = result.get("lights");
        int i = 1;
        while(true) {
            if(lights.isNull(boost::lexical_cast<string>(i))) break;
            Json::Object light = lights.get(boost::lexical_cast<string>(i));
            cout << "***Light " << i << "***\n";
            
            WString outp = light.get("name");
            cout << "Name: " << outp << "\n";
            outp = light.get("type");
            cout << "Type: " << outp << "\n";
            
            cout << "*State Info*\n";
            Json::Object state = light.get("state");
            bool outpbool = state.get("on");
            cout << "On: " << outpbool << "\n";
            
            i++;
        }
        
        BridgeScreenWidget::updateBridgeTable();
    }
    else {
        cerr << "Error: " << err.message() << ", " << response.status() << "\n";
        // message that warns user of failed connection
        statusMessage_->setText("Unable to connect to Bridge");
        statusMessage_->setHidden(false);
    }
}

/**
 *   @brief  Removes a Bridge from the list at the row specified by the user in an input field.
 *           This deletes the Bridge from the user account and its data, but not the Bridge file in case
 *           another user has the Bridge associated with their account.
 *
 *   @return  void
 *
 */
void BridgeScreenWidget::removeBridge(){
    int index;
    
    if(bridgeIndex_->text().toUTF8().compare("") == 0) {
        index = 0;
    }
    else {
        index = boost::lexical_cast<int>(bridgeIndex_->text());
    }
    
    if(account_->getNumBridges() == 0) {
        statusMessage_->setText("No bridges to delete!");
        statusMessage_->setHidden(false);
    }
    else if (index > account_->getNumBridges() || index < 1){
        string errorMessage = "Enter number between 1 and " + boost::lexical_cast<string>(account_->getNumBridges());
        statusMessage_->setText(errorMessage);
        statusMessage_->setHidden(false);
    }
    else {
        statusMessage_->setText("Bridge at position " + boost::lexical_cast<string>(index) + " removed.");
        statusMessage_->setHidden(false);
        account_->removeBridge(index);
        account_->writeFile(); //update credentials file
        BridgeScreenWidget::updateBridgeTable();
    }
}

/**
 *   @brief  Generates list of Bridges connected to user account
 *
 *   @return  void
 *
 */
void BridgeScreenWidget::updateBridgeTable(){
    //remove any existing entries
    bridgeTable_->clear();
    
    //only populate if there are bridges existing
    if(account_->getNumBridges() > 0) {
        //create new row for headers <tr>
        WTableRow *tableRow = bridgeTable_->insertRow(bridgeTable_->rowCount());
        //table headers <th>
        tableRow->elementAt(0)->addWidget(new Wt::WText("#"));
        tableRow->elementAt(1)->addWidget(new Wt::WText("Name"));
        tableRow->elementAt(2)->addWidget(new Wt::WText("Location"));
        tableRow->elementAt(3)->addWidget(new Wt::WText("IP Address"));
        tableRow->elementAt(4)->addWidget(new Wt::WText("Port"));
        tableRow->elementAt(5)->addWidget(new Wt::WText("Username"));
        tableRow->elementAt(6)->addWidget(new Wt::WText("Status"));
        tableRow->elementAt(7)->addWidget(new Wt::WText("Actions"));
        
        
        //populate each row with user account bridges
        for(auto &bridge : account_->getBridges()) {
            //create new row for entry <tr>
            tableRow = bridgeTable_->insertRow(bridgeTable_->rowCount());
            
            //table data <td>
            tableRow->elementAt(0)->addWidget(new Wt::WText(boost::lexical_cast<string>(bridgeTable_->rowCount() - 1)));
            
            tableRow->elementAt(1)->addWidget(new Wt::WText(bridge.getName()));
            tableRow->elementAt(2)->addWidget(new Wt::WText(bridge.getLocation()));
            tableRow->elementAt(3)->addWidget(new Wt::WText(bridge.getIP()));
            tableRow->elementAt(4)->addWidget(new Wt::WText(bridge.getPort()));
            tableRow->elementAt(5)->addWidget(new Wt::WText(bridge.getUsername()));
            
            string status = bridge.isConnected()?"Connected":"Disconnected";
            tableRow->elementAt(6)->addWidget(new Wt::WText(status));
            
            /* Trying to get in-line buttons to work but the connect() requirement for WPushButton is really limiting me... */
            WPushButton *viewBridgeButton = new WPushButton("View");
            viewBridgeButton->clicked().connect(this, &BridgeScreenWidget::removeBridge);
            
            WPushButton *editBridgeButton = new WPushButton("Edit");
            editBridgeButton->clicked().connect(this, &BridgeScreenWidget::removeBridge);
            
            WPushButton *removeBridgeButton = new WPushButton("Remove");
            removeBridgeButton->clicked().connect(this, &BridgeScreenWidget::removeBridge);
            
            tableRow->elementAt(7)->addWidget(viewBridgeButton);
            tableRow->elementAt(7)->addWidget(editBridgeButton);
            tableRow->elementAt(7)->addWidget(removeBridgeButton);
        }
    }
}












