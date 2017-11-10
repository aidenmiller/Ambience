/*
Header for Account.cpp
*/

#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
#include "Bridge.h"
#include <vector>

using namespace std;

class Account
{
private:
    string firstName_;
    string lastName_;
    string email_;
    string password_;
    bool auth_;
    vector<Bridge> bridges;

public:
    Account(string fn, string ln, string em, string pw);
    virtual ~Account();

    //Name, email, and password getters/setters
    string getFirstName() {return firstName_;}
    string getLastName() {return lastName_;}
    string getEmail() {return email_;}
    string getPassword() {return password_;}
    bool isAuth() {return auth_;}
    vector<Bridge> getBridges() {return bridges;}

    void setFirstName(string fn);
    void setLastName(string ln);
    void setEmail(string em) {email_ = em;}
    void setPassword(string pw);
    void setAuth(bool val) {auth_ = val;}
    void addBridge(Bridge br) {bridges.push_back(br);}
    void writeFile();

};

#endif // ACCOUNT_H
