#include "User.h"
#include <iostream>
#include <vector>
#include "DB.h"
#include "Admin.h"
#include <sstream>
#include "global.h"  
#include "User.h"  

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



void User::SetDbUsername(const std::string& username) {
    User::dbUsername = username;
}

std::string User::getDbUsername() const {
    return User::dbUsername;
}

bool User::verifyPassword(const std::string& inputPassword) const {  
    // Compare the input password with the stored password  
    return password == inputPassword;  
}  
void User::displayUserInfo() const {  
    if (currentUser) {  
        std::cout << "\n===== User Information =====\n";  
        std::cout << "User ID: " << currentUser->getUserId() << "\n";  
        std::cout << "Username: " << currentUser->getUsername() << "\n";  
        std::cout << "Name: " << currentUser->getName() << "\n";  
        std::cout << "Email: " << currentUser->getEmail() << "\n";  
    } else {  
        std::cout << "No user is currently logged in.\n";  
    }  
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
        login(); // recursive login  
    }  

    inFile.close(); 
    // Setelah login berhasil  
    if (currentUser) {  
        currentUser->displayUserInfo();  
    }  
}



void User::loginAdmin() {  
    DB db("admin.txt");  
    std::ifstream inFile(db.fileName);  

    if (!inFile.is_open()) {  
        std::cerr << "FATAL: Tidak bisa membuka file admin.txt" << std::endl;  
        return;  
    }  

    // Reset posisi file  
    inFile.clear();  
    inFile.seekg(0);  

    std::string username, password;  
    std::cout << "\nAdmin Login\n";  
    std::cout << "Enter username: ";  
    std::cin >> username;  
    std::cout << "Enter password: ";  
    std::cin >> password;  


    if (!inFile.is_open()) {  
        std::cerr << "Unable to open admin database file" << std::endl;  
        return;  
    }  

    std::string line;  
    bool found = false;  
    while (std::getline(inFile, line)) {  
        std::istringstream iss(line);  
        std::string dbUsername, dbPassword, dbName, dbEmail, dbCode;  
        
        std::getline(iss, dbUsername, ',');  
        std::getline(iss, dbPassword, ',');  
        std::getline(iss, dbName, ',');  
        std::getline(iss, dbEmail, ',');  
        std::getline(iss, dbCode);  // Hapus parameter terakhir  

        if (dbUsername == username && dbPassword == password) {  
            // Buat objek Admin secara dinamis  
            currentUser = new Admin(  
                nextUserId++,     // ID unik  
                dbUsername,       // username  
                dbPassword,       // password  
                dbName,           // nama  
                dbEmail,          // email  
                dbCode            // kode admin  
            );  
            
            // Debug print  
            std::cout << "Admin Login Successful!\n";  
            
            found = true;  
            break;  
        }  
    }  

    if (!found) {  
        std::cout << "Invalid admin username or password.\n";  
        currentUser = nullptr;  // Pastikan currentUser diset ke nullptr jika login gagal  
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
    
    // Check if username already exists  
    std::ifstream checkFile("user.txt");  
    std::string line;  
    while (std::getline(checkFile, line)) {  
        std::istringstream ss(line);  
        std::string existingUsername;  
        std::getline(ss, existingUsername, ',');  
        
        if (existingUsername == username) {  
            std::cout << "Username already exists. Please choose another.\n";  
            return;  
        }  
    }  
    checkFile.close();  

    std::cout << "Enter password: ";  
    std::cin >> password;  
    std::cout << "Enter name: ";  
    std::cin.ignore();  
    getline(std::cin, name);  
    std::cout << "Enter email: ";  
    std::cin >> email;  

    // Determine next user ID  
    int newUserId = 1;  
    std::ifstream idFile("user.txt");  
    while (std::getline(idFile, line)) {  
        newUserId++;  
    }  
    idFile.close();  

    // Append to user.txt  
    std::ofstream outFile("user.txt", std::ios::app);  
    outFile << username << ","  
            << password << ","  
            << name << ","  
            << email << std::endl;  
    outFile.close();  

    std::cout << "User registered successfully with ID: " << newUserId << "\n";  
} 

void User::registerAdmin() {  
    std::string username, password, name, email, code;  
    
    std::cout << "\nAdmin Registration\n";  
    std::cout << "Enter username: ";  
    std::cin >> username;  
    
    // Check if username already exists  
    std::ifstream checkFile("admin.txt");  
    std::string line;  
    while (std::getline(checkFile, line)) {  
        std::istringstream ss(line);  
        std::string existingUsername;  
        std::getline(ss, existingUsername, ',');  
        
        if (existingUsername == username) {  
            std::cout << "Username already exists. Please choose another.\n";  
            return;  
        }  
    }  
    checkFile.close();  

    std::cout << "Enter password: ";  
    std::cin >> password;  
    std::cout << "Enter name: ";  
    std::cin.ignore();  
    getline(std::cin, name);  
    std::cout << "Enter email: ";  
    std::cin >> email;  
    std::cout << "Enter code: ";  
    std::cin >> code;  

    if (code == "BookXtra97") {  
        // Determine next admin ID  
        int newAdminId = 1;  
        std::ifstream idFile("admin.txt");  
        while (std::getline(idFile, line)) {  
            newAdminId++;  
        }  
        idFile.close();  

        // Append to admin.txt  
        std::ofstream outFile("admin.txt", std::ios::app);  
        outFile << username << ","  
                << password << ","  
                << name << ","  
                << email << ","  
                << code << std::endl;  
        outFile.close();  
        
        std::cout << "Admin registered successfully with ID: " << newAdminId << "\n";  
    } else {  
        std::cout << "Invalid code. Please contact the administrator.\n";  
    }  
}  