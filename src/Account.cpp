/**
 *  @file       Account.cpp
 *  @author     CS 3307 - Team 13
 *  @date       10/7/2017
 *  @version    1.0
 *
 *  @brief      CS 3307, Hue Light Application Account class to store a user account
 *
 *  @section    DESCRIPTION
 *
 *              This class stores the profile of the user that has registered to the
 *              application. The hashed password is stored and not in plain text. The
 *              Account class manages writing to the serialized account file, editing
 *              user details, and adding and removing Bridge objects that the user
 *              has registered to their account.
 */

#include <string>
#include "Account.h"

/**
 *   @brief  Account constructor
 *
 *   @param  fn is the first name of the user
 *   @param  ln is the last name of the user
 *   @param  em is the email of the user
 *   @param  pw is the hashed password of the user
 *
 */
Account::Account(string fn, string ln, string em, string pw) :
firstName_(fn),
lastName_(ln),
email_(em),
password_(pw),
auth_(false){
    bridges.reserve(15); //reserve initial 15 spaces which dynamically increases once surpassed
}

/**
 *   @brief  Account destructor
 *
 */
Account::~Account() {
    
}


/**
 *   @brief  Write Account object to the user account data
 *
 *   @return void
 *
 */
void Account::writeFile() {
    ofstream writefile;
    string file = "credentials/" + getEmail() +".txt"; // password will be stored in username.txt
    
    writefile.open(file.c_str());
    
    writefile << getPassword() <<endl; // cryptographically hashed password
    
    writefile<< getFirstName() << endl;
    writefile<< getLastName() << endl;
    
    for(auto &bridge : getBridges()) { //loop through all bridges
        writefile << bridge.getUsername() << "-" << bridge.getIP() << "-" << bridge.getPort() << ".txt\n";
    }
    
    writefile.close();
}

/**
 *   @brief  Set first name of user and update the data file
 *
 *   @param  fn the name to change first name to
 *   @return void
 *
 */
void Account::setFirstName(string fn) {
    firstName_ = fn;
    writeFile();
}

/**
 *   @brief  Set last name of user and update the data file
 *
 *   @param  ln the name to change last name to
 *   @return void
 *
 */
void Account::setLastName(string ln) {
    lastName_ = ln;
    writeFile();
}

/**
 *   @brief  Set password of user and update the data file
 *
 *   @param  pw the hashed password to change the password to
 *
 *   @return void
 *
 */
void Account::setPassword(string pw) {
    password_ = pw;
    writeFile();
}

/**
 *   @brief  Logout of user account and clear existing user data
 *
 *   @return void
 *
 */
void Account::logout() {
    firstName_ = "";
    lastName_ = "";
    email_ = "";
    password_ = "";
    auth_ = false;
    bridges.clear();
}
