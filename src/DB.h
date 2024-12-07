#ifndef DB_H
#define DB_H
#include <fstream>
#include <string>
#include "User.h"
#include "Book.h"
#include "iostream"

class Transaction;
class DB {
public:
    std::ifstream in;
    std::ofstream out;
    std::string fileName;

    DB(const char* fileName){
    DB::fileName = fileName;}
    void saveUser(const User& data);
    void saveBooks(const std::vector<Book>& books);  
    void loadBooks(std::vector<Book>& books);
    void saveTransactions(const std::vector<Transaction>& transactions);
    void loadTransactions(std::vector<Transaction>& transactions);
};

#endif
