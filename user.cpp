#include "user.h"

namespace ECommerce {

User::User(const std::string& uname, const std::string& pwd)
    : username(uname), password(pwd) {}

bool User::authenticate(const std::string& pwd) const {
    return password == pwd;
}

} // namespace ECommerce