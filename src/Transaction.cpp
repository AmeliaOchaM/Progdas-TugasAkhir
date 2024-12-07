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

extern std::vector<Transaction> transactions;
extern User* currentUser;
extern std::vector<Book> books;


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

void Transaction::returnBook() {
        if (!currentUser || currentUser->getIsAdmin()) {
            std::cout << "Only regular users can return books.\n";
            return;
        }

        std::cout << "\nYour Active Rentals:\n";
        bool hasActiveRentals = false;
        for (Transaction& trans : transactions) {
            if (trans.getUserId() == currentUser->getUserId() && !trans.getIsReturned()) {
                hasActiveRentals = true;
                std::cout << "Transaction ID: " << trans.getTransactionId()
                     << ", Book ID: " << trans.getBookId() << "\n";
            }
        }

        if (!hasActiveRentals) {
            std::cout << "You have no active rentals.\n";
            return;
        }

        int transId;
        std::cout << "Enter transaction ID to return: ";
        std::cin >> transId;

        for (Transaction& trans : transactions) {
            if (trans.getTransactionId() == transId) {
                if (trans.getIsReturned()) {
                    std::cout << "This book has already been returned.\n";
                    return;
                }
                trans.returnBook();
                for (Book& book : books) {
                    if (book.getBookId() == trans.getBookId()) {
                        book.setAvailability(true);
                        break;
                    }
                }
                std::cout << "Book returned successfully!\n";
                if (trans.getTotalFine() > 0) {
                    std::cout << "Late return fine: Rp" << trans.getTotalFine() << "\n";
                }
                return;
            }
        }
        std::cout << "Transaction not found.\n";
    }

void Transaction::viewTransactions() {
    std::cout << "\nTransaction History\n";
    bool found = false;
    for (const Transaction& trans : transactions) {
        if (!currentUser->getIsAdmin() && trans.getUserId() != currentUser->getUserId()) {
            continue;
        }
        found = true;
        trans.displayInfo();
        std::cout << "------------------------\n";
    }
    if (!found) {
        std::cout << "No transactions found.\n";
    }
}