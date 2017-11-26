/**
 *  @file       LightManagementWidget.cpp
 *  @author     CS 3307 - Team 13
 *  @date       11/16/2017
 *  @version    1.0
 *
 *  @brief      CS 3307, Hue Light Application screen for managing lights/groups/schedules on a bridge
 *
 *  @section    DESCRIPTION
 *
 *
 */

#include <Wt/WText>
#include <string>
#include <vector>
#include <unistd.h>
#include "LightManagementWidget.h"
#include <Wt/WContainerWidget>
#include <Wt/WSplitButton>
#include <Wt/WPopupMenu>
#include <Wt/WPopupMenuItem>
#include <Wt/WColor>
#include <Wt/WCssDecorationStyle>
#include <Wt/WMenu>
#include <Wt/WStackedWidget>
#include <Wt/WImage>
#include <Wt/Json/Value>
#include <Wt/Json/Object>
#include <Wt/Json/Parser>
#include <Wt/Json/Array>
#include <Wt/WCalendar>
#include <Wt/WDate>
#include <Wt/WDateEdit>
#include <Wt/WTemplate>
#include <Wt/WTimeEdit>


using namespace Wt;
using namespace std;

/**
 *   @brief  light management Widget constructor
 *
 *   @param  *parent is a pointer the the containerwidget that stores this widget
 *   @param  *bridge is a pointer to the Bridge object
 *   @param  *main is a pointer to the app's welcome screen
 */
LightManagementWidget::LightManagementWidget(WContainerWidget *parent, Bridge *bridge, WelcomeScreen *main):
WContainerWidget(parent),
overviewWidget_(0),
lightsWidget_(0),
groupsWidget_(0),
schedulesWidget_(0)
{
    setContentAlignment(AlignLeft);
    parent_ = main;
    bridge_ = bridge;
}


/**
 *   @brief  Update function, clears the widget and re-populates with elements of the light management
 *
 *   @return  void
 *
 */
void LightManagementWidget::update()
{
    clear(); // everytime you come back to page, reset the widgets

    WMenu *menu = new WMenu();
    addWidget(menu);

    WMenuItem *overviewMenuItem = new WMenuItem("Overview");
    menu->addItem(overviewMenuItem);
    overviewMenuItem->triggered().connect(this, &LightManagementWidget::viewOverviewWidget);

    WMenuItem *lightMenuItem = new WMenuItem("Lights");
    menu->addItem(lightMenuItem);
    lightMenuItem->triggered().connect(this, &LightManagementWidget::viewLightsWidget);

    WMenuItem *schedulesMenuItem = new WMenuItem("Schedules");
    menu->addItem(schedulesMenuItem);
    schedulesMenuItem->triggered().connect(this, &LightManagementWidget::viewSchedulesWidget);

    WMenuItem *groupsMenuItem = new WMenuItem("Groups");
    menu->addItem(groupsMenuItem);
    groupsMenuItem->triggered().connect(this, &LightManagementWidget::viewGroupsWidget);

    lightManagementStack_ = new WStackedWidget();
    lightManagementStack_->setContentAlignment(AlignCenter);

    menu->setStyleClass("nav nav-pills nav-stacked");
    menu->setWidth(150);

    //create refreshButton for refreshing Bridge JSON data
    WPushButton *refreshButton = new WPushButton("Refresh Bridge");
    refreshButton->clicked().connect(boost::bind(&LightManagementWidget::refreshBridge, this));
    
    addWidget(refreshButton);
    
    addWidget(lightManagementStack_);
    
    //create overviewWidget
    overviewWidget_ = new WContainerWidget(lightManagementStack_);
    overviewWidget_->setContentAlignment(AlignCenter);
    new WText("Bridge Name: " + bridge_->getName(), overviewWidget_);
    new WBreak(overviewWidget_);
    new WText("Bridge Location: " + bridge_->getLocation(), overviewWidget_);
    
    //create lightsWidget
    lightsWidget_ = new WContainerWidget(lightManagementStack_);
    lightsWidget_->setContentAlignment(AlignCenter);
    lightsTable_ = new WTable(lightsWidget_);
    lightsTable_->setHeaderCount(1); //set first row as header
    updateLightsTable();
    
    //create groupsWidget
    groupsWidget_ = new WContainerWidget(lightManagementStack_);
    groupsWidget_->setContentAlignment(AlignCenter);
    groupsTable_ = new WTable(groupsWidget_);
    groupsTable_->setHeaderCount(1); //set first row as header
    updateGroupsTable();
    
    //create schedulesWidget
    schedulesWidget_ = new WContainerWidget(lightManagementStack_);
    schedulesWidget_->setContentAlignment(AlignCenter);
    WPushButton *newScheduleButton = new WPushButton("Add +");
    newScheduleButton->clicked().connect(boost::bind(&LightManagementWidget::createScheduleDialog, this));
    schedulesWidget_->addWidget(newScheduleButton);
    schedulesTable_ = new WTable(schedulesWidget_);
    schedulesTable_->setHeaderCount(1);
    updateSchedulesTable();
    
    //initialize page with Overview as initial view
    overviewMenuItem->select();
}

