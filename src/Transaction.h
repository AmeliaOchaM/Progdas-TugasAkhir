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
    int rentalPrice;
    std::time_t rentDate;
    std::time_t dueDate;
    std::time_t returnDate;
    double totalFine;
    bool isReturned;

public:
    // Original 3-parameter constructor  
    Transaction(int transId, int userId, int bookId);  
    
    // New 6-parameter constructor  
    Transaction(int transId, int userId, int bookId, int rentalPrice, bool isReturned, double totalFine);
    
    // Getters
    int getTransactionId() const;
    int getUserId() const;
    int getBookId() const;
    std::time_t getRentDate() const;
    std::time_t getDueDate() const;
    std::time_t getReturnDate() const;
    double getTotalFine() const;
    bool getIsReturned() const;
    int getRentalPrice() const;
    static double calculateLateFine(int bookId, std::time_t dueDate);
    
    // Methods
    static void returnBook();
    
    void displayInfo() const;
    void setIsReturned(bool returned);
    static Transaction* findTransactionById(std::vector<Transaction>& transactions, int transactionId);
    static void viewTransactions();
    // static void saveTransactions(const std::vector<Transaction>& transactions);
    void setTotalFine(double fine) { totalFine = fine; }  
    void setRentDate(std::time_t date) { rentDate = date; }  
    void setDueDate(std::time_t date) { dueDate = date; } 
    static void updateBookAvailability(int bookId);
    static void updateBooksFile();
    static void updateTransactionsFile();  
    std::string getBookTitle(const std::vector<Book>& books) const;
     


};

#endif
