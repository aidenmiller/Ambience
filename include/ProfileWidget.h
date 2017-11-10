#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WInPlaceEdit>
#include "WelcomeScreen.h"


class ProfileWidget: public Wt::WContainerWidget
{
public:
    ProfileWidget(Wt::WContainerWidget *parent = 0,
                Account *account = 0,
                WelcomeScreen *main = 0);
    void update();

private:

    WelcomeScreen *parent_;
    Account *account_;
    Wt::WInPlaceEdit *editableFirstName_;
    Wt::WInPlaceEdit *editableLastName_;

    void updateFirstName();
    void updateLastName();

};

#endif //PROFILEWIDGET_H