void LightManagementWidget::viewOverviewWidget(){
    lightManagementStack_->setCurrentWidget(overviewWidget_);
}

void LightManagementWidget::viewLightsWidget(){
    lightManagementStack_->setCurrentWidget(lightsWidget_);
}

void LightManagementWidget::viewGroupsWidget(){
    lightManagementStack_->setCurrentWidget(groupsWidget_);
}

void LightManagementWidget::viewSchedulesWidget(){
    lightManagementStack_->setCurrentWidget(schedulesWidget_);
}

void LightManagementWidget::updateLightsTable() {
    lightsTable_->clear();

    //create new row for headers <tr>
    WTableRow *tableRow = lightsTable_->insertRow(lightsTable_->rowCount());
    //table headers <th>
    tableRow->elementAt(0)->addWidget(new Wt::WText("Light #"));
    tableRow->elementAt(1)->addWidget(new Wt::WText("Name"));
    tableRow->elementAt(2)->addWidget(new Wt::WText("Brightness"));
    tableRow->elementAt(3)->addWidget(new Wt::WText("Actions"));


    //convert json string into json object
    Json::Object bridgeJson;
    Json::parse(bridge_->getJson(), bridgeJson);
    Json::Object lights = bridgeJson.get("lights");

    int i = 1;
    while(true) {
        //if light i does not exist then break while loop
        if(lights.isNull(boost::lexical_cast<string>(i))) break;

        //if light i exists then get the json data for it and make Light object
        Json::Object lightData = lights.get(boost::lexical_cast<string>(i));
        Light *light = new Light(boost::lexical_cast<string>(i), lightData);

        //create new row for entry <tr>
        tableRow = lightsTable_->insertRow(lightsTable_->rowCount());

        //table data <td>
        tableRow->elementAt(0)->addWidget(new WText(light->getLightnum()));
        tableRow->elementAt(1)->addWidget(new WText(light->getName()));

        //brightness slider
        WSlider *brightnessSlider_ = new WSlider();
        brightnessSlider_->resize(200,20);
        brightnessSlider_->setMinimum(0);

        brightnessSlider_->setMaximum(254);
        brightnessSlider_->setValue(light->getBri());
        brightnessSlider_->setDisabled(!light->getOn()); //disable if light off
        brightnessSlider_->valueChanged().connect(boost::bind(&LightManagementWidget::updateLightBri, this, brightnessSlider_, i));
        tableRow->elementAt(2)->addWidget(brightnessSlider_);
        
        string onButton = light->getOn() == 1 ? "On" : "Off";
        WPushButton *switchButton_ = new WPushButton(onButton);
        switchButton_->clicked().connect(boost::bind(&LightManagementWidget::updateLightOn, this, switchButton_, i));
        
        /*WPushButton *editLightButton_ = new WPushButton("Edit");
        editLightButton_->clicked().connect(boost::bind(&LightManagementWidget::editLight, this, i));*/

        WSplitButton *colourButton_ = new WSplitButton("Colour");
        WPopupMenu *colourPopup = new WPopupMenu();
        colourButton_->dropDownButton()->setMenu(colourPopup);
        WPopupMenuItem *hsv = new WPopupMenuItem("Hue/Saturation");
        colourPopup->addItem(hsv);
        //hsv->triggered().connect(boost::bind(&LightManagementWidget::editHSVDialog, this, i));
        colourButton_->setDisabled(!light->getOn());  //disable if light off
        colourButton_->actionButton()->clicked().connect(boost::bind(&LightManagementWidget::editRGBDialog, this, light, i));

        //WCssDecorationStyle *colour = new WCssDecorationStyle();
        //struct xy *cols = ColourConvert::rgb2xy(255.0f, 0.0f, 0.0f);
        //struct rgb *cols2 = ColourConvert::xy2rgb()

        tableRow->elementAt(3)->addWidget(switchButton_);
        tableRow->elementAt(3)->addWidget(colourButton_);
        //tableRow->elementAt(3)->addWidget(editLightButton_);

        i++;
    }
}

