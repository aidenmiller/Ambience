#include <Wt/WBreak>
#include <Wt/WText>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>

#include "LoginWidget.h"

using namespace Wt;

LoginWidget::LoginWidget(WContainerWidget *parent):
  WContainerWidget(parent)
{
  setContentAlignment(AlignCenter);
}

void LoginWidget::update()
{
    clear();
    new WText("User ID: ", this);
    idEdit = new WLineEdit();
    addWidget(idEdit);
    new WBreak(this);

    new WText("Password: ", this);
    pwEdit = new WLineEdit();
    addWidget(pwEdit);
    new WBreak(this);

    loginButton = new WPushButton("Login");
    addWidget(loginButton);
}
