/*
Header for Account.cpp
*/

#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>

class Account
{
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string password;
    bool isAuth;

public:
    Account(std::string firstName, std::string lastName, std::string email, std::string password);
    virtual ~Account();

    //Name, email, and password getters/setters
    std::string getFirstName() {return firstName;}
    std::string getLastName() {return lastName;}
    std::string getEmail() {return email;}
    std::string getPassword() {return password;}

    std::string setFirstName(std::string first) {firstName = first;}
    std::string setLasttName(std::string last) {lastName = last;}
    std::string setEmail(std::string e) {email = e;}
    std::string setPassword(std::string pw) {password = pw;}
};

#endif // ACCOUNT_H
