#ifndef LIGHTMANAGEMENTWIDGET_H
#define LIGHTMANAGEMENTWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WTable>
#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WSlider>
#include <Wt/WIntValidator>
#include <Wt/WCalendar>
#include <Wt/WDate>
#include <Wt/WDateEdit>
#include <Wt/WTemplate>
#include <Wt/WTimeEdit>
#include <Wt/WButtonGroup>
#include <Wt/WGroupBox>
#include <Wt/WRadioButton>
#include <Wt/WCheckBox>
#include "WelcomeScreen.h"
#include "Bridge.h"
#include "Light.h"
#include "Group.h"
#include "Schedule.h"
#include "ColourConvert.h"

class LightManagementWidget: public Wt::WContainerWidget
{
public:
    LightManagementWidget(Wt::WContainerWidget *parent = 0,
                        Bridge *bridge = 0,
                        WelcomeScreen *main = 0);

    void update();
private:
    Wt::WContainerWidget *lightsContainer_;
    WelcomeScreen *parent_;
    Bridge *bridge_;

    Wt::WStackedWidget *lightManagementStack_; // main stack of the screen

    void viewOverviewWidget();
    void viewLightsWidget();
    void viewGroupsWidget();
    void viewSchedulesWidget();
    void updateLightsTable();
    void updateGroupsTable();
    void updateSchedulesTable();
    Wt::WContainerWidget *overviewWidget_;
    Wt::WContainerWidget *lightsWidget_;
    Wt::WContainerWidget *groupsWidget_;
    Wt::WContainerWidget *schedulesWidget_;

    void updateLightBri(WSlider *slider_, Light *light);
    void updateLightOn(WPushButton *button_, Light *light);
    void updateLightXY(Light *light);
    void updateLightInfo(Light *light);
    void handlePutHttp(boost::system::error_code err, const Wt::Http::Message &response);

    void editRGBDialog(Light *light);
    Wt::WDialog *editRGBDialog_;
    Wt::WSlider *redSlider;
    Wt::WSlider *greenSlider;
    Wt::WSlider *blueSlider;

    void editLightDialog(Light *light);
    Wt::WDialog *editLightDialog_;
    Wt::WLineEdit *editLightName;

    Wt::WLineEdit *editLightTransition;
    Wt::WIntValidator *intValidator;

    Wt::WTable *lightsTable_;
    Wt::WTable *groupsTable_;
    Wt::WTable *schedulesTable_;
    
    void createScheduleDialog();
    void createSchedule();
    void removeSchedule(Schedule *schedule);
    void removeGroup(Group *group);
    void removeLight(Light *light);
    
    //objects from createScheduleDialog()
    Wt::WDialog *createScheduleDialog_;
    Wt::WLineEdit *scheduleName;
    Wt::WLineEdit *description;
    Wt::WDateEdit *dateEdit;
    Wt::WTimeEdit *timeEdit;
    Wt::WButtonGroup *resourceButtonGroup;
    WLineEdit *resourceNum;
    Wt::WButtonGroup *actionButtonGroup;
    Wt::WButtonGroup *onButtonGroup;
    WSlider *brightnessSchedule;
    WLineEdit *xEdit;
    WLineEdit *yEdit;
    WLineEdit *transitionSchedule;
    
    void postRequest(string url, string json);
    void deleteRequest(string url);

    Wt::WDialog *createGroupDialog_;
    Wt::WDialog *groupAdvancedDialog_;
    void createGroupDialog();
    void updateGroupBri(WSlider *slider_, Group *group);
    void updateGroupOn(WPushButton *button_, Group *group);
    void groupAdvancedDialog(Group *group);

    void refreshBridge();
    void refreshBridgeHttp(boost::system::error_code err, const Wt::Http::Message &response);
};


#endif // LIGHTMANAGEMENTWIDGET_H
