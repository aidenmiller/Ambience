#ifndef GROUP_H
#define GROUP_H

#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <Wt/Json/Value>
#include <Wt/Json/Object>
#include <Wt/Json/Parser>
#include <Wt/Json/Array>


using namespace std;
using namespace Wt;

class Group {
    
public:
    Group(WString groupNum, Json::Object groupData);
    
    virtual ~Group();
    
    //GETTERS
    WString getGroupnum() {return groupnum_;}
    WString getName() {return name_;}
    WString getAlert() {return alert_;}
    int getBri() {return bri_;}
    WString getColormode() {return colormode_;}
    int getCt() {return ct_;}
    WString getEffect() {return effect_;}
    int getHue() {return hue_;}
    bool getOn() {return on_;}
    bool getReachable() {return reachable_;}
    int getSat() {return sat_;}
    double getX() {return xy_[0];}
    double getY() {return xy_[1];}
    vector<WString> getLights() {return lights_;}
    int getNumLights() {return lights_.size();}
    
    //SETTERS
    void setGroupnum(WString groupnum) {groupnum_ = groupnum;}
    void setName(WString name) {name_ = name;}
    void setAlert(WString alert) {alert_ = alert;}
    void setBri(int bri) {bri_ = bri;}
    void setColormode(WString colormode) {colormode_ = colormode;}
    void setCt(int ct) {ct_ = ct;}
    void setEffect(WString effect) {effect_ = effect;}
    void setHue(int hue) {hue_ = hue;}
    void setOn(bool on) {on_ = on;}
    void setReachable(bool reachable) {reachable_ = reachable;}
    void setSat(int sat) {sat_ = sat;}
    void setX(double x) {xy_[0] = x;}
    void setY(double y) {xy_[1] = y;}
    void addLight(WString lightNum) {lights_.push_back(lightNum);}
    
    void toggleOnOff() {on_ = !on_;}
    
private:
    WString groupnum_;
    WString name_;
    WString alert_;
    int bri_;
    WString colormode_;
    int ct_;
    WString effect_;
    int hue_;
    bool on_;
    bool reachable_;
    int sat_;
    double xy_[2];
    vector<WString> lights_;
};

#endif //GROUP_H
