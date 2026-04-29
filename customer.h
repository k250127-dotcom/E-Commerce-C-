#ifndef CUSTOMER_H
#define CUSTOMER_H
using namespace std;

#include "user.h"

namespace ECommerce {

class Customer : public User {
public:
    Customer(const string& uname, const string& pwd);
    void showMenu() const override;          // override keyword
    string getRole() const override { return "Customer"; }
};

} // namespace ECommerce

#endif