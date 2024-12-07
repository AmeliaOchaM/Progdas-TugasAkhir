#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>
#include "Book.h"
#include "User.h"

class Transaction {
private:
    int transactionId;
    int userId;
    int bookId;
    std::time_t rentDate;
    std::time_t dueDate;
    std::time_t returnDate;
    double totalFine;
    bool isReturned;

public:
    Transaction(int transId, int userId, int bookId);
    
    // Getters
    int getTransactionId() const;
    int getUserId() const;
    int getBookId() const;
    std::time_t getRentDate() const;
    std::time_t getDueDate() const;
    std::time_t getReturnDate() const;
    double getTotalFine() const;
    bool getIsReturned() const;
    
    // Methods
    static void returnBook();
    void calculateFine();
    void displayInfo() const;
    void setIsReturned(bool returned);
    static Transaction* findTransactionById(std::vector<Transaction>& transactions, int transactionId);
    static void viewTransactions();

};

#endif
