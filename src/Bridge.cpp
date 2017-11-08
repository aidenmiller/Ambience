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

//GETTER METHODS
string Bridge::getName(){
    return (name_);
}

string Bridge::getIP(){
    return (ip_);
}

string Bridge::getPort(){
    return (port_);
}

string Bridge::getUsername(){
    return (username_);
}


//SETTER METHODS
void Bridge::setName(string bName){
    name_ = bName;
}

void Bridge::setIP(string bIP){
    ip_= bIP;
}

void Bridge::setPort(string bPort){
    port_ = bPort;
}

void Bridge::setUsername(string bUsername){
    username_ = bUsername;
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
                                           this, _1, _2));
        if(client->get(url_)) WApplication::instance()->deferRendering();
    }
}

void Bridge::handleHttpResponse(boost::system::error_code err,
                                const Wt::Http::Message &response)
{
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        cout << response.body() << "\n";
        
        this->writeBridge(response.body());
    }
}

void Bridge::writeBridge(string data){
    /* WRITE INDIVIDUAL BRIDGE TO FILE */
    const int dir_err = system("mkdir -p bridges");
    if (-1 == dir_err)
    {
        cout << "ERROR - Could not create directory\n";
        exit(1);
    }
    
    ofstream writefile;
    string file = "bridges/" + username_ + "-" +
    ip_ + "-" + port_ +".txt";
    
    writefile.open(file.c_str());
    writefile << data;
    
    writefile.close();
    
    /* WRITE BRIDGE REFERENCE TO USER ACCOUNT */
    //string filename = "credentials/" + parent_->getAccount().getEmail() + ".txt";
    string filename = "credentials/a@b.com.txt";

    //open the credentials file to append the bridge textfile name to it
    writefile.open(filename.c_str(), ios::out | ios::app);
    writefile << "\n" << username_ + "-" + ip_ + "-" + port_ +".txt";
    writefile.close();
    
}
