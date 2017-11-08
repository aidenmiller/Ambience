/*
Header for Account.cpp
*/

#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>

using namespace std;

class Account
{
private:
    string firstName_;
    string lastName_;
    string email_;
    string password_;
    bool auth_;

public:
    Account(string fn, string ln, string em, string pw);
    virtual ~Account();

    //Name, email, and password getters/setters
    string getFirstName() {return firstName_;}
    string getLastName() {return lastName_;}
    string getEmail() {return email_;}
    string getPassword() {return password_;}
    bool isAuth() {return auth_;}

    void setFirstName(string fn) {firstName_ = fn;}
    void setLasttName(string ln) {lastName_ = ln;}
    void setEmail(string em) {email_ = em;}
    void setPassword(string pw) {password_ = pw;}
    void setAuth(bool val) {auth_ = val;}
    
};

#endif // ACCOUNT_H
