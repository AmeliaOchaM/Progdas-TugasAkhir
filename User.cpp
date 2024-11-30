#include "User.h"
#include <iostream>

User::User(int id, const std::string& username, const std::string& password,
           const std::string& name, const std::string& email)
    : userId(id), username(username), password(password), 
      name(name), email(email), isAdmin(false) {}

int User::getUserId() const { return userId; }
std::string User::getUsername() const { return username; }
std::string User::getName() const { return name; }
std::string User::getEmail() const { return email; }
bool User::getIsAdmin() const { return isAdmin; }

bool User::verifyPassword(const std::string& inputPassword) const {
    return password == inputPassword; // In real application, use proper hashing
}

void User::displayInfo() const {
    std::cout << "User ID: " << userId << "\n"
              << "Username: " << username << "\n"
              << "Name: " << name << "\n"
              << "Email: " << email << "\n";
}