void LightManagementWidget::updateGroupsTable() {
    groupsTable_->clear();

    //create new row for headers <tr>
    WTableRow *tableRow = groupsTable_->insertRow(groupsTable_->rowCount());
    //table headers <th>
    tableRow->elementAt(0)->addWidget(new Wt::WText("Group #"));
    tableRow->elementAt(1)->addWidget(new Wt::WText("Name"));
    tableRow->elementAt(2)->addWidget(new Wt::WText("Bri"));
    tableRow->elementAt(3)->addWidget(new Wt::WText("Colormode"));
    tableRow->elementAt(4)->addWidget(new Wt::WText("Ct"));
    tableRow->elementAt(5)->addWidget(new Wt::WText("Alert"));
    tableRow->elementAt(6)->addWidget(new Wt::WText("Reachable"));
    tableRow->elementAt(7)->addWidget(new Wt::WText("Effect"));
    tableRow->elementAt(8)->addWidget(new Wt::WText("Hue"));
    tableRow->elementAt(9)->addWidget(new Wt::WText("On"));
    tableRow->elementAt(10)->addWidget(new Wt::WText("Sat"));
    tableRow->elementAt(11)->addWidget(new Wt::WText("X"));
    tableRow->elementAt(12)->addWidget(new Wt::WText("Y"));
    tableRow->elementAt(13)->addWidget(new Wt::WText("Lights"));

    //convert json string into json object
    Json::Object bridgeJson;
    Json::parse(bridge_->getJson(), bridgeJson);
    Json::Object groups = bridgeJson.get("groups");

    int i = 1;
    while(true) {
        //if group i does not exist then break while loop
        if(groups.isNull(boost::lexical_cast<string>(i))) break;

        //if group i exists then get the json data for it and make Group object
        Json::Object groupData = groups.get(boost::lexical_cast<string>(i));
        Group *group = new Group(boost::lexical_cast<string>(i), groupData);

        //create new row for entry <tr>
        tableRow = groupsTable_->insertRow(groupsTable_->rowCount());

        //table data <td>
        tableRow->elementAt(0)->addWidget(new WText(group->getGroupnum()));
        tableRow->elementAt(1)->addWidget(new WText(group->getName()));
        tableRow->elementAt(2)->addWidget(new WText(boost::lexical_cast<string>(group->getBri())));
        tableRow->elementAt(3)->addWidget(new WText(group->getColormode()));
        tableRow->elementAt(4)->addWidget(new WText(boost::lexical_cast<string>(group->getCt())));
        tableRow->elementAt(5)->addWidget(new WText(group->getAlert()));

        string reachablestr = group->getReachable() == 1 ? "True" : "False";
        tableRow->elementAt(6)->addWidget(new WText(reachablestr));

        tableRow->elementAt(7)->addWidget(new WText(group->getEffect()));
        tableRow->elementAt(8)->addWidget(new WText(boost::lexical_cast<string>(group->getHue())));

        string onstr = group->getOn() == 1 ? "True" : "False";
        tableRow->elementAt(9)->addWidget(new WText(onstr));
        tableRow->elementAt(10)->addWidget(new WText(boost::lexical_cast<string>(group->getSat())));

        tableRow->elementAt(11)->addWidget(new WText(boost::lexical_cast<string>(group->getX())));
        tableRow->elementAt(12)->addWidget(new WText(boost::lexical_cast<string>(group->getY())));

        for(WString lightNum : group->getLights()) {
            tableRow->elementAt(13)->addWidget(new WText(lightNum + " "));
        }
        i++;
    }
}

