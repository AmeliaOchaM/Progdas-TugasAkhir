#include "Transaction.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "DB.h"
#include <algorithm>
#include <ctime>
#include <cmath>


Transaction::Transaction(int transId, int userId, int bookId)  
    : transactionId(transId), userId(userId), bookId(bookId) {  
    rentDate = std::time(nullptr);  
    dueDate = rentDate + (7 * 24 * 60 * 60); // 7 days rental period  
    isReturned = false;  
    totalFine = 0.0;  
    rentalPrice = 0; // Default rental price  
}  

Transaction::Transaction(int transId, int userId, int bookId, int rentalPrice, bool isReturned, double totalFine)  
    : transactionId(transId), userId(userId), bookId(bookId),   
      rentalPrice(rentalPrice), isReturned(isReturned), totalFine(totalFine) {  
    rentDate = std::time(nullptr);  
    dueDate = rentDate + (7 * 24 * 60 * 60); // 7 days rental period  
    
    if (isReturned) {  
        returnDate = std::time(nullptr);  
    }  
}  

/**
 * Getter for transaction ID
 * @return transaction ID
 */
int Transaction::getTransactionId() const { return transactionId; }

/**
 * Getter for user ID
 * @return user ID
 */
int Transaction::getUserId() const { return userId; }

/**
 * Getter for book ID
 * @return book ID
 */
int Transaction::getBookId() const { return bookId; }

/**
 * Getter for rent date
 * @return rent date
 */
std::time_t Transaction::getRentDate() const { return rentDate; }

/**
 * Getter for due date
 * @return due date
 */
std::time_t Transaction::getDueDate() const { return dueDate; }

/**
 * Getter for return date
 * @return return date
 */
std::time_t Transaction::getReturnDate() const { return returnDate; }

/**
 * Getter for total fine
 * @return total fine
 */
double Transaction::getTotalFine() const { return totalFine; }

/**
 * Getter for is returned status
 * @return true if returned, false otherwise
 */
bool Transaction::getIsReturned() const { return isReturned; }

/**
 * Getter for rental price
 * @return rental price
 */

int Transaction::getRentalPrice() const { return rentalPrice; }
extern std::vector<Transaction> transactions;
extern User* currentUser;
extern std::vector<Book> books;

// Di Transaction.cpp  
void Transaction::returnBook() {  
    // Validasi login  
    if (currentUser == nullptr) {  
        std::cout << "Error: No user logged in.\n";  
        return;  
    }  

    // Validasi bukan admin  
    if (currentUser->getIsAdmin()) {  
        std::cout << "Only regular users can return books.\n";  
        return;  
    }  

    // Load transactions from file to ensure latest data  
    DB dbTrans("transactions.txt");  
    std::vector<Transaction> transactions;  
    dbTrans.loadTransactions(transactions);  

    // Tampung transaksi yang perlu diupdate  
    std::vector<Transaction> transactionsToUpdate;  

    // Cari transaksi aktif untuk user saat ini  
    for (auto& trans : transactions) {  
        if (trans.getUserId() == currentUser->getUserId() &&   
            !trans.getIsReturned()) {  
            transactionsToUpdate.push_back(trans);  
        }  
    }  

    // Cek ada transaksi aktif  
    if (transactionsToUpdate.empty()) {  
        std::cout << "You have no active rentals.\n";  
        return;  
    }  
    
    // Tampilkan transaksi aktif  
    std::cout << "\nYour Active Rentals:\n";  
    for (const auto& trans : transactionsToUpdate) {  
        std::cout << "Transaction ID: " << trans.getTransactionId()   
                  << ", Book ID: " << trans.getBookId() << std::endl;  
    }  
    
    // Pilih transaksi untuk dikembalikan  
    int transId;  
    std::cout << "Enter transaction ID to return: ";  
    std::cin >> transId;  

    // Proses pengembalian  
    bool transactionFound = false;  

    for (auto& trans : transactions) {  
        if (trans.getTransactionId() == transId) {  
            // Hitung denda  
            double fine = calculateLateFine(trans.getBookId(), trans.getDueDate());  
            
            // Update status menjadi dikembalikan  
            trans.setIsReturned(true);  
            trans.setTotalFine(fine);  
            
            transactionFound = true;  

            std::cout << "Book returned successfully!\n";  
            if (fine > 0) {  
                std::cout << "Late return fine: Rp" << fine << std::endl;  
            }  
            break;  
        }  
    }  

    // Jika transaksi tidak ditemukan  
    if (!transactionFound) {  
        std::cout << "Transaction not found.\n";  
        return;  
    }  

    // Perbarui file transactions.txt dengan seluruh transaksi terbaru  
    std::ofstream outFile("transactions.txt", std::ios::trunc);  
    
    if (!outFile.is_open()) {  
        std::cerr << "Error: Cannot open transactions file for writing.\n";  
        return;  
    }  

    // Tulis ulang semua transaksi dengan status terbaru   
    for (const auto& trans : transactions) {  
        outFile << trans.getTransactionId() << ","  
                << trans.getUserId() << ","  
                << trans.getBookId() << ","  
                << trans.getRentalPrice() << ","  
                << (trans.getIsReturned() ? "1" : "0") << ","  
                << trans.getTotalFine() << "\n";  
    }  

    outFile.close();  
    std::cout << "Transactions file updated successfully.\n";  

    // Update buku menjadi tersedia  
    updateBookAvailability(transId);  

    // Update status buku di books.txt  
    updateBooksFile();  
}  

