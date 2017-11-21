#ifndef LIGHTMANAGEMENTWIDGET_H
#define LIGHTMANAGEMENTWIDGET_H

#include <Wt/WContainerWidget>
#include "WelcomeScreen.h"

class LightManagementWidget: public Wt::WContainerWidget
{
public:
    LightManagementWidget(Wt::WContainerWidget *parent = 0,
                        Bridge *bridge = 0,
                        WelcomeScreen *main = 0);

    void update();
private:
    WelcomeScreen *parent_;
    Bridge *bridge_;
    
    void displayLights();
    void displayGroups();
    void displaySchedules();

};


#endif // LIGHTMANAGEMENTWIDGET_H
