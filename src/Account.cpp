#include <string>
#include "Account.h"

//Constructor
Account::Account(std::string fn, std::string ln, std::string em, std::string pw) :
firstName(fn),
lastName(ln),
email(em),
password(pw){

}

// Destructor
Account::~Account() {

}
