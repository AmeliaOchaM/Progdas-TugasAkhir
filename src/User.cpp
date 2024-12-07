#include "User.h"
#include <iostream>
#include <vector>
#include "DB.h"
#include "Admin.h"


int nextUserId = 1;
User* User::currentUser = nullptr;
std::vector<User*> User::users;

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
};

void User::login() {
    std::string username, password;
    std::cout << "\nLogin\n";
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    for (User* user : users) {
        if (user->getUsername() == username && user->verifyPassword(password)) {
            currentUser = user;
            std::cout << "Login successful!\n";
            return;
        }
    }
    std::cout << "Invalid username or password.\n";
}

void User::displayInfo() const {
    std::cout << "User ID: " << userId << "\n"
              << "Username: " << username << "\n"
              << "Name: " << name << "\n"
              << "Email: " << email << "\n";
}


void User::registerUser() {
    std::string username, password, name, email;
    std::cout << "\nUser Registration\n";
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;
    std::cout << "Enter name: ";
    std::cin.ignore();
    getline(std::cin, name);
    std::cout << "Enter email: ";
    std::cin >> email;

    User* newUser = new User(nextUserId++, username, password, name, email);
    users.push_back(newUser);
    DB db("database_connection_string"); // Provide the necessary arguments
    db.saveUser(*newUser);
    std::cout << "User registered successfully!\n";
}

void User::registerAdmin() {
        std::string username, password, name, email, role;
        std::cout << "\nAdmin Registration\n";
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;
        std::cout << "Enter name: ";
        std::cin.ignore();
        getline(std::cin, name);
        std::cout << "Enter email: ";
        std::cin >> email;
        std::cout << "Enter role: ";
        std::cin >> role;

        Admin* newAdmin = new Admin(nextUserId++, username, password, name, email, role);
        users.push_back(newAdmin);
        DB db("database_connection_string"); // Provide the necessary arguments
        db.saveUser(*newAdmin);
        std::cout << "Admin registered successfully!\n";
    }


