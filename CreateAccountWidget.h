#ifndef CREATE_ACCOUNT_WIDGET_H
#define CREATE_ACCOUNT_WIDGET_H

#include <Wt/WContainerWidget>

class CreateAccountWidget: public Wt::WContainerWidget
{
public:
    CreateAccountWidget(Wt::WContainerWidget *parent = 0);

    void update();
private:
};

#endif //CREATE_ACCOUNT_WIDGET_H
