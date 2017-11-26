#ifndef LIGHTMANAGEMENTWIDGET_H
#define LIGHTMANAGEMENTWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WTable>
#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WSlider>
#include <Wt/WIntValidator>
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
    Wt::WDialog *createScheduleDialog_;
    
    void refreshBridge();
    void refreshBridgeHttp(boost::system::error_code err, const Wt::Http::Message &response);
};


#endif // LIGHTMANAGEMENTWIDGET_H
