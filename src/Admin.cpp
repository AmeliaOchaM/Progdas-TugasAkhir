#include "Admin.h"
#include <iostream>

Admin::Admin(int id, const std::string& username, const std::string& password,
             const std::string& name, const std::string& email, 
             const std::string& role)
    : User(id, username, password, name, email), code(code) {
    isAdmin = true;
}

std::string Admin::getCode() const { return code; }

void Admin::displayInfo() const {
    User::displayInfo();
    std::cout << "Code: " << code << "\n"
              << "Type: " << (code == "BukuPustaka01" ? "true" : "false") << "\n";
}

