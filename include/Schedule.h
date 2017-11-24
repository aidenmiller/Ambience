#ifndef SCHEDULE_H
#define SCHEDULE_H

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

class Schedule {
    
public:
    Schedule(WString scheduleNum, Json::Object scheduleData);
    
    virtual ~Schedule();
    
    //GETTERS
    WString getSchedulenum() {return schedulenum_;}
    WString getName() {return name_;}
    WString getDescription() {return description_;}
    WString getTime() {return time_;}
    WString getAddress() {return address_;}
    WString getMethod() {return method_;}
    int getBri() {return bri_;}
    int getTransition() {return transition_;}
    bool getOn() {return on_;}
    double getX() {return xy_[0];}
    double getY() {return xy_[1];}
    
    //SETTERS
    void setSchedulenum(WString schedulenum) {schedulenum_ = schedulenum;}
    void setName(WString name) {name_ = name;}
    void setDescription(WString description) {description_ = description;}
    void setTime(WString time) {time_ = time;}
    void setAddress(WString address) {address_ = address;}
    void setMethod(WString method) {method_ = method;}
    void setBri(int bri) {bri_ = bri;}
    void setTransition(int transition) {transition_ = transition;}
    void setOn(bool on) {on_ = on;}
    void setX(double x) {xy_[0] = x;}
    void setY(double y) {xy_[1] = y;}
    
private:
    WString schedulenum_;
    WString name_;
    WString description_;
    WString time_;
    WString address_;
    WString method_;
    int bri_;
    int transition_;
    bool on_;
    double xy_[2];
};

#endif //SCHEDULE_H