// Method untuk menghitung denda  
double Transaction::calculateLateFine(int bookId, std::time_t dueDate) {  
    // Cari buku untuk mendapatkan rental price  
    auto bookIt = std::find_if(books.begin(), books.end(),  
        [bookId](const Book&book) { return book.getBookId() == bookId; });  

    if (bookIt == books.end()) {  
        std::cerr << "Book with ID " << bookId << " not found.\n";  
        return 0.0;  
    }  

    std::time_t now = std::time(nullptr);  

    // Validasi waktu  
    if (now == -1 || dueDate == -1) {  
        std::cerr << "Error in time calculation.\n";  
        return 0.0;  
    }  

    if (now > dueDate) {  
        // Gunakan difftime untuk perhitungan yang aman  
        double secondsDiff = std::difftime(now, dueDate);  

        // Konversi ke hari, bulatkan ke atas  
        int daysLate = static_cast<int>(std::ceil(secondsDiff / (24 * 60 * 60)));  

        return daysLate * (bookIt->getRentalPrice() * 0.5);  
    }  

    return 0.0;  
}  

// Modifikasi method setIsReturned  
void Transaction::setIsReturned(bool returned) {   
    isReturned = returned;   
    if (returned) {  
        returnDate = std::time(nullptr);  
        // Hitung denda menggunakan due date milik objek ini  
        totalFine = calculateLateFine(bookId, dueDate);  
    }  
} 
// Method untuk update file transactions.txt  
void Transaction::updateTransactionsFile() {  
    std::string transactionFile = "transactions.txt";  
    std::ofstream outFile(transactionFile);  
    
    if (!outFile.is_open()) {  
        std::cerr << "Error: Cannot open transactions file for writing.\n";  
        return;  
    }  

    // Tulis ulang semua transaksi  
    for (const auto& trans : transactions) {  
        outFile << trans.getTransactionId() << ","  
                << trans.getUserId() << ","  
                << trans.getBookId() << ","  
                << trans.getRentalPrice() << ","  
                << (trans.getIsReturned() ? "1" : "0") << ","  
                << trans.getTotalFine() << "\n";  
    }  

    outFile.close();  
    std::cout << "Transactions file updated successfully.\n";  
}  

// Method untuk update file books.txt  
void Transaction::updateBooksFile() {  
    std::string bookFile = "books.txt";  
    std::ifstream inFile(bookFile);  
    std::vector<std::string> updatedBooks;  
    std::string line;  

    if (!inFile.is_open()) {  
        std::cerr << "Error: Cannot open books file for reading.\n";  
        return;  
    }  

    while (std::getline(inFile, line)) {  
        std::istringstream ss(line);  
        std::string field;  
        std::vector<std::string> fields;  

        while (std::getline(ss, field, ',')) {  
            fields.push_back(field);  
        }  

        // Ubah status buku menjadi tersedia  
        if (fields.size() >= 6) {  
            fields[5] = "1";  // Kolom availability  
            
            std::string updatedBook =   
                fields[0] + "," +  
                fields[1] + "," +  
                fields[2] + "," +  
                fields[3] + "," +  
                fields[4] + "," +  
                "1";  
            updatedBooks.push_back(updatedBook);  
        }  
    }  
    inFile.close();  

    // Simpan buku yang diupdate  
    std::ofstream outFile(bookFile);  
    for (const auto& book : updatedBooks) {  
        outFile << book << "\n";  
    }  
    outFile.close();  

    std::cout << "Books file updated successfully.\n";  
}  

// Method untuk update ketersediaan buku  
void Transaction::updateBookAvailability(int bookId) {  
    for (auto& book : books) {  
        if (book.getBookId() == bookId) {  
            book.setAvailability(true);  
            break;  
        }  
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
            double rentalPrice = 0.0;  
            for (const auto& book : books) {  
                if (book.getBookId() == trans.getBookId()) {  
                    bookTitle = book.getTitle();  
                    rentalPrice = book.getRentalPrice();  
                    break;  
                }  
            }  

            std::cout << "Transaction ID: " << trans.getTransactionId()   
                      << " | User ID: " << trans.getUserId()  
                      << " | Book: " << bookTitle  
                      << " | Rental Price: Rp" << rentalPrice  
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
                double rentalPrice = 0.0;  
                for (const auto& book : books) {  
                    if (book.getBookId() == trans.getBookId()) {  
                        bookTitle = book.getTitle();  
                        rentalPrice = book.getRentalPrice();  
                        break;  
                    }  
                }  

                std::cout << "Transaction ID: " << trans.getTransactionId()   
                          << " | Book: " << bookTitle  
                          << " | Rental Price: Rp" << rentalPrice  
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