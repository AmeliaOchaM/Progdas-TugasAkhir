#include "global.h"  
#include "User.h"  
#include "Book.h"  
#include "Transaction.h"  

// Definisi aktual variabel global  
User* currentUser = nullptr;  
int nextUserId = 1;  
int nextBookId = 1;  
int nextTransactionId = 1;  

std::vector<Book> books;  
std::vector<User*> users;  
std::vector<Transaction> transactions;