void LightManagementWidget::updateSchedulesTable() {
    schedulesTable_->clear();

    // create row for headers table
    WTableRow *tableRow = schedulesTable_->insertRow(schedulesTable_->rowCount());
    //table headers <th>
    tableRow->elementAt(0)->addWidget(new WText("Schedules"));
    tableRow->elementAt(1)->addWidget(new WText("Description"));
    tableRow->elementAt(2)->addWidget(new WText("Action"));
    tableRow->elementAt(3)->addWidget(new WText("Time"));

    //convert json string into json object
    Json::Object bridgeJson;
    Json::parse(bridge_->getJson(), bridgeJson);
    Json::Object schedules = bridgeJson.get("schedules");

    int i = 1;
    while(true) {
        //if group i does not exist then break while loop
        if(schedules.isNull(boost::lexical_cast<string>(i))) break;

        //if group i exists then get the json data for it and make Group object
        Json::Object scheduleData = schedules.get(boost::lexical_cast<string>(i));
        Schedule *schedule = new Schedule(boost::lexical_cast<string>(i), scheduleData);

        tableRow = schedulesTable_->insertRow(schedulesTable_->rowCount());

        tableRow->elementAt(0)->addWidget(new WText(schedule->getName()));
        tableRow->elementAt(1)->addWidget(new WText(schedule->getDescription()));


        tableRow->elementAt(2)->addWidget(new WText("X: " + boost::lexical_cast<string>(schedule->getX())));

        tableRow->elementAt(2)->addWidget(new WBreak());

        tableRow->elementAt(2)->addWidget(new WText("Y: " + boost::lexical_cast<string>(schedule->getY())));

        tableRow->elementAt(2)->addWidget(new WBreak());

        tableRow->elementAt(2)->addWidget(new WText("Bri: " + boost::lexical_cast<string>(schedule->getBri())));

        tableRow->elementAt(2)->addWidget(new WBreak());

        tableRow->elementAt(2)->addWidget(new WText("Transition: " + boost::lexical_cast<string>(schedule->getTransition())));

        WString time = schedule->getTime();
        tableRow->elementAt(3)->addWidget(new WText(time));

        WPushButton *editScheduleButton = new WPushButton("Edit Schedule");
        //editScheduleButton->clicked().connect

        WPushButton *removeScheduleButton = new WPushButton("Remove Bridge");
        //removeScheduleButton_->clicked().connect(boost::bind(FUNCTION TO REMOVE sched, this, i));

        tableRow->elementAt(4)->addWidget(editScheduleButton);
        tableRow->elementAt(4)->addWidget(removeScheduleButton);

        i++;
    }
}

/*
void LightManagementWidget::editLight(int pos) {
    Json::Object bridgeJson;
    Json::parse(bridge_->getJson(), bridgeJson);

    Json::Object lights = bridgeJson.get("lights");
    Json::Object light = lights.get(boost::lexical_cast<string>(pos));

    lightEditDialog_ = new WDialog("Edit Light"); // title

    new WLabel("Light Name: ", lightEditDialog_->contents());
    lightEditName_ = new WLineEdit(lightEditDialog_->contents());
    new WBreak(lightEditDialog_->contents());

    // make okay and cancel buttons, cancel sends a reject dialogstate, okay sends an accept
    WPushButton *ok = new WPushButton("OK", lightEditDialog_->contents());
    WPushButton *cancel = new WPushButton("Cancel", lightEditDialog_->contents());

    ok->clicked().connect(lightEditDialog_, &WDialog::accept);
    cancel->clicked().connect(lightEditDialog_, &WDialog::reject);

    // when the user is finished, call the updateLight function
    //lightEditDialog_->finished().connect(boost::bind(&LightManagementWidget::updateLight, this, pos));
    lightEditDialog_->show();
}
 */

