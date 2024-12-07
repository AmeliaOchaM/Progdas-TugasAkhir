#ifndef GLOBAL_H  
#define GLOBAL_H  

#include <vector>  // Include this for std::vector  
#include <string>  // Include for std::string if needed  

// Forward declarations  
class User;  
class Book;  
class Transaction;  

// Deklarasi global yang dapat diakses di semua file  
extern User* currentUser;  
extern int nextUserId;  
extern int nextBookId;  
extern int nextTransactionId;  

extern std::vector<Book> books;  
extern std::vector<User*> users;  
extern std::vector<Transaction> transactions;  

#endif