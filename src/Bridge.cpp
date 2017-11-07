//
//  Bridge.cpp
//
//
//  Created by John Abed on 2017-11-06.
//
//

#include "Bridge.h"

// Constructor
Bridge::Bridge(WContainerWidget *parent, WelcomeScreen *main):
WContainerWidget(parent)
{
    setContentAlignment(AlignCenter);
    parent_ = main;
    
    string url = "http://172.30.75.112:80/api/newdeveloper";
    
    if (!url.empty()) {
        Wt::Http::Client *client = new Wt::Http::Client(this);
        client->setTimeout(15);
        client->setMaximumResponseSize(1000000);
        client->done().connect(boost::bind(&Bridge::handleHttpResponse,
                                           this, client, _1, _2));
        client->get(url);
    }
    
}

// Destructor
Bridge::~Bridge() {
    
    
}

void Bridge::handleHttpResponse(Wt::Http::Client *client,
                                boost::system::error_code err,
                                const Wt::Http::Message& response) const
{
    if (err || response.status() != 200) {
        std::cerr << "Error: " << err.message() << ", " << response.status()
        << std::endl;
    }
    else {
        //Wt::Json::Object result;
        //Wt::Json::parse(response.body(), result);
        cout << response.body() << "\n";
    }
    
    delete client;
}
