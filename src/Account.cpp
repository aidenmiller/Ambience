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

// Destructor
Account::~Account() {

}
