#include "admin.h"
#include <iostream>

namespace ECommerce {

Admin::Admin(const std::string& uname, const std::string& pwd)
    : User(uname, pwd) {}

void Admin::showMenu() const {
    std::cout << "Admin Menu:\n1. Add Product\n2. Edit Product\n3. Delete Product\n";
}

} // namespace ECommerce