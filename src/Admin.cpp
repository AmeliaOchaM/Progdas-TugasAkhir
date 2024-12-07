#include "Admin.h"
#include <iostream>

Admin::Admin(int id, const std::string& username, const std::string& password,
             const std::string& name, const std::string& email, 
             const std::string& role)
    : User(id, username, password, name, email), role(role) {
    isAdmin = true;
}

std::string Admin::getRole() const { return role; }

void Admin::displayInfo() const {
    User::displayInfo();
    std::cout << "Role: " << role << "\n"
              << "Type: Admin\n";
}

