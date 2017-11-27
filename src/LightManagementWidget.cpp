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
#include <Wt/WComboBox>
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
#include <Wt/Json/Serializer>
#include <Wt/Json/Array>

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
    WPushButton *newGroupButton = new WPushButton("Add +");
    newGroupButton->clicked().connect(boost::bind(&LightManagementWidget::createGroupDialog, this));
    groupsWidget_->addWidget(newGroupButton);
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
    
    //WContainer for RGB Colour Picker used for selecting Colours
    rgbContainer_ = new WContainerWidget();
    new WText("Red: ", rgbContainer_);
    redSlider = new WSlider(rgbContainer_);
    new WBreak(rgbContainer_);
    new WText("Green: ", rgbContainer_);
    greenSlider = new WSlider(rgbContainer_);
    new WBreak(rgbContainer_);
    new WText("Blue: ", rgbContainer_);
    blueSlider = new WSlider(rgbContainer_);
    new WBreak(rgbContainer_);
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
    
    colour->setBackgroundColor(WColor(redSlider->value(), greenSlider->value(), blueSlider->value()));
    rgbContainer_->setDecorationStyle(*colour);
    
    redSlider->valueChanged().connect(bind([=] {
        colour->setBackgroundColor(WColor(redSlider->value(),
                                          greenSlider->value(),
                                          blueSlider->value()));
        rgbContainer_->setDecorationStyle(*colour);
    }));
    
    greenSlider->valueChanged().connect(bind([=] {
        colour->setBackgroundColor(WColor(redSlider->value(),
                                          greenSlider->value(),
                                          blueSlider->value()));
        rgbContainer_->setDecorationStyle(*colour);
    }));
    
    blueSlider->valueChanged().connect(bind([=] {
        colour->setBackgroundColor(WColor(redSlider->value(),
                                          greenSlider->value(),
                                          blueSlider->value()));
        rgbContainer_->setDecorationStyle(*colour);
    }));
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
    tableRow->elementAt(2)->addWidget(new Wt::WText("Transition"));
    tableRow->elementAt(3)->addWidget(new Wt::WText("Brightness"));
    tableRow->elementAt(4)->addWidget(new Wt::WText("Actions"));
    
    
    //convert json string into json object
    Json::Object bridgeJson;
    Json::parse(bridge_->getJson(), bridgeJson);
    Json::Object lights = bridgeJson.get("lights");

    set<string> data = lights.names();
    for(string num : data) {
        Json::Object lightData = lights.get(num);
        Light *light = new Light(num, lightData);

        //create new row for entry <tr>
        tableRow = lightsTable_->insertRow(lightsTable_->rowCount());
        
        //table data <td>
        tableRow->elementAt(0)->addWidget(new WText(light->getLightnum()));
        tableRow->elementAt(1)->addWidget(new WText(light->getName()));
        
        //transition definer
        WLineEdit *editLightTransition = new WLineEdit();
        editLightTransition->resize(40,20);
        //display transition time to user (transition time is stored as multiple of 100ms)
        editLightTransition->setValueText(boost::lexical_cast<string>(light->getTransition()));
        tableRow->elementAt(2)->addWidget(editLightTransition);
        editLightTransition->setDisabled(!light->getOn());  //disable if light off
        intValidator = new WIntValidator(0, 100, tableRow->elementAt(2)); //100 second maximum
        intValidator->setMandatory(true);
        editLightTransition->setValidator(intValidator);
        editLightTransition->changed().connect(bind([=] {
            light->setTransition(boost::lexical_cast<int>(editLightTransition->valueText()));
        }));
        tableRow->elementAt(2)->addWidget(new WText(" seconds"));
        
        //brightness slider
        WSlider *brightnessSlider_ = new WSlider();
        brightnessSlider_->resize(200,20);
        brightnessSlider_->setMinimum(0);
        
        brightnessSlider_->setMaximum(254);
        brightnessSlider_->setValue(light->getBri());
        brightnessSlider_->setDisabled(!light->getOn()); //disable if light off
        brightnessSlider_->valueChanged().connect(boost::bind(&LightManagementWidget::updateLightBri, this, brightnessSlider_, light));
        tableRow->elementAt(3)->addWidget(brightnessSlider_);
        
        string onButton = light->getOn() == 1 ? "On" : "Off";
        WPushButton *switchButton_ = new WPushButton(onButton);
        switchButton_->clicked().connect(boost::bind(&LightManagementWidget::updateLightOn, this, switchButton_, light));
        
        WPushButton *editLightButton_ = new WPushButton("Edit");
        editLightButton_->clicked().connect(boost::bind(&LightManagementWidget::editLightDialog, this, light));
        
        WSplitButton *colourButton_ = new WSplitButton("Colour");
        WPopupMenu *colourPopup = new WPopupMenu();
        colourButton_->dropDownButton()->setMenu(colourPopup);
        WPopupMenuItem *hsv = new WPopupMenuItem("Hue/Saturation");
        colourPopup->addItem(hsv);
        //hsv->triggered().connect(boost::bind(&LightManagementWidget::editHSVDialog, this, i));
        colourButton_->setDisabled(!light->getOn());  //disable if light off
        colourButton_->actionButton()->clicked().connect(boost::bind(&LightManagementWidget::editRGBDialog, this, light));
        
        //WCssDecorationStyle *colour = new WCssDecorationStyle();
        //struct xy *cols = ColourConvert::rgb2xy(255.0f, 0.0f, 0.0f);
        //struct rgb *cols2 = ColourConvert::xy2rgb()
        
        tableRow->elementAt(4)->addWidget(switchButton_);
        tableRow->elementAt(4)->addWidget(colourButton_);
        tableRow->elementAt(4)->addWidget(editLightButton_);

        WPushButton *removeLightButton = new WPushButton("Remove");
        removeLightButton->clicked().connect(boost::bind(&LightManagementWidget::removeLight, this, light));
        tableRow->elementAt(4)->addWidget(removeLightButton);
    }
}

