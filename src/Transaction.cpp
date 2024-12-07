#include "Transaction.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "DB.h"

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

void Transaction::setIsReturned(bool returned) {   
    isReturned = returned;   
    if (returned) {  
        returnDate = std::time(nullptr);  
        calculateFine();  
    }  
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

void Transaction::returnBook() {  
    if (!currentUser || currentUser->getIsAdmin()) {  
        std::cout << "Only regular users can return books.\n";  
        return;  
    }  

    // Load transactions dari file  
    DB dbTrans("transactions.txt");  
    std::vector<Transaction> transactions;  
    dbTrans.loadTransactions(transactions);  

    // Load books dari file  
    DB dbBooks("books.txt");  
    std::vector<Book> books;  
    dbBooks.loadBooks(books);  

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

            // Hitung denda  
            trans.calculateFine();  

            // Set status pengembalian  
            trans.setIsReturned(true);  

            // Update buku menjadi tersedia  
            for (Book& book : books) {  
                if (book.getBookId() == trans.getBookId()) {  
                    book.setAvailability(true);  
                    break;  
                }  
            }  

            // Simpan perubahan transaksi  
            dbTrans.saveTransactions(transactions);  

            // Simpan perubahan buku  
            dbBooks.saveBooks(books);  

            std::cout << "Book returned successfully!\n";  
            if (trans.getTotalFine() > 0) {  
                std::cout << "Late return fine: Rp" << trans.getTotalFine() << "\n";  
            }  
            return;  
        }  
    }  
    std::cout << "Transaction not found.\n";  
}

// Di dalam kelas Transaction atau sebagai method global  
void Transaction::viewTransactions() {  
    // Cek apakah user sudah login  
    if (!currentUser) {  
        std::cout << "Please log in first.\n";  
        return;  
    }  

    // Load transactions dari file  
    DB dbTrans("transactions.txt");  
    std::vector<Transaction> transactions;  
    dbTrans.loadTransactions(transactions);  

    // Load books untuk mendapatkan detail buku  
    DB dbBooks("books.txt");  
    std::vector<Book> books;  
    dbBooks.loadBooks(books);  

    // Jika admin, tampilkan semua transaksi  
    if (currentUser->getIsAdmin()) {  
        std::cout << "\n===== ALL TRANSACTIONS =====\n";  
        for (const auto& trans : transactions) {  
            // Cari buku terkait  
            std::string bookTitle = "Unknown";  
            for (const auto& book : books) {  
                if (book.getBookId() == trans.getBookId()) {  
                    bookTitle = book.getTitle();  
                    break;  
                }  
            }  

            std::cout << "Transaction ID: " << trans.getTransactionId()   
                      << " | User ID: " << trans.getUserId()  
                      << " | Book: " << bookTitle  
                      << " | Returned: " << (trans.getIsReturned() ? "Yes" : "No")  
                      << " | Fine: Rp" << trans.getTotalFine() << "\n";  
        }  
    }   
    // Jika user biasa, tampilkan hanya transaksi milik user tersebut  
    else {  
        std::cout << "\n===== YOUR TRANSACTIONS =====\n";  
        bool hasTransactions = false;  
        for (const auto& trans : transactions) {  
            if (trans.getUserId() == currentUser->getUserId()) {  
                // Cari buku terkait  
                std::string bookTitle = "Unknown";  
                for (const auto& book : books) {  
                    if (book.getBookId() == trans.getBookId()) {  
                        bookTitle = book.getTitle();  
                        break;  
                    }  
                }  

                std::cout << "Transaction ID: " << trans.getTransactionId()   
                          << " | Book: " << bookTitle  
                          << " | Returned: " << (trans.getIsReturned() ? "Yes" : "No")  
                          << " | Fine: Rp" << trans.getTotalFine() << "\n";  
                hasTransactions = true;  
            }  
        }  

        if (!hasTransactions) {  
            std::cout << "You have no transaction history.\n";  
        }  
    }  
}  

// Tambahan method untuk mencari transaksi berdasarkan ID  
Transaction* Transaction::findTransactionById(std::vector<Transaction>& transactions, int transactionId) {  
    for (auto& trans : transactions) {  
        if (trans.getTransactionId() == transactionId) {  
            return &trans;  
        }  
    }  
    return nullptr;  
}