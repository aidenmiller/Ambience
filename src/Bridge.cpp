//
//  Bridge.cpp
//
//
//  Created by John Abed on 2017-11-06.
//
//

#include "Bridge.h"

// Constructor
Bridge::Bridge(string name, string ip, string port, string username, WContainerWidget *parent, WelcomeScreen *main):
WContainerWidget(parent)
{
    setContentAlignment(AlignCenter);
    parent_ = main;
    
    name_ = name;
    ip_ = ip;
    port_ = port;
    username_ = username;
}

// Destructor
Bridge::~Bridge() {
    
}

void Bridge::connect() {
    //string url_ = "http://172.30.75.112:80/api/newdeveloper";
    string url_ = "http://" + ip_ + ":" + port_ + "/api/" + username_;
    
    if (!url_.empty()) {
        cout << "\n\nBegin connect to: "  + url_ + "\n\n\n";
        Wt::Http::Client *client = new Wt::Http::Client(this);
        client->setTimeout(15);
        client->setMaximumResponseSize(1000000);
        client->done().connect(boost::bind(&Bridge::handleHttpResponse,
                                           this, client, _1, _2));
        client->get(url_);
    }
    else {
        cout << "no url provided\n";
    }
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
        Json::Object result();
        //Json::parse(response.body(), result);
        cout << response.body() << "\n";
    }
    delete client;
}