void LightManagementWidget::updateGroupsTable() {
    groupsTable_->clear();
    
    //create new row for headers <tr>
    WTableRow *tableRow = groupsTable_->insertRow(groupsTable_->rowCount());
    //table headers <th>
    tableRow->elementAt(0)->addWidget(new Wt::WText("Group #"));
    tableRow->elementAt(1)->addWidget(new Wt::WText("Name"));
    tableRow->elementAt(2)->addWidget(new Wt::WText("Lights"));
    tableRow->elementAt(3)->addWidget(new Wt::WText("Transitions"));
    tableRow->elementAt(4)->addWidget(new Wt::WText("Actions"));
    //convert json string into json object
    Json::Object bridgeJson;
    Json::parse(bridge_->getJson(), bridgeJson);
    Json::Object groups = bridgeJson.get("groups");

    set<string> data = groups.names();
    for(string num : data) {
        Json::Object groupData = groups.get(num);
        Group *group = new Group(num, groupData);
        
        //create new row for entry <tr>
        tableRow = groupsTable_->insertRow(groupsTable_->rowCount());
        
        //table data <td>
        tableRow->elementAt(0)->addWidget(new WText(group->getGroupnum()));
        tableRow->elementAt(1)->addWidget(new WText(group->getName()));

        for(WString lightNum : group->getLights()) {
            tableRow->elementAt(2)->addWidget(new WText(lightNum + " "));
        }
        
        //transition definer
        WLineEdit *editGroupTransition = new WLineEdit();
        editGroupTransition->resize(40,20);
        //display transition time to user in seconds (transition time is stored as multiple of 100ms)
        editGroupTransition->setValueText(boost::lexical_cast<string>(group->getTransition()));
        tableRow->elementAt(3)->addWidget(editGroupTransition);
        editGroupTransition->setDisabled(!group->getOn());  //disable if light off
        intValidator = new WIntValidator(0, 100, tableRow->elementAt(2)); //100 second maximum
        intValidator->setMandatory(true);
        editGroupTransition->setValidator(intValidator);
        editGroupTransition->changed().connect(bind([=] {
            group->setTransition(boost::lexical_cast<int>(editGroupTransition->valueText()));
        }));
        tableRow->elementAt(3)->addWidget(new WText(" seconds"));

        WPushButton *advancedButton_ = new WPushButton("Advanced");
        advancedButton_->clicked().connect(boost::bind(&LightManagementWidget::groupAdvancedDialog, this, group));
        tableRow->elementAt(4)->addWidget(advancedButton_);

        // remove group
        WPushButton *removeGroupButton = new WPushButton("Remove");
        removeGroupButton->clicked().connect(boost::bind(&LightManagementWidget::removeGroup, this, group));
        tableRow->elementAt(4)->addWidget(removeGroupButton);
    }
}

// WIP, DOES NOT DO ANYTHING YET
void LightManagementWidget::createGroupDialog() {
    
    createGroupDialog_ = new WDialog("Create a Group"); // title
    
    new WLabel("Group Name: ", createGroupDialog_->contents());
    WLineEdit *groupName = new WLineEdit(createGroupDialog_->contents());
    groupName->setValueText("group");
    new WBreak(createGroupDialog_->contents());
    
    new WLabel("Lights: ", createGroupDialog_->contents());
    Json::Object bridgeJson;
    Json::parse(bridge_->getJson(), bridgeJson);
    Json::Object lights = bridgeJson.get("lights");

    set<string> data = lights.names();
    for(string num : data) {
        Json::Object lightData = lights.get(num);
        Light *light = new Light(num, lightData);
        
        WCheckBox *lightButton_ = new WCheckBox(num, createGroupDialog_->contents());
        //lightButton_->clicked().connect(boost::bind(&Group::addLight, group, light->getLightnum())); //need to find a way to find out which boxes were checked off when the OK button is pressed...
    }
    new WBreak(createGroupDialog_->contents());
    
    // make okay and cancel buttons, cancel sends a reject dialogstate, okay sends an accept
    WPushButton *ok = new WPushButton("OK", createGroupDialog_->contents());
    WPushButton *cancel = new WPushButton("Cancel", createGroupDialog_->contents());
    
    ok->clicked().connect(createGroupDialog_, &WDialog::accept);
    cancel->clicked().connect(createGroupDialog_, &WDialog::reject);

    //createGroupDialog_->finished().connect(boost::bind(&LightManagementWidget::createGroup, this));
    
    createGroupDialog_->show();
}

