//
//  Bridge.cpp
//
//
//  Created by John Abed on 2017-11-06.
//
//

#include "Bridge.h"

// Constructor
Bridge::Bridge(string name, string location,
               string ip, string port, string username)
{
    bridgename_ = name;
    ip_ = ip;
    location_ = location;
    port_ = port;
    username_ = username;
}

// Destructor
Bridge::~Bridge() {
    
}

/*
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
        if(client->get(url_))
            WApplication::instance()->deferRendering();
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
*/

bool Bridge::writeBridge(string email, string data) {
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
    if (!writefile)
        return false; //error writing to file
    cout << "\n***WRITING BRIDGE***\n";
    writefile << bridgename_ << "\n";
    writefile << location_ << "\n";
    writefile << ip_ << "\n";
    writefile << port_ << "\n";
    writefile << username_ << "\n";
    cout << bridgename_ << "\n";
    cout << location_ << "\n";
    cout << ip_ << "\n";
    cout << port_ << "\n";
    cout << username_ << "\n";
    writefile << data;
    cout << "***END WRITING BRIDGE***\n\n";
    
    writefile.close();
    
    /* WRITE BRIDGE REFERENCE TO USER ACCOUNT */
    //string filename = "credentials/" + parent_->getAccount().getEmail() + ".txt";
    string filename = "credentials/" + email + ".txt";
    
    //open the credentials file to append the bridge textfile name to it
    writefile.open(filename.c_str(), ios::out | ios::app);
    if (!writefile)
        return false; //error writing to file
    writefile << username_ + "-" + ip_ + "-" + port_ +".txt\n";
    writefile.close();
    
    return true;
}

bool Bridge::readBridge(string fileName) {
    ifstream inFile;
    string str;
    string filename = "bridges/" + fileName;
    
    inFile.open(filename.c_str());
    if (!inFile)
        return false ; // file not found
    
    cout << "\n***READING BRIDGE***\n";
    getline(inFile, str);
    bridgename_ = str;
    getline(inFile, str);
    location_ = str;
    getline(inFile, str);
    ip_ = str;
    getline(inFile, str);
    port_ = str;
    getline(inFile, str);
    username_ = str;
    cout << bridgename_ << "\n";
    cout << location_ << "\n";
    cout << ip_ << "\n";
    cout << port_ << "\n";
    cout << username_ << "\n";
    while (getline(inFile, str))
    {
        cout << str << "\n";
    }
    cout << "***END READING BRIDGE***\n\n";
    inFile.close();
    return true;
}
