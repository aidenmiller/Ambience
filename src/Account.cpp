#include <string>
#include "Account.h"

//Constructor
Account::Account(string fn, string ln, string em, string pw) :
firstName_(fn),
lastName_(ln),
email_(em),
password_(pw),
auth_(false){
    bridges.reserve(15);
}


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

void Account::setFirstName(string fn) {
        firstName_ = fn;
        writeFile();
    }
void Account::setLastName(string ln) {
        lastName_ = ln;
        writeFile();
    }

void Account::setPassword(string pw) {
    password_ = pw;
    writeFile();
    }

// Destructor
Account::~Account() {

}