// does not work yet
void LightManagementWidget::updateGroupOn(WPushButton *button_, Group *group){
    //set value string to reflect current state of the button
    string value = button_->text() == "On" ? "False" : "True";
    
    string url = "http://" + bridge_->getIP() + ":" + bridge_->getPort() + "/api/" + bridge_->getUsername() + "/groups/" + group->getGroupnum().toUTF8() + "/action";
    Http::Message *data = new Http::Message();
    
    //can only set transition time while light is on
    if(group->getOn()) {
        data->addBodyText("{\"on\":" + value + ", \"transitiontime\":" + boost::lexical_cast<string>(group->getTransition()) + "}");
    }
    else {
        data->addBodyText("{\"on\":" + value + "}");
    }
    
    cout << "Group: Updating group on at URL " << url << "\n";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->setTimeout(2); //2 second timeout of request
    client->setMaximumResponseSize(1000000);
    client->done().connect(boost::bind(&LightManagementWidget::handlePutHttp, this, _1, _2));
    
    if(client->put(url, *data)) {
        WApplication::instance()->deferRendering();
    }
    else {
        cerr << "Group: Error in client->put(url) call\n";
    }
}

// does not work yet
void LightManagementWidget::updateGroupBri(WSlider *slider_, Group *group){
    string url = "http://" + bridge_->getIP() + ":" + bridge_->getPort() + "/api/" + bridge_->getUsername() + "/groups/" + group->getGroupnum().toUTF8() + "/action";
    Http::Message *data = new Http::Message();
    data->addBodyText("{\"bri\":" + boost::lexical_cast<string>(slider_->value()) + ", \"transitiontime\":" + boost::lexical_cast<string>(group->getTransition()) + "}");
    
    cout << "Group: Updating group brightness at URL " << url << "\n";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->setTimeout(2); //2 second timeout of request
    client->setMaximumResponseSize(1000000);
    client->done().connect(boost::bind(&LightManagementWidget::handlePutHttp, this, _1, _2));
    
    if(client->put(url, *data)) {
        WApplication::instance()->deferRendering();
    }
    else {
        cerr << "Group: Error in client->put(url) call\n";
    }
}

