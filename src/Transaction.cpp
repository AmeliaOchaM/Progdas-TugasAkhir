#include "Transaction.h"
#include <iostream>
#include <iomanip>

Transaction::Transaction(int transId, int userId, int bookId)
    : transactionId(transId), userId(userId), bookId(bookId) {
    rentDate = std::time(nullptr);
    dueDate = rentDate + (7 * 24 * 60 * 60); // 7 days rental period
    isReturned = false;
    totalFine = 0.0;
}

int Transaction::getTransactionId() const { return transactionId; }
int Transaction::getUserId() const { return userId; }
int Transaction::getBookId() const { return bookId; }
std::time_t Transaction::getRentDate() const { return rentDate; }
std::time_t Transaction::getDueDate() const { return dueDate; }
std::time_t Transaction::getReturnDate() const { return returnDate; }
double Transaction::getTotalFine() const { return totalFine; }
bool Transaction::getIsReturned() const { return isReturned; }

void Transaction::returnBook() {
    returnDate = std::time(nullptr);
    isReturned = true;
    calculateFine();
}

void Transaction::calculateFine() {
    if (!isReturned) return;
    
    // Calculate days overdue
    double daysOverdue = std::difftime(returnDate, dueDate) / (24 * 60 * 60);
    if (daysOverdue > 0) {
        totalFine = daysOverdue * 1000; // Rp1000 per day fine
    }
}

void Transaction::displayInfo() const {
    std::cout << "Transaction ID: " << transactionId << "\n"
              << "User ID: " << userId << "\n"
              << "Book ID: " << bookId << "\n"
              << "Rent Date: " << std::ctime(&rentDate)
              << "Due Date: " << std::ctime(&dueDate)
              << "Status: " << (isReturned ? "Returned" : "Active") << "\n";
    
    if (isReturned) {
        std::cout << "Return Date: " << std::ctime(&returnDate)
                  << "Fine: Rp" << std::fixed << std::setprecision(2) << totalFine << "\n";
    }
}
