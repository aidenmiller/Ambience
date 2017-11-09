#include <Wt/WApplication>
#include <Wt/WServer>

#include "WelcomeScreen.h"

using namespace Wt;
using namespace std;

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
    Wt::WApplication *app = new Wt::WApplication(env);

    app->setTitle("Hue Light App- Team13");

    app->useStyleSheet("style/stylesheet.css");

    new WelcomeScreen(app->root());
    return app;
}

int main(int argc, char **argv)
{
  try {
    Wt::WServer server(argc, argv, WTHTTP_CONFIGURATION);

    server.addEntryPoint(Wt::Application, createApplication);

    server.run();
  } catch (Wt::WServer::Exception& e) {
    std::cerr << e.what() << std::endl;
  } catch (std::exception &e) {
    std::cerr << "exception: " << e.what() << std::endl;
  }
}