void LightManagementWidget::groupAdvancedDialog(Group *group) {
    groupAdvancedDialog_ = new WDialog("Advanced"); // title

    new WLabel("State: ", groupAdvancedDialog_->contents());
    onButtonGroup = new WButtonGroup(this);
    WRadioButton *onRadioButton;
    onRadioButton = new WRadioButton("On", groupAdvancedDialog_->contents());
    onButtonGroup->addButton(onRadioButton, 0);
    onRadioButton = new WRadioButton("Off", groupAdvancedDialog_->contents());
    onButtonGroup->addButton(onRadioButton, 1);
    onButtonGroup->setCheckedButton(onButtonGroup->button(0));
    new WBreak(groupAdvancedDialog_->contents());

    //brightness slider
    new WLabel("Brightness: ", groupAdvancedDialog_->contents());
    brightnessSlider_ = new WSlider(groupAdvancedDialog_->contents());
    brightnessSlider_->resize(200,20);
    brightnessSlider_->setMinimum(0);
    brightnessSlider_->setMaximum(254);
    brightnessSlider_->setValue(group->getBri());
    brightnessSlider_->setDisabled(!group->getOn()); //disable if light off
    new WBreak(groupAdvancedDialog_->contents());

    // color
    new WLabel("Color: ", groupAdvancedDialog_->contents());
    xEdit = new WLineEdit(groupAdvancedDialog_->contents());
    xEdit->setPlaceholderText("X");
    yEdit = new WLineEdit(groupAdvancedDialog_->contents());
    yEdit->setPlaceholderText("Y");
    xEdit->resize(40,20);
    yEdit->resize(40,20);
    new WBreak(groupAdvancedDialog_->contents());

    // hue
    new WLabel("Hue: ", groupAdvancedDialog_->contents());
    hue = new WLineEdit(groupAdvancedDialog_->contents());
    hue->setValueText(boost::lexical_cast<string>(group->getHue()));
    new WBreak(groupAdvancedDialog_->contents());

    // saturation
    new WLabel("Saturation: ", groupAdvancedDialog_->contents());
    saturation = new WLineEdit(groupAdvancedDialog_->contents());
    saturation->setValueText(boost::lexical_cast<string>(group->getSat()));
    new WBreak(groupAdvancedDialog_->contents());

    // CT
    new WLabel("CT: ", groupAdvancedDialog_->contents());
    ct = new WLineEdit(groupAdvancedDialog_->contents());
    ct->setValueText(boost::lexical_cast<string>(group->getCt()));
    new WBreak(groupAdvancedDialog_->contents());

    // alert
    new WLabel("Alert: ", groupAdvancedDialog_->contents());
    alert = new WComboBox(groupAdvancedDialog_->contents());
    alert->addItem("none");
    alert->addItem("colorloop");
    alert->setCurrentIndex(boost::lexical_cast<string>(group->getAlert()) == "none" ? 0 : 1);
    new WBreak(groupAdvancedDialog_->contents());

    // effect
    new WLabel("Effect: ", groupAdvancedDialog_->contents());
    effect = new WComboBox(groupAdvancedDialog_->contents());
    effect->addItem("null");
    effect->addItem("select");
    effect->setCurrentIndex(boost::lexical_cast<string>(group->getEffect()) == "null" ? 0 : 1);
    new WBreak(groupAdvancedDialog_->contents());

    // reachable
    new WLabel("Reachable: ", groupAdvancedDialog_->contents());
    reachable = new WComboBox(groupAdvancedDialog_->contents());
    reachable->addItem("null");
    new WBreak(groupAdvancedDialog_->contents());

    // disable all fields while group is off
    onButtonGroup->checkedChanged().connect(bind([=] {
        bool onStatus = onButtonGroup->checkedButton()->text() == "On" ? 1 : 0;
        brightnessSlider_->setDisabled(!onStatus);
        xEdit->setDisabled(!onStatus);
        yEdit->setDisabled(!onStatus);
        hue->setDisabled(!onStatus);
        saturation->setDisabled(!onStatus);
        ct->setDisabled(!onStatus);
        alert->setDisabled(!onStatus);
        effect->setDisabled(!onStatus);
        reachable->setDisabled(!onStatus);
    }));

    // make okay and cancel buttons, cancel sends a reject dialogstate, okay sends an accept
    WPushButton *ok = new WPushButton("OK", groupAdvancedDialog_->contents());
    WPushButton *cancel = new WPushButton("Cancel", groupAdvancedDialog_->contents());
    
    ok->clicked().connect(groupAdvancedDialog_, &WDialog::accept);
    cancel->clicked().connect(groupAdvancedDialog_, &WDialog::reject);
    
    // when the user is finished, call function to update group
    groupAdvancedDialog_->finished().connect(boost::bind(&LightManagementWidget::groupUpdateAdvanced, this, group));
    groupAdvancedDialog_->show();
}

void LightManagementWidget::groupUpdateAdvanced(Group *group){
    if (groupAdvancedDialog_->result() == WDialog::DialogCode::Rejected)
        return;
    
    string on;
    if(onButtonGroup->checkedButton() != 0) {
        on = onButtonGroup->checkedButton()->text() == "On" ? "1" : "0";
    }
    cout << on << '\n';
    if(on == "1") {
    cout << boost::lexical_cast<string>(brightnessSlider_->value()) << '\n';
    cout << boost::lexical_cast<string>(xEdit->text().toUTF8()) << '\n';
    cout << boost::lexical_cast<string>(yEdit->text().toUTF8()) << '\n';
    cout << boost::lexical_cast<string>(hue->text().toUTF8()) << '\n';
    cout << boost::lexical_cast<string>(saturation->text().toUTF8()) << '\n';
    cout << boost::lexical_cast<string>(ct->text().toUTF8()) << '\n';
    cout << boost::lexical_cast<string>(alert->valueText().toUTF8()) << '\n';
    cout << boost::lexical_cast<string>(effect->valueText().toUTF8()) << '\n';
    cout << boost::lexical_cast<string>(reachable->valueText().toUTF8()) << '\n';
    }
}