void LightManagementWidget::createScheduleDialog() {


    createScheduleDialog_ = new WDialog("Create a Schedule"); // title

    new WLabel("Schedule Name: ", createScheduleDialog_->contents());
    WLineEdit *scheduleName = new WLineEdit(createScheduleDialog_->contents());
    scheduleName->setValueText("schedule");
    new WBreak(createScheduleDialog_->contents());

    new WLabel("Description: ", createScheduleDialog_->contents());
    WLineEdit *description = new WLineEdit(createScheduleDialog_->contents());
    description->setValueText("description");
    new WBreak(createScheduleDialog_->contents());

    //WTemplate *form = new WTemplate(WString::tr("dateEdit-template"), createScheduleDialog_->contents());
    //form->addFunction("id", &WTemplate::Functions::id);

    WDateEdit *dateEdit = new WDateEdit(createScheduleDialog_->contents());
    //form->bindWidget("from",dateEdit);
    dateEdit->setDate(WDate::currentServerDate());
    new WBreak(createScheduleDialog_->contents());

    WTimeEdit *timeEdit = new WTimeEdit(createScheduleDialog_->contents());
    timeEdit->setTime(WTime::currentTime());
    new WBreak(createScheduleDialog_->contents());


    // make okay and cancel buttons, cancel sends a reject dialogstate, okay sends an accept
    WPushButton *ok = new WPushButton("OK", createScheduleDialog_->contents());
    WPushButton *cancel = new WPushButton("Cancel", createScheduleDialog_->contents());

    ok->clicked().connect(createScheduleDialog_, &WDialog::accept);
    cancel->clicked().connect(createScheduleDialog_, &WDialog::reject);

    // when the user is finished, call the ADD SCHEDULE function
    //createScheduleDialog_->finished().connect(boost::bind(&LightManagementWidget::addSchedule, this, pos));
    createScheduleDialog_->show();
}

void LightManagementWidget::updateLightBri(WSlider *slider_, int lightNum){
    string url = "http://" + bridge_->getIP() + ":" + bridge_->getPort() + "/api/" + bridge_->getUsername() + "/lights/" + boost::lexical_cast<string>(lightNum) + "/state";
    Http::Message *data = new Http::Message();
    data->addBodyText("{\"bri\":" + boost::lexical_cast<string>(slider_->value()) + "}");
    
    cout << "Light: Updating light brightness at URL " << url << "\n";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->setTimeout(2); //2 second timeout of request
    client->setMaximumResponseSize(1000000);
    client->done().connect(boost::bind(&LightManagementWidget::handlePutHttp, this, _1, _2));
    
    if(client->put(url, *data)) {
        WApplication::instance()->deferRendering();
    }
    else {
        cerr << "Light: Error in client->put(url) call\n";
    }
}

void LightManagementWidget::updateLightOn(WPushButton *button_, int lightNum){
    //set value string to reflect current state of the button
    string value = button_->text() == "On" ? "False" : "True";
    
    string url = "http://" + bridge_->getIP() + ":" + bridge_->getPort() + "/api/" + bridge_->getUsername() + "/lights/" + boost::lexical_cast<string>(lightNum) + "/state";
    Http::Message *data = new Http::Message();
    
    data->addBodyText("{\"on\":" + value + "}");
    
    cout << "Light: Updating light brightness at URL " << url << "\n";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->setTimeout(2); //2 second timeout of request
    client->setMaximumResponseSize(1000000);
    client->done().connect(boost::bind(&LightManagementWidget::handlePutHttp, this, _1, _2));
    
    if(client->put(url, *data)) {
        WApplication::instance()->deferRendering();
    }
    else {
        cerr << "Light: Error in client->put(url) call\n";
    }
}

void LightManagementWidget::updateLightXY(int lightNum){
    if (editRGBDialog_->result() == WDialog::DialogCode::Rejected)
        return;
    
    string url = "http://" + bridge_->getIP() + ":" + bridge_->getPort() + "/api/" + bridge_->getUsername() + "/lights/" + boost::lexical_cast<string>(lightNum) + "/state";
    
    struct xy *cols = ColourConvert::rgb2xy(redSlider->value(), greenSlider->value(), blueSlider->value());
    
    Http::Message *data = new Http::Message();
    data->addBodyText("{\"xy\":[" + boost::lexical_cast<string>(cols->x) + "," + boost::lexical_cast<string>(cols->y) + "]}");
    
    cout << "Light: Updating light brightness at URL " << url << "\n";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->setTimeout(2); //2 second timeout of request
    client->setMaximumResponseSize(1000000);
    client->done().connect(boost::bind(&LightManagementWidget::handlePutHttp, this, _1, _2));
    
    if(client->put(url, *data)) {
        WApplication::instance()->deferRendering();
    }
    else {
        cerr << "Light: Error in client->put(url) call\n";
    }
}

void LightManagementWidget::handlePutHttp(boost::system::error_code err, const Wt::Http::Message &response)
{
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        cout << "Successful update" << "\n";
        refreshBridge();
    }
    else {
        cerr << "Error: " << err.message() << ", " << response.status() << "\n";
    }
}

