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
#include "Light.h"
#include "Group.h"
#include "Schedule.h"
#include <Wt/WContainerWidget>
#include <Wt/WMenu>
#include <Wt/WStackedWidget>
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
    
    addWidget(lightManagementStack_);
}

void LightManagementWidget::viewOverviewWidget(){
    if (!overviewWidget_) {
        overviewWidget_ = new WContainerWidget(lightManagementStack_);
        overviewWidget_->setContentAlignment(AlignCenter);
        
        new WText("Bridge Name: " + bridge_->getName(), overviewWidget_);
        new WBreak(overviewWidget_);
        new WText("Bridge Location: " + bridge_->getLocation(), overviewWidget_);
    }
    lightManagementStack_->setCurrentWidget(overviewWidget_);
}

void LightManagementWidget::viewLightsWidget(){
    if (!lightsWidget_) {
        lightsWidget_ = new WContainerWidget(lightManagementStack_);
        lightsWidget_->setContentAlignment(AlignCenter);
        
        lightsTable_ = new WTable(lightsWidget_);
        lightsTable_->setHeaderCount(1); //set first row as header
        updateLightsTable();
    }
    lightManagementStack_->setCurrentWidget(lightsWidget_);
}

void LightManagementWidget::viewGroupsWidget(){
    if (!groupsWidget_) {
        groupsWidget_ = new WContainerWidget(lightManagementStack_);
        groupsWidget_->setContentAlignment(AlignCenter);
        
        groupsTable_ = new WTable(groupsWidget_);
        groupsTable_->setHeaderCount(1); //set first row as header
        updateGroupsTable();
    }
    lightManagementStack_->setCurrentWidget(groupsWidget_);
}

void LightManagementWidget::viewSchedulesWidget(){
    if (!schedulesWidget_) {
        schedulesWidget_ = new WContainerWidget(lightManagementStack_);
        schedulesWidget_->setContentAlignment(AlignCenter);
        
        WPushButton *newScheduleButton = new WPushButton("Add +");
        newScheduleButton->clicked().connect(boost::bind(&LightManagementWidget::createScheduleDialog, this));
        schedulesWidget_->addWidget(newScheduleButton);
        schedulesTable_ = new WTable(schedulesWidget_);
        schedulesTable_->setHeaderCount(1);
        updateSchedulesTable();
    }
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
        brightnessSlider_->valueChanged().connect(boost::bind(&LightManagementWidget::updateLight, this, brightnessSlider_));
        
        tableRow->elementAt(2)->addWidget(brightnessSlider_);
        
        string onButton = light->getOn() == 1 ? "On" : "Off";
        WPushButton *switchButton_ = new WPushButton(onButton);
        //switchButton->clicked().connect(boost::bind(&BridgeScreenWidget::viewBridge, this, counter));
        
        WPushButton *editLightButton_ = new WPushButton("Edit");
        editLightButton_->clicked().connect(boost::bind(&LightManagementWidget::editLight, this, i));
        
        tableRow->elementAt(3)->addWidget(switchButton_);
        tableRow->elementAt(3)->addWidget(editLightButton_);
        
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