void LightManagementWidget::updateSchedulesTable() {
    schedulesTable_->clear();
    
    // create row for headers table
    WTableRow *tableRow = schedulesTable_->insertRow(schedulesTable_->rowCount());
    //table headers <th>
    tableRow->elementAt(0)->addWidget(new WText("Name"));
    tableRow->elementAt(1)->addWidget(new WText("Description"));
    tableRow->elementAt(2)->addWidget(new WText("Action"));
    tableRow->elementAt(3)->addWidget(new WText("Time"));
    
    //convert json string into json object
    Json::Object bridgeJson;
    Json::parse(bridge_->getJson(), bridgeJson);
    Json::Object schedules = bridgeJson.get("schedules");

    set<string> data = schedules.names();
    for(string num : data) {
        Json::Object scheduleData = schedules.get(num);
        Schedule *schedule = new Schedule(num, scheduleData);

        tableRow = schedulesTable_->insertRow(schedulesTable_->rowCount());
        
        tableRow->elementAt(0)->addWidget(new WText(schedule->getName()));
        tableRow->elementAt(1)->addWidget(new WText(schedule->getDescription()));

        tableRow->elementAt(2)->addWidget(new WText(schedule->getMethod().toUTF8()));
        tableRow->elementAt(2)->addWidget(new WBreak());

        tableRow->elementAt(2)->addWidget(new WText(schedule->getAddress().toUTF8()));
        tableRow->elementAt(2)->addWidget(new WBreak());

        if(schedule->getMethod().toUTF8() != "DELETE") {
            string onState = schedule->getOn() == 1 ? "true" : "false";
            tableRow->elementAt(2)->addWidget(new WText("On: " + onState));
            tableRow->elementAt(2)->addWidget(new WBreak());

            if(schedule->getX() != -1 && schedule->getY() != -1) {
                tableRow->elementAt(2)->addWidget(new WText("Color: [" + boost::lexical_cast<string>(schedule->getX()) + "," + boost::lexical_cast<string>(schedule->getY()) + "]"));
                tableRow->elementAt(2)->addWidget(new WBreak());
            }
            if(schedule->getBri() != -1) {
                tableRow->elementAt(2)->addWidget(new WText("Bri: " + boost::lexical_cast<string>(schedule->getBri())));
                tableRow->elementAt(2)->addWidget(new WBreak());
            }
            if(schedule->getTransition() != -1) {
                tableRow->elementAt(2)->addWidget(new WText("Transition: " + boost::lexical_cast<string>(schedule->getTransition())));
            }
        }

        tableRow->elementAt(3)->addWidget(new WText(schedule->getTime()));

        WPushButton *editScheduleButton = new WPushButton("Edit");
        //editScheduleButton->clicked().connect

        WPushButton *removeScheduleButton = new WPushButton("Remove");
        removeScheduleButton->clicked().connect(boost::bind(&LightManagementWidget::removeSchedule, this, schedule));

        tableRow->elementAt(4)->addWidget(editScheduleButton);
        tableRow->elementAt(4)->addWidget(removeScheduleButton);
    }
}

void LightManagementWidget::removeSchedule(Schedule *schedule) {
    string url = "http://" + bridge_->getIP() + ":" + bridge_->getPort() + "/api/" + bridge_->getUsername() + "/schedules/" + schedule->getSchedulenum().toUTF8();
    deleteRequest(url);
}

void LightManagementWidget::removeGroup(Group *group) {
    string url = "http://" + bridge_->getIP() + ":" + bridge_->getPort() + "/api/" + bridge_->getUsername() + "/groups/" + group->getGroupnum().toUTF8();
    deleteRequest(url);
}

//NOTE: Does not work on Hue Emulator but would work with real bridge
void LightManagementWidget::removeLight(Light *light) {
    string url = "http://" + bridge_->getIP() + ":" + bridge_->getPort() + "/api/" + bridge_->getUsername() + "/lights/" + light->getLightnum().toUTF8();
    deleteRequest(url);
}

void LightManagementWidget::deleteRequest(string url) {
    Http::Message *data = new Http::Message();

    cout << "DELETE: Deleting at URL " << url << "\n";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->setTimeout(2); //2 second timeout of request
    client->setMaximumResponseSize(1000000);
    client->done().connect(boost::bind(&LightManagementWidget::handlePutHttp, this, _1, _2));

    if(client->deleteRequest(url, *data)) {
        WApplication::instance()->deferRendering();
    }
    else {
        cerr << "DELETE: Error in client->deleteRequest() call\n";
    }
}

void LightManagementWidget::editLightDialog(Light *light) {
    editLightDialog_ = new WDialog("Edit Light #" + boost::lexical_cast<string>(light->getLightnum())); // title
    
    new WLabel("Light Name: ", editLightDialog_->contents());
    editLightName = new WLineEdit(editLightDialog_->contents());
    editLightName->setValueText(light->getName());
    new WBreak(editLightDialog_->contents());
    
    // make okay and cancel buttons, cancel sends a reject dialogstate, okay sends an accept
    WPushButton *ok = new WPushButton("OK", editLightDialog_->contents());
    WPushButton *cancel = new WPushButton("Cancel", editLightDialog_->contents());
    
    ok->clicked().connect(editLightDialog_, &WDialog::accept);
    cancel->clicked().connect(editLightDialog_, &WDialog::reject);
    
    // when the user is finished, call the updateLight function
    editLightDialog_->finished().connect(boost::bind(&LightManagementWidget::updateLightInfo, this, light));
    editLightDialog_->show();
}

