#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

class Admin : public User {
private:
    std::string role;

public:
    Admin(int id, const std::string& username, const std::string& password,
          const std::string& name, const std::string& email, 
          const std::string& role);
    
    std::string getRole() const;
    void displayInfo() const override;
};

#endif
