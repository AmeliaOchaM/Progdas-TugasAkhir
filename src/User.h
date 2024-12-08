#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

class User {
protected:
    int userId;
    std::string username;
    std::string password;
    std::string name;
    std::string email;
    bool isAdmin;
    std::string dbUsername;
    std::string dbPassword;
    std::string dbName;
    std::string dbEmail;

public:
    User(int id, const std::string& username, const std::string& password,
         const std::string& name, const std::string& email);
    
    
    static std::vector<User*> users;
    
    // Getters
    int getUserId() const;
    std::string getUsername() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getPassword() const;
    virtual bool getIsAdmin() const {   
        return isAdmin;   
    } 
    
    // Methods
    bool verifyPassword(const std::string& inputPassword) const;
    virtual void displayInfo() const;
    static void login();
    static void loginAdmin();
    static void registerUser();
    static void registerAdmin();
    void SetDbUsername(const std::string& username);
    std::string getDbUsername() const;
    void displayUserInfo() const;
};

#endif