void LightManagementWidget::updateLightInfo(Light *light){
    if (editLightDialog_->result() == WDialog::DialogCode::Rejected)
        return;
    
    string url = "http://" + bridge_->getIP() + ":" + bridge_->getPort() + "/api/" + bridge_->getUsername() + "/lights/" + boost::lexical_cast<string>(light->getLightnum());
    
    Http::Message *data = new Http::Message();
    data->addBodyText("{\"name\":\"" + boost::lexical_cast<string>(editLightName->text().toUTF8()) + "\"}");
    
    cout << "Light: Updating light name at URL " << url << "\n";
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

void LightManagementWidget::createScheduleDialog() {
    createScheduleDialog_ = new WDialog("Create a Schedule"); // title
    
    new WLabel("Schedule Name: ", createScheduleDialog_->contents());
    scheduleName = new WLineEdit(createScheduleDialog_->contents());
    scheduleName->setValueText("schedule");
    new WBreak(createScheduleDialog_->contents());
    
    new WLabel("Description: ", createScheduleDialog_->contents());
    description = new WLineEdit(createScheduleDialog_->contents());
    new WBreak(createScheduleDialog_->contents());
    
    //WTemplate *form = new WTemplate(WString::tr("dateEdit-template"), createScheduleDialog_->contents());
    //form->addFunction("id", &WTemplate::Functions::id);

    // Container for Resource Buttons
    Wt::WGroupBox *resourceGroupContainer = new Wt::WGroupBox("Resource", createScheduleDialog_->contents());
    resourceButtonGroup = new Wt::WButtonGroup(this);
    Wt::WRadioButton *resourceRadioButton;
    resourceRadioButton = new Wt::WRadioButton("Light", resourceGroupContainer);
    resourceButtonGroup->addButton(resourceRadioButton, 0);
    resourceRadioButton = new Wt::WRadioButton("Group", resourceGroupContainer);
    resourceButtonGroup->addButton(resourceRadioButton, 1);
    resourceButtonGroup->setCheckedButton(resourceButtonGroup->button(0));
    new WBreak(resourceGroupContainer);
    new WLabel("Light/Group Num: ", resourceGroupContainer);
    resourceNum = new WLineEdit(resourceGroupContainer);
    resourceNum->setValueText("1");
    intValidator = new WIntValidator(0, 100, resourceGroupContainer); //max 100
    intValidator->setMandatory(true);
    resourceNum->setValidator(intValidator);
    new WBreak(createScheduleDialog_->contents());

    // Container for Action Buttons
    Wt::WGroupBox *actionGroupContainer = new Wt::WGroupBox("Action", createScheduleDialog_->contents());
    actionButtonGroup = new Wt::WButtonGroup(this);
    Wt::WRadioButton *actionRadioButton;
    actionRadioButton = new Wt::WRadioButton("Change", actionGroupContainer);
    actionButtonGroup->addButton(actionRadioButton, 0);
    actionRadioButton = new Wt::WRadioButton("Add", actionGroupContainer);
    actionButtonGroup->addButton(actionRadioButton, 1);
    actionRadioButton = new Wt::WRadioButton("Remove", actionGroupContainer);
    actionButtonGroup->addButton(actionRadioButton, 2);
    actionButtonGroup->setCheckedButton(actionButtonGroup->button(0));
    new WBreak(createScheduleDialog_->contents());

    // Container for Body Buttons
    Wt::WGroupBox *bodyGroupContainer = new Wt::WGroupBox("State", createScheduleDialog_->contents());
    // on or off
    onButtonGroup = new Wt::WButtonGroup(this);
    Wt::WRadioButton *onRadioButton;
    onRadioButton = new Wt::WRadioButton("On", bodyGroupContainer);
    onButtonGroup->addButton(onRadioButton, 0);
    onRadioButton = new Wt::WRadioButton("Off", bodyGroupContainer);
    onButtonGroup->addButton(onRadioButton, 1);
    onButtonGroup->setCheckedButton(onButtonGroup->button(0));
    onButtonGroup->checkedChanged().connect(bind([=] {
        bool onStatus = onButtonGroup->checkedButton()->text() == "On" ? 1 : 0;
        brightnessSchedule->setDisabled(!onStatus);
        redSlider->setDisabled(!onStatus);
        greenSlider->setDisabled(!onStatus);
        blueSlider->setDisabled(!onStatus);
        transitionSchedule->setDisabled(!onStatus);
    }));

    new WBreak(bodyGroupContainer);
    // brightness slider
    new WLabel("Brightness: ", bodyGroupContainer);
    brightnessSchedule = new WSlider(bodyGroupContainer);
    brightnessSchedule->resize(200,20);
    brightnessSchedule->setValue(1);
    brightnessSchedule->setMinimum(0);
    brightnessSchedule->setMaximum(254);
    new WBreak(bodyGroupContainer);
    // color picker
    new WLabel("Color: ", bodyGroupContainer);
    bodyGroupContainer->addWidget(rgbContainer_);
    redSlider->setValue(0);
    greenSlider->setValue(0);
    blueSlider->setValue(0);
    WCssDecorationStyle *colour = new WCssDecorationStyle();
    colour->setBackgroundColor(WColor(redSlider->value(), greenSlider->value(), blueSlider->value()));
    rgbContainer_->setDecorationStyle(*colour);
    new WBreak(bodyGroupContainer);
    // transition time
    new WLabel("Transition time: ", bodyGroupContainer);
    transitionSchedule = new WLineEdit(bodyGroupContainer);
    transitionSchedule->resize(40,20);
    transitionSchedule->setValueText("4");
    intValidator = new WIntValidator(0, 100, bodyGroupContainer);//100 maximum
    transitionSchedule->setValidator(intValidator);
    new WText(" seconds", bodyGroupContainer);
    new WBreak(bodyGroupContainer);

    // Container for Date and Time
    Wt::WGroupBox *datetimeGroupContainer = new Wt::WGroupBox("Date & Time", createScheduleDialog_->contents());
    dateEdit = new WDateEdit(datetimeGroupContainer);
    //form->bindWidget("from",dateEdit);
    dateEdit->setDate(WDate::currentServerDate());
    timeEdit = new WTimeEdit(datetimeGroupContainer);
    timeEdit->setTime(WTime::currentTime());

    new WBreak(createScheduleDialog_->contents());

    // make okay and cancel buttons, cancel sends a reject dialogstate, okay sends an accept
    WPushButton *ok = new WPushButton("OK", createScheduleDialog_->contents());
    WPushButton *cancel = new WPushButton("Cancel", createScheduleDialog_->contents());
    
    ok->clicked().connect(createScheduleDialog_, &WDialog::accept);
    cancel->clicked().connect(createScheduleDialog_, &WDialog::reject);
    
    // when the user is finished, call the ADD SCHEDULE function
    createScheduleDialog_->finished().connect(boost::bind(&LightManagementWidget::createSchedule, this));
    createScheduleDialog_->show();
}

void LightManagementWidget::createSchedule() {
    if (createScheduleDialog_->result() == WDialog::DialogCode::Rejected)
        return;
    if (resourceNum->validate() != 2)
        return;

    string name = scheduleName->valueText().toUTF8();
    string desc = description->valueText().toUTF8();
    string date = dateEdit->valueText().toUTF8();
    string time = timeEdit->valueText().toUTF8();
    string localtime = date + "T" + time;

    string resource = "";
    string resourceTwo = "";
    string action = "";
    string on = "";
    string xval = "";
    string yval = "";
    if(resourceButtonGroup->checkedButton()->text() == "Light") {
        resource = "lights";
        resourceTwo = "state";
    }
    else {
        resource = "groups";
        resourceTwo = "action";
    }
    string num = resourceNum->valueText().toUTF8();

    if(actionButtonGroup->checkedButton()->text() == "Change") action = "PUT";
    else if(actionButtonGroup->checkedButton()->text() == "Add") action = "POST";
    else action = "DELETE";

    if(onButtonGroup->checkedButton() != 0) {
        on = onButtonGroup->checkedButton()->text() == "On" ? "1" : "0";
    }

    //if value is 1 user did not change it
    string bri = boost::lexical_cast<string>(brightnessSchedule->value());
    
    if(redSlider->value() != 0 && greenSlider->value() != 0 && blueSlider->value() != 0) {
        struct xy *cols = ColourConvert::rgb2xy(redSlider->value(), greenSlider->value(), blueSlider->value());
        
        xval = boost::lexical_cast<string>(cols->x);
        yval = boost::lexical_cast<string>(cols->y);
    }

    //if value is 4 user did not change it
    int transition = boost::lexical_cast<int>(transitionSchedule->valueText());

    //json formatting
    Json::Object scheduleJSON;
    if(name != "") scheduleJSON["name"] = Json::Value(name);
    if(desc != "") scheduleJSON["description"] = Json::Value(desc);

    Json::Object commandJSON;
    commandJSON["address"] = Json::Value("/api/" + bridge_->getUsername() + "/" + resource + "/" + num + "/" + resourceTwo);
    commandJSON["method"] = Json::Value(action);
    commandJSON["body"] = Json::Value(Json::ObjectType);

    Json::Object bodyJSON;
    if(on != "") bodyJSON["on"] = Json::Value((bool)boost::lexical_cast<int>(on));
    else bodyJSON["on"] = Json::Value(Json::Value::Null);
    if(bri != "1" && on != "0") bodyJSON["bri"] = Json::Value(boost::lexical_cast<int>(bri));
    else bodyJSON["bri"] = Json::Value(Json::Value::Null);
    if(xval != "" && yval != "" && on != "0") {
        Json::Array xyJSON;
        xyJSON.push_back(Json::Value(boost::lexical_cast<double>(xval)));
        xyJSON.push_back(Json::Value(boost::lexical_cast<double>(yval)));
        bodyJSON["xy"] = Json::Value(xyJSON);
    }
    else bodyJSON["xy"] = Json::Value(Json::Value::Null);
    if(transition != 4 && on != "0") bodyJSON["transitiontime"] = Json::Value(transition);
    else bodyJSON["transitiontime"] = Json::Value(Json::Value::Null);

    commandJSON["body"] = Json::Value(bodyJSON);
    scheduleJSON["command"] = Json::Value(commandJSON);
    scheduleJSON["time"] = Json::Value(localtime);

    string url = "http://" + bridge_->getIP() + ":" + bridge_->getPort() + "/api/" + bridge_->getUsername() + "/schedules";
    postRequest(url, Json::serialize(scheduleJSON));
}


void LightManagementWidget::postRequest(string url, string json) {
    Http::Message *data = new Http::Message();
    data->addBodyText(json);

    cout << "POST: Updating at URL " << url << "\n";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->setTimeout(2); //2 second timeout of request
    client->setMaximumResponseSize(1000000);
    client->done().connect(boost::bind(&LightManagementWidget::handlePutHttp, this, _1, _2));

    if(client->post(url, *data)) {
        WApplication::instance()->deferRendering();
    }
    else {
        cerr << "POST: Error in client->post() call\n";
    }
}

void LightManagementWidget::updateLightBri(WSlider *slider_, Light *light){
    string url = "http://" + bridge_->getIP() + ":" + bridge_->getPort() + "/api/" + bridge_->getUsername() + "/lights/" + light->getLightnum().toUTF8() + "/state";
    Http::Message *data = new Http::Message();
    data->addBodyText("{\"bri\":" + boost::lexical_cast<string>(slider_->value()) + ", \"transitiontime\":" + boost::lexical_cast<string>(light->getTransition()) + "}");
    
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

void LightManagementWidget::updateLightOn(WPushButton *button_, Light *light){
    //set value string to reflect current state of the button
    string value = button_->text() == "On" ? "False" : "True";
    
    string url = "http://" + bridge_->getIP() + ":" + bridge_->getPort() + "/api/" + bridge_->getUsername() + "/lights/" + light->getLightnum().toUTF8() + "/state";
    Http::Message *data = new Http::Message();
    
    //can only set transition time while light is on
    if(light->getOn()) {
        data->addBodyText("{\"on\":" + value + ", \"transitiontime\":" + boost::lexical_cast<string>(light->getTransition()) + "}");
    }
    else {
        data->addBodyText("{\"on\":" + value + "}");
    }
    
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

void LightManagementWidget::updateLightXY(Light *light){
    if (editRGBDialog_->result() == WDialog::DialogCode::Rejected)
        return;
    
    string url = "http://" + bridge_->getIP() + ":" + bridge_->getPort() + "/api/" + bridge_->getUsername() + "/lights/" + light->getLightnum().toUTF8() + "/state";
    
    struct xy *cols = ColourConvert::rgb2xy(redSlider->value(), greenSlider->value(), blueSlider->value());
    
    Http::Message *data = new Http::Message();
    data->addBodyText("{\"xy\":[" + boost::lexical_cast<string>(cols->x) + "," + boost::lexical_cast<string>(cols->y) + "]" + ", \"transitiontime\":" + boost::lexical_cast<string>(light->getTransition()) + "}");
    
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
void LightManagementWidget::editRGBDialog(Light *light) {
    
    editRGBDialog_ = new WDialog("Change Colour"); // title
    
    editRGBDialog_->contents()->addWidget(rgbContainer_);
    
    //get current RGB values and set the Slider and BG colours to match
    struct rgb *currentRGBVals = ColourConvert::xy2rgb((float)light->getX(),
                                                       (float)light->getY(),
                                                       (float)light->getBri());
    redSlider->setValue(currentRGBVals->r);
    greenSlider->setValue(currentRGBVals->g);
    blueSlider->setValue(currentRGBVals->b);
    WCssDecorationStyle *colour = new WCssDecorationStyle();
    colour->setBackgroundColor(WColor(redSlider->value(), greenSlider->value(), blueSlider->value()));
    rgbContainer_->setDecorationStyle(*colour);
    
    new WBreak(editRGBDialog_->contents());
    
    // make okay and cancel buttons, cancel sends a reject dialogstate, okay sends an accept
    WPushButton *ok = new WPushButton("OK", editRGBDialog_->contents());
    WPushButton *cancel = new WPushButton("Cancel", editRGBDialog_->contents());
    
    ok->clicked().connect(editRGBDialog_ ,&WDialog::accept);
    cancel->clicked().connect(editRGBDialog_, &WDialog::reject);
    
    // when the user is finished, call the updateBridge function
    editRGBDialog_->finished().connect(boost::bind(&LightManagementWidget::updateLightXY, this, light));
    editRGBDialog_->show();
}
