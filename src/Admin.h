#ifndef ADMIN_H  
#define ADMIN_H  

#include "User.h"  
#include <string>  

class Admin : public User {  
private:  
    std::string code;  

public:  
    Admin(int id, const std::string& username, const std::string& password,  
          const std::string& name, const std::string& email,   
          const std::string& code);  

    // Metode khusus admin  
    std::string getAdminCode() const;  
     bool getIsAdmin() const override {   
        return true;   
    }  
    void displayAdminInfo() const;  
    
    // Override metode dari User jika diperlukan  
    void displayInfo() const override;  
};  

#endif