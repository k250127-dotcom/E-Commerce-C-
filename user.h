#ifndef USER_H
#define USER_H

#include <string>

namespace ECommerce {

class User {
protected:
    std::string username;
    std::string password;

public:
    User(const std::string& uname, const std::string& pwd);
    virtual ~User() = default;                     // virtual destructor
    virtual void showMenu() const = 0;             // pure virtual => abstraction
    virtual std::string getRole() const = 0;
    
    // Getters (const)
    std::string getUsername() const { return username; }
    bool authenticate(const std::string& pwd) const;
};

} // namespace ECommerce

#endif