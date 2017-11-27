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

        //transition definer
        WLineEdit *editLightTransition = new WLineEdit();
        editLightTransition->resize(40,20);
        //display transition time to user in seconds (transition time is stored as multiple of 100ms)
        editLightTransition->setValueText(boost::lexical_cast<string>(light->getTransition() / 10.0));
        tableRow->elementAt(2)->addWidget(editLightTransition);
        editLightTransition->setDisabled(!light->getOn());  //disable if light off
        intValidator = new WIntValidator(0, 100, tableRow->elementAt(2)); //100 second maximum
        intValidator->setMandatory(true);
        editLightTransition->setValidator(intValidator);
        editLightTransition->changed().connect(bind([=] {
            light->setTransition(boost::lexical_cast<int>(editLightTransition->valueText()) * 10);
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
    tableRow->elementAt(2)->addWidget(new Wt::WText("Transitions"));
    tableRow->elementAt(3)->addWidget(new Wt::WText("Brightness"));
    tableRow->elementAt(4)->addWidget(new Wt::WText("Lights"));
    tableRow->elementAt(5)->addWidget(new Wt::WText("Color"));
    tableRow->elementAt(6)->addWidget(new Wt::WText("Actions"));
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

        //transition definer
        WLineEdit *editGroupTransition = new WLineEdit();
        editGroupTransition->resize(40,20);
        //display transition time to user in seconds (transition time is stored as multiple of 100ms)
        editGroupTransition->setValueText(boost::lexical_cast<string>(group->getTransition() / 10.0));
        tableRow->elementAt(2)->addWidget(editGroupTransition);
        editGroupTransition->setDisabled(!group->getOn());  //disable if light off
        intValidator = new WIntValidator(0, 100, tableRow->elementAt(2)); //100 second maximum
        intValidator->setMandatory(true);
        editGroupTransition->setValidator(intValidator);
        editGroupTransition->changed().connect(bind([=] {
            group->setTransition(boost::lexical_cast<int>(editGroupTransition->valueText()) * 10);
        }));
        tableRow->elementAt(2)->addWidget(new WText(" seconds"));

        //brightness slider
        WSlider *brightnessSlider_ = new WSlider();
        brightnessSlider_->resize(200,20);
        brightnessSlider_->setMinimum(0);

        brightnessSlider_->setMaximum(254);
        brightnessSlider_->setValue(group->getBri());
        brightnessSlider_->setDisabled(!group->getOn()); //disable if light off
        brightnessSlider_->valueChanged().connect(boost::bind(&LightManagementWidget::updateGroupBri, this, brightnessSlider_, group));
        tableRow->elementAt(3)->addWidget(brightnessSlider_);

        for(WString lightNum : group->getLights()) {
            tableRow->elementAt(4)->addWidget(new WText(lightNum + " "));
        }

        string onButton = group->getOn() == 1 ? "On" : "Off";
        WPushButton *switchButton_ = new WPushButton(onButton);
        switchButton_->clicked().connect(boost::bind(&LightManagementWidget::updateGroupOn, this, switchButton_, group));
        tableRow->elementAt(5)->addWidget(switchButton_);

        // color stuff

        WPushButton *advancedButton_ = new WPushButton("Advanced");
        advancedButton_->clicked().connect(boost::bind(&LightManagementWidget::groupAdvancedDialog, this, group));
        tableRow->elementAt(6)->addWidget(advancedButton_);
        i++;
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
    int i = 1;
    while(true) {
        //if light i does not exist then break while loop
        if(lights.isNull(boost::lexical_cast<string>(i))) break;

        //if light i exists then get the json data for it and make Light object
        Json::Object lightData = lights.get(boost::lexical_cast<string>(i));
        Light *light = new Light(boost::lexical_cast<string>(i), lightData);

        WPushButton *lightButton_ = new WPushButton(createGroupDialog_->contents());
        //lightButton_->clicked().connect(boost::bind(&Group::addLight, group, light->getLightnum()));
        i++;
    }
    new WBreak(createGroupDialog_->contents());

    // make okay and cancel buttons, cancel sends a reject dialogstate, okay sends an accept
    WPushButton *ok = new WPushButton("OK", createGroupDialog_->contents());
    WPushButton *cancel = new WPushButton("Cancel", createGroupDialog_->contents());

    ok->clicked().connect(createGroupDialog_, &WDialog::accept);
    cancel->clicked().connect(createGroupDialog_, &WDialog::reject);

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

    new WLabel("Hue: ", groupAdvancedDialog_->contents());
    WLineEdit *hue = new WLineEdit(groupAdvancedDialog_->contents());
    hue->setValueText(boost::lexical_cast<string>(group->getHue()));
    new WBreak(groupAdvancedDialog_->contents());

    new WLabel("Saturation: ", groupAdvancedDialog_->contents());
    WLineEdit *saturation = new WLineEdit(groupAdvancedDialog_->contents());
    saturation->setValueText(boost::lexical_cast<string>(group->getSat()));
    new WBreak(groupAdvancedDialog_->contents());

    new WLabel("CT: ", groupAdvancedDialog_->contents());
    WLineEdit *ct = new WLineEdit(groupAdvancedDialog_->contents());
    saturation->setValueText(boost::lexical_cast<string>(group->getCt()));
    new WBreak(groupAdvancedDialog_->contents());

    new WLabel("Alert: ", groupAdvancedDialog_->contents());
    WComboBox *alert = new WComboBox(groupAdvancedDialog_->contents());
    alert->addItem("none");
    alert->addItem("colorloop");
    alert->setCurrentIndex(boost::lexical_cast<string>(group->getAlert()) == "none" ? 0 : 1);
    new WBreak(groupAdvancedDialog_->contents());

    new WLabel("Effect: ", groupAdvancedDialog_->contents());
    WComboBox *effect = new WComboBox(groupAdvancedDialog_->contents());
    effect->addItem("null");
    effect->addItem("select");
    effect->setCurrentIndex(boost::lexical_cast<string>(group->getEffect()) == "null" ? 0 : 1);
    new WBreak(groupAdvancedDialog_->contents());

    new WLabel("Color Mode: ", groupAdvancedDialog_->contents());
    WComboBox *colormode = new WComboBox(groupAdvancedDialog_->contents());
    colormode->addItem("xy");
    colormode->addItem("hs");
    colormode->setCurrentIndex(boost::lexical_cast<string>(group->getColormode()) == "xy" ? 0 : 1);
    new WBreak(groupAdvancedDialog_->contents());

    new WLabel("Reachable: ", groupAdvancedDialog_->contents());
    WComboBox *reachable = new WComboBox(groupAdvancedDialog_->contents());
    reachable->addItem("null");
    new WBreak(groupAdvancedDialog_->contents());

    // make okay and cancel buttons, cancel sends a reject dialogstate, okay sends an accept
    WPushButton *ok = new WPushButton("OK", groupAdvancedDialog_->contents());
    WPushButton *cancel = new WPushButton("Cancel", groupAdvancedDialog_->contents());

    ok->clicked().connect(groupAdvancedDialog_, &WDialog::accept);
    cancel->clicked().connect(groupAdvancedDialog_, &WDialog::reject);

    // when the user is finished, call function to update group
    //groupAdvancedDialog_->finished().connect(boost::bind(&LightManagementWidget::udpateAdvanced, this, etc));
    groupAdvancedDialog_->show();
}
/*
void LightManagementWidget::updateAdvanced(){

}
*/
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
    groupAdvancedDialog_->show();
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
    editRGBDialog_->finished().connect(boost::bind(&LightManagementWidget::updateLightXY, this, light));
    editRGBDialog_->show();
}
