#include <Wt/WText>
#include <Wt/WTable>

#include "CreateAccountWidget.h"

using namespace Wt;

CreateAccountWidget::CreateAccountWidget(WContainerWidget *parent):
  WContainerWidget(parent)
{
  setContentAlignment(AlignCenter);
}

void CreateAccountWidget::update()
{
    clear();
    new WText("Create Account Page", this);
}
