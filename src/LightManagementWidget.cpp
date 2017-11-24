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
#include "Bridge.h"
#include <Wt/WContainerWidget>
#include <Wt/WMenu>
#include <Wt/WStackedWidget>
#include <Wt/Json/Value>
#include <Wt/Json/Object>
#include <Wt/Json/Parser>
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
WContainerWidget(parent)
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




    // Bridge Info
    WText *bridgeInfo = new WText("Connected to " + bridge_->getName() + " located in " + bridge_->getLocation(), this);
    bridgeInfo->setStyleClass("title");
    new WBreak(this);

    WMenu *menu = new WMenu();
    addWidget(menu);

    menu->addItem("Overview");
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

    createLightWidget();
    createSchedulesWidget();
    createGroupsWidget();

    lightManagementStack_->addWidget(groupsWidget_);
    lightManagementStack_->addWidget(schedulesWidget_);

    lightManagementStack_->addWidget(lightsWidget_);
    lightManagementStack_->setContentAlignment(AlignCenter);

    menu->setStyleClass("nav nav-pills nav-stacked");
    menu->setWidth(150);





    addWidget(lightManagementStack_);




/*
    //displayLights();
    new WBreak(this);
    new WBreak(this);

  //  displayGroups();
    new WBreak(this);
    new WBreak(this);

    displaySchedules();
    new WBreak(this);
    new WBreak(this); */
}

void LightManagementWidget::createLightWidget(){
        lightsWidget_ = new WContainerWidget();
        lightsWidget_->setContentAlignment(AlignCenter);
        lightsTable_ = new WTable(lightsWidget_);
        lightsTable_->setHeaderCount(1); //set first row as header
        displayLights();
}
void LightManagementWidget::viewLightsWidget(){
    lightManagementStack_->setCurrentWidget(lightsWidget_);
}

void LightManagementWidget::createGroupsWidget(){

        groupsWidget_ = new WContainerWidget();
        new WText("I am part of groupsWidget_", groupsWidget_);
}
void LightManagementWidget::viewGroupsWidget(){
    lightManagementStack_->setCurrentWidget(groupsWidget_);

}

void LightManagementWidget::createSchedulesWidget(){
        schedulesWidget_ = new WContainerWidget();
        new WText("I am part of scedulesWidget_", schedulesWidget_);
}

void LightManagementWidget::viewSchedulesWidget(){

    lightManagementStack_->setCurrentWidget(schedulesWidget_);

}

