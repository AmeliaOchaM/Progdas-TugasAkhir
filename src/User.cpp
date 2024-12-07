#include "User.h"
#include <iostream>
#include <vector>
#include "DB.h"
#include "Admin.h"
#include <sstream>


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
std::string User::getPassword() const { return password; }
bool User::getIsAdmin() const { return isAdmin; }

/// Set the database username for the user.
/**
 * This function assigns the given username to the database username field of the user.
 * 
 * @param username The username to be set as the database username.
 */
void User::SetDbUsername(const std::string& username) {
    User::dbUsername = username;
}

std::string User::getDbUsername() const {
    return User::dbUsername;
}

/**
 * Verify that the given password matches the password stored in the database.
 * @return true if the passwords match, false otherwise
 */
bool User::verifyPassword(const std::string& inputPassword) const {  
    // Compare the input password with the stored password  
    return password == inputPassword;  
}  

void User::login() {  
    std::string username, password;  
    std::cout << "\nLogin\n";  
    std::cout << "Enter username: ";  
    std::cin >> username;  
    std::cout << "Enter password: ";  
    std::cin >> password;  

    // Open the user database file  
    DB db("user.txt");  
    std::ifstream inFile(db.fileName);  

    if (!inFile.is_open()) {  
        std::cerr << "Unable to open user database file" << std::endl;  
        return;  
    }  

    std::string line;  
    bool found = false;  
    while (std::getline(inFile, line)) {  
        std::istringstream iss(line);  
        std::string dbUsername, dbPassword, dbName, dbEmail;  
        
        std::getline(iss, dbUsername, ',');  
        std::getline(iss, dbPassword, ',');  
        std::getline(iss, dbName, ',');  
        std::getline(iss, dbEmail, ',');  

        if (dbUsername == username && dbPassword == password) {  
            currentUser = new User(nextUserId++, dbUsername, dbPassword, dbName, dbEmail);  
            std::cout << "Login successful!\n";  
            found = true;  
            break;  
        }  
    }  

    if (!found) {  
        std::cout << "Invalid username or password.\n";  
    }  

    inFile.close();  
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
    DB db("user.txt"); // Provide the necessary arguments
    db.saveUser(*newUser);
    std::cout << "User registered successfully!\n";
}

void User::registerAdmin() {
        std::string username, password, name, email, code;
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
        std::cout << "Enter code: ";
        std::cin >> code;

        Admin* newAdmin = new Admin(nextUserId++, username, password, name, email, code);
        users.push_back(newAdmin);
        DB db("admin.txt"); // Provide the necessary arguments
        db.saveUser(*newAdmin);
        std::cout << "Admin registered successfully!\n";
        if(code != "BukuPustaka01"){
            std::cout << "You are a super admin\n";
        }
    }


