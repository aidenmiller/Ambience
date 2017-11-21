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
    setContentAlignment(AlignCenter);
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
    
    displayLights();
    new WBreak(this);
    new WBreak(this);
    
    displayGroups();
    new WBreak(this);
    new WBreak(this);
    
    displaySchedules();
    new WBreak(this);
    new WBreak(this);
}

void LightManagementWidget::displayLights() {
    //convert json string into json object
    Json::Object bridgeJson;
    Json::parse(bridge_->getJson(), bridgeJson);
    
    // Lights header
    WText *lightsTitle = new WText("Lights", this);
    lightsTitle->setStyleClass("title");
    new WBreak(this);
    
    Json::Object lights = bridgeJson.get("lights");
    int i = 1;
    while(true) {
        if(lights.isNull(boost::lexical_cast<string>(i))) break;
        Json::Object light = lights.get(boost::lexical_cast<string>(i));
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
    }
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
