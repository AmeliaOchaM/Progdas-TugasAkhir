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

public:
    User(int id, const std::string& username, const std::string& password,
         const std::string& name, const std::string& email);
    
    // Getters
    int getUserId() const;
    std::string getUsername() const;
    std::string getName() const;
    std::string getEmail() const;
    bool getIsAdmin() const;
    
    // Methods
    bool verifyPassword(const std::string& inputPassword) const;
    virtual void displayInfo() const;
};

#endif
