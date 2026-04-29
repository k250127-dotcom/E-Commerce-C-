
#ifndef ADMIN_H
#define ADMIN_H

#include "user.h"

namespace ECommerce {

class Admin : public User {
public:
    Admin(const std::string& uname, const std::string& pwd);
    void showMenu() const override;
    std::string getRole() const override { return "Admin"; }
};

} // namespace ECommerce

#endif