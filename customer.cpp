#include "customer.h"
#include <iostream>
using namespace std;

namespace ECommerce {

Customer::Customer(const string& uname, const string& pwd)
    : User(uname, pwd) {}

void Customer::showMenu() const {
    cout << "Customer Menu:\n1. View Products\n2. Add to Cart\n3. Checkout\n";
}

} // namespace ECommerce