void LightManagementWidget::refreshBridge() {
    string url = "http://" + bridge_->getIP() + ":" + bridge_->getPort() + "/api/" + bridge_->getUsername();
    
    cout << "BRIDGE: Connecting to URL " << url << "\n";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->setTimeout(2); //2 second timeout of request
    client->setMaximumResponseSize(1000000);
    client->done().connect(boost::bind(&LightManagementWidget::refreshBridgeHttp, this, _1, _2));
    
    if(client->get(url)) {
        WApplication::instance()->deferRendering();
    }
    else {
        cerr << "BRIDGE: Error in client->get(url) call\n";
    }
}

void LightManagementWidget::refreshBridgeHttp(boost::system::error_code err, const Wt::Http::Message &response)
{
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        bridge_->setJson(response.body());
        
        //update tables with new JSON information
        updateLightsTable();
        updateGroupsTable();
        updateSchedulesTable();
    }
    else {
        cerr << "Error: " << err.message() << ", " << response.status() << "\n";
    }
}

/**
 *   @brief  Opens a WDialog box to edit rgb for specified light
 *
 *   @param   lightNum the position of the Light to view
 *
 *   @return  void
 *
 */
void LightManagementWidget::editRGBDialog(Light *light, int lightNum) {

    editRGBDialog_ = new WDialog("Change Colour"); // title
    new WText("Red: ", editRGBDialog_->contents());
    redSlider = new WSlider(editRGBDialog_->contents());
    new WBreak(editRGBDialog_->contents());
    new WText("Green: ", editRGBDialog_->contents());
    greenSlider = new WSlider(editRGBDialog_->contents());
    new WBreak(editRGBDialog_->contents());
    new WText("Blue: ", editRGBDialog_->contents());
    blueSlider = new WSlider(editRGBDialog_->contents());
    new WBreak(editRGBDialog_->contents());
    redSlider->resize(200,20);
    greenSlider->resize(200,20);
    blueSlider->resize(200,20);

    redSlider->setMinimum(0);
    redSlider->setMaximum(255);
    greenSlider->setMinimum(0);
    greenSlider->setMaximum(255);
    blueSlider->setMinimum(0);
    blueSlider->setMaximum(255);

    WCssDecorationStyle *colour = new WCssDecorationStyle();

    struct rgb *currentRGBVals = ColourConvert::xy2rgb((float)light->getX(),
                                               (float)light->getY(),
                                               (float)light->getBri());

    redSlider->setValue(currentRGBVals->r);
    greenSlider->setValue(currentRGBVals->g);
    blueSlider->setValue(currentRGBVals->b);

    colour->setBackgroundColor(WColor(redSlider->value(), greenSlider->value(), blueSlider->value()));
    editRGBDialog_->contents()->setDecorationStyle(*colour);

    redSlider->valueChanged().connect(bind([=] {
        colour->setBackgroundColor(WColor(redSlider->value(),
                                          greenSlider->value(),
                                          blueSlider->value()));
        editRGBDialog_->contents()->setDecorationStyle(*colour);
                                      }));

    greenSlider->valueChanged().connect(bind([=] {
        colour->setBackgroundColor(WColor(redSlider->value(),
                                          greenSlider->value(),
                                          blueSlider->value()));
        editRGBDialog_->contents()->setDecorationStyle(*colour);
                                      }));

    blueSlider->valueChanged().connect(bind([=] {
        colour->setBackgroundColor(WColor(redSlider->value(),
                                          greenSlider->value(),
                                          blueSlider->value()));
        editRGBDialog_->contents()->setDecorationStyle(*colour);
                                      }));

    new WBreak(editRGBDialog_->contents());

    // make okay and cancel buttons, cancel sends a reject dialogstate, okay sends an accept
    WPushButton *ok = new WPushButton("OK", editRGBDialog_->contents());
    WPushButton *cancel = new WPushButton("Cancel", editRGBDialog_->contents());

    ok->clicked().connect(editRGBDialog_ ,&WDialog::accept);
    cancel->clicked().connect(editRGBDialog_, &WDialog::reject);

    // when the user is finished, call the updateBridge function
    editRGBDialog_->finished().connect(boost::bind(&LightManagementWidget::updateLightXY, this, lightNum));
    editRGBDialog_->show();
}
