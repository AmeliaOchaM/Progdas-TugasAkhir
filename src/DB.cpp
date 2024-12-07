#include "DB.h"
#include "Admin.h"
#include "User.h"
#include <iostream>
#include <vector>
#include <fstream>

void DB::saveUser(const User& user) {  
    std::ofstream outFile(fileName, std::ios::app);  
    if (!outFile.is_open()) {  
        std::cerr << "Unable to open file for writing" << std::endl;  
        return;  
    }  

    // Cek apakah ini admin  
    const Admin* adminUser = dynamic_cast<const Admin*>(&user);  
    if (adminUser) {  
        // Simpan dengan format: username,password,name,email,code  
        outFile << adminUser->getUsername() << ","  
                << adminUser->getPassword() << ","  
                << adminUser->getName() << ","  
                << adminUser->getEmail() << ","  
                << adminUser->getAdminCode() << std::endl;  
    } else {  
        // Simpan user biasa  
        outFile << user.getUsername() << ","  
                << user.getPassword() << ","  
                << user.getName() << ","  
                << user.getEmail() << std::endl;  
    }  

    outFile.close();  
}