void LightManagementWidget::displayLights() {
    lightsTable_->clear();

    //convert json string into json object
    Json::Object bridgeJson;
    Json::parse(bridge_->getJson(), bridgeJson);

    Json::Object lights = bridgeJson.get("lights");
    int i = 1;

    //create new row for headers <tr>
    WTableRow *tableRow = lightsTable_->insertRow(lightsTable_->rowCount());
    //table headers <th>
    tableRow->elementAt(0)->addWidget(new Wt::WText("Lights"));

    while(true) {
        if(lights.isNull(boost::lexical_cast<string>(i))) break;
        Json::Object light = lights.get(boost::lexical_cast<string>(i));

        //create new row for entry <tr>
        tableRow = lightsTable_->insertRow(lightsTable_->rowCount());

        //table data <td>
        //tableRow->elementAt(0)->addWidget(new Wt::WText(light.get("name")));
        tableRow->elementAt(0)->addWidget(new Wt::WText("name"));

        // brightness slider
        tableRow->elementAt(1)->addWidget(new WLabel("Brightness: "));

        WSlider *brightnessSlider_ = new WSlider();
        brightnessSlider_->resize(200,20);
        brightnessSlider_->setMinimum(0);
        brightnessSlider_->setMaximum(100);
        brightnessSlider_->valueChanged().connect(boost::bind(&LightManagementWidget::updateLight, this, brightnessSlider_)); //have to pass the whole damn slider for some reason

        tableRow->elementAt(1)->addWidget(brightnessSlider_);

        WPushButton *switchButton_ = new WPushButton("On/Off");
        //switchButton->clicked().connect(boost::bind(&BridgeScreenWidget::viewBridge, this, counter));

        WPushButton *editLightButton_ = new WPushButton("Edit");
        editLightButton_->clicked().connect(boost::bind(&LightManagementWidget::editLight, this, i));

        tableRow->elementAt(2)->addWidget(switchButton_);
        tableRow->elementAt(2)->addWidget(editLightButton_);

        i++;
    }

    /*
        new WText("Light #" + boost::lexical_cast<string>(i), this);
        new WBreak(this);

        WString name = light.get("name");
        new WText("Name: " + name, this);
        new WBreak(this);

        WString type = light.get("type");
        new WText("Type: " + type, this);
        new WBreak(this);

        WString modelid = light.get("modelid");
        new WText("ModelID: " + modelid, this);
        new WBreak(this);

        new WText("**Light State Info**", this);
        new WBreak(this);

        Json::Object state = light.get("state");
        WString alert = state.get("alert");
        new WText("Alert: " + alert, this);
        new WBreak(this);

        int bri = state.get("bri");
        new WText("Bri: " + boost::lexical_cast<string>(bri), this);
        new WBreak(this);

        WString colormode = state.get("colormode");
        new WText("Colormode: " + colormode, this);
        new WBreak(this);

        int ct = state.get("ct");
        new WText("Ct: " + boost::lexical_cast<string>(ct), this);
        new WBreak(this);

        WString effect = state.get("effect");
        new WText("Effect: " + effect, this);
        new WBreak(this);

        int hue = state.get("hue");
        new WText("Hue: " + boost::lexical_cast<string>(hue), this);
        new WBreak(this);

        bool on = state.get("on");
        string onstr = on == 1 ? "True" : "False";
        new WText("On: " + onstr, this);
        new WBreak(this);

        bool reachable = state.get("reachable");
        string reachablestr = reachable == 1 ? "True" : "False";
        new WText("Reachable: " + reachablestr, this);
        new WBreak(this);

        int sat = state.get("sat");
        new WText("Sat: " + boost::lexical_cast<string>(sat), this);
        new WBreak(this);

        Json::Array xy = state.get("xy");

        double x = xy[0];
        new WText("X: " + boost::lexical_cast<string>(x), this);
        new WBreak(this);

        double y = xy[1];
        new WText("Y: " + boost::lexical_cast<string>(y), this);
        new WBreak(this);

        new WBreak(this); //line to separate different lights

        i++;
        */
}

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

void LightManagementWidget::updateLight(WSlider *slider_){
    cout << endl << slider_->value() << endl;
}

