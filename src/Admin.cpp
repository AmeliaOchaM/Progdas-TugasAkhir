#include "Admin.h"  
#include <iostream>  

Admin::Admin(int id, const std::string& username, const std::string& password,  
             const std::string& name, const std::string& email,   
             const std::string& code)  
    : User(id, username, password, name, email), code(code) {  
    // Secara eksplisit set isAdmin ke true untuk admin  
    isAdmin = true;  
}  

std::string Admin::getAdminCode() const {  
    return code;  
}  

void Admin::displayAdminInfo() const {  
    // Tampilkan informasi admin dengan detail tambahan  
    displayInfo();  // Gunakan metode displayInfo dari User  
    std::cout << "Admin Code: " << code << "\n";  
    std::cout << "Admin Status: Yes\n";  
}  

void Admin::displayInfo() const {  
    // Override metode displayInfo dari User untuk admin  
    User::displayInfo();  // Panggil metode displayInfo dari kelas induk  
    std::cout << "Role: Administrator\n";  
}