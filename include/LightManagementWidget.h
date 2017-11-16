#ifndef LIGHTMANAGEMENTWIDGET_H
#define LIGHTMANAGEMENTWIDGET_H

#include <Wt/WContainerWidget>
#include "WelcomeScreen.h"

class LightManagementWidget: public Wt::WContainerWidget
{
public:
    LightManagementWidget(Wt::WContainerWidget *parent = 0,
                        Account *account = 0,
                        WelcomeScreen *main = 0);

    void update();
private:
    WelcomeScreen *parent_;
    Account *account_;

};


#endif // LIGHTMANAGEMENTWIDGET_H