void LightManagementWidget::displayGroups() {
    //convert json string into json object
    Json::Object bridgeJson;
    Json::parse(bridge_->getJson(), bridgeJson);

    // Groups header
    WText *groupsTitle = new WText("Groups", this);
    groupsTitle->setStyleClass("title");
    new WBreak(this);
    Json::Object groups = bridgeJson.get("groups");
    int i = 1;
    while(true) {
        if(groups.isNull(boost::lexical_cast<string>(i))) break;
        Json::Object group = groups.get(boost::lexical_cast<string>(i));
        new WText("Group #" + boost::lexical_cast<string>(i), this);
        new WBreak(this);

        WString name = group.get("name");
        new WText("Name: " + name, this);
        new WBreak(this);

        new WText("**Action**", this);
        new WBreak(this);

        Json::Object action = group.get("action");
        int bri = action.get("bri");
        new WText("Bri: " + boost::lexical_cast<string>(bri), this);
        new WBreak(this);

        WString colormode = action.get("colormode");
        new WText("Colormode: " + colormode, this);
        new WBreak(this);

        int ct = action.get("ct");
        new WText("Ct: " + boost::lexical_cast<string>(ct), this);
        new WBreak(this);

        if(action.type("alert") != 0){
            WString alert = action.get("alert");
            new WText("Alert: " + alert, this);
            new WBreak(this);
        }

        if(action.type("reachable") != 0){
            bool reachable = action.get("reachable");
            string reachablestr = reachable == 1 ? "True" : "False";
            new WText("Reachable: " + reachablestr, this);
            new WBreak(this);
        }

        WString effect = action.get("effect");
        new WText("Effect: " + effect, this);
        new WBreak(this);

        int hue = action.get("hue");
        new WText("Hue: " + boost::lexical_cast<string>(hue), this);
        new WBreak(this);

        bool on = action.get("on");
        string onstr = on == 1 ? "True" : "False";
        new WText("On: " + onstr, this);
        new WBreak(this);

        int sat = action.get("sat");
        new WText("Sat: " + boost::lexical_cast<string>(sat), this);
        new WBreak(this);

        Json::Array xy = action.get("xy");
        double x = xy[0];
        new WText("X: " + boost::lexical_cast<string>(x), this);
        new WBreak(this);

        double y = xy[1];
        new WText("Y: " + boost::lexical_cast<string>(y), this);
        new WBreak(this);

        new WText("**Lights**", this);
        new WBreak(this);
        Json::Array lights = group.get("lights");

        for(Json::Value val : lights) {
            WString lightnum = val;
            new WText("Light: " + lightnum, this);
            new WBreak(this);
        }

        new WBreak(this); //line to separate different groups

        i++;
    }
}

void LightManagementWidget::displaySchedules() {
    //convert json string into json object
    Json::Object bridgeJson;
    Json::parse(bridge_->getJson(), bridgeJson);

    // Schedules header
    WText *schedulesTitle = new WText("Schedules", this);
    schedulesTitle->setStyleClass("title");
    new WBreak(this);
    Json::Object schedules = bridgeJson.get("schedules");
    int i = 1;
    while(true) {
        if(schedules.isNull(boost::lexical_cast<string>(i))) break;
        Json::Object schedule = schedules.get(boost::lexical_cast<string>(i));
        new WText("Schedule #" + boost::lexical_cast<string>(i), this);
        new WBreak(this);

        WString time = schedule.get("time");
        new WText("Time: " + time, this);
        new WBreak(this);

        WString description = schedule.get("description");
        new WText("Description: " + description, this);
        new WBreak(this);

        WString name = schedule.get("name");
        new WText("Name: " + name, this);
        new WBreak(this);

        new WText("**Command**", this);
        new WBreak(this);

        Json::Object command = schedule.get("command");
        WString address = command.get("address");
        new WText("Address: " + address, this);
        new WBreak(this);

        WString method = command.get("method");
        new WText("Method: " + method, this);
        new WBreak(this);

        new WText("****Body****", this);
        new WBreak(this);
        Json::Object body = command.get("body");

        bool on = body.get("on");
        string onstr = on == 1 ? "True" : "False";
        new WText("On: " + onstr, this);
        new WBreak(this);

        if(body.type("xy") != 0) {
            Json::Array xy = body.get("xy");
            double x = xy[0];
            new WText("X: " + boost::lexical_cast<string>(x), this);
            new WBreak(this);

            double y = xy[1];
            new WText("Y: " + boost::lexical_cast<string>(y), this);
            new WBreak(this);
        }
        if(body.type("bri") != 0) {
            int bri = body.get("bri");
            new WText("Bri: " + boost::lexical_cast<string>(bri), this);
            new WBreak(this);
        }
        if(body.type("transition") != 0) {
            int transition = body.get("transition");
            new WText("Transition: " + boost::lexical_cast<string>(transition), this);
            new WBreak(this);
        }

        new WBreak(this); //line to separate different schedules

        i++;
    }
}
