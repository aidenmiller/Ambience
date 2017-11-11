/**
 *  @file       Bridge.cpp
 *  @author     CS 3307 - Team 13
 *  @date       10/7/2017
 *  @version    1.0
 *
 *  @brief      CS 3307, Hue Light Application Bridge class to store a Bridge object
 *
 *  @section    DESCRIPTION
 *
 *              This class stores the user-provided information of the Bridge they are
 *              connecting to, if the connection is successful. It allows reading and
 *              writing of data found in the user account file and references it with
 *              the serialized Bridge object to construct a Bridge and add it to the
 *              user account.
 */

#include "Bridge.h"

/**
 *   @brief  Bridge constructor
 *
 *   @param  name is the name of the bridge
 *   @param  location is the location of the bridge
 *   @param  ip is the ip of the bridge URL
 *   @param  port is the port of the bridge URL
 *   @param  username is the username of the bridge URL, default 'newdeveloper'
 *
 */
Bridge::Bridge(string name, string location,
               string ip, string port, string username)
{
    bridgename_ = name;
    ip_ = ip;
    location_ = location;
    port_ = port;
    username_ = username;
}

/**
 *   @brief  Bridge destructor
 *
 */
Bridge::~Bridge() {
    
}

/**
 *   @brief  Write Bridge object to the user account file and to the bridge data file
 *
 *   @param  email the email of the user registering the bridge
 *   @param  data the output from querying the URL to store in the bridge file
 *
 *   @return bool false if either file accessed does not exist, true otherwise
 *
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

/**
 *   @brief  Reads Bridge data from a file and assigns it to this Bridge object
 *
 *   @param  fileName the name of the bridge file to read
 *
 *   @return bool false if file accessed does not exist, true otherwise
 *
 */
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
