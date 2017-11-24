#ifndef LIGHTMANAGEMENTWIDGET_H
#define LIGHTMANAGEMENTWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WTable>
#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WSlider>
#include "WelcomeScreen.h"

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

    void displayLights();
    void displayGroups();
    void displaySchedules();
    Wt::WStackedWidget *lightManagementStack_; // main stack of the screen

    void viewOverviewWidget();
    void viewLightsWidget();
    void viewGroupsWidget();
    void viewSchedulesWidget();
    void createOverviewWidget();
    void createLightWidget();
    void createGroupsWidget();
    void createSchedulesWidget();
    Wt::WContainerWidget *overviewWidget_;
    Wt::WContainerWidget *lightsWidget_;
    Wt::WContainerWidget *groupsWidget_;
    Wt::WContainerWidget *schedulesWidget_;

    void editLight(int pos);
    void updateLight(WSlider *slider_);

    Wt::WDialog *lightEditDialog_;
    Wt::WLineEdit *lightEditName_;
    Wt::WTable *lightsTable_;
};


#endif // LIGHTMANAGEMENTWIDGET_H
