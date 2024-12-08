#include "Transaction.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "DB.h"
#include <algorithm>

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
int Transaction::getRentalPrice() const { return rentalPrice; }

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

// Di Transaction.cpp  
void Transaction::returnBook() {  
    // Debug: Cetak status login  
    std::cout << "Debug: Current User Pointer: " << currentUser << std::endl;  
    
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

    // Debug: Cetak user ID  
    int currentUserId = currentUser->getUserId();  
    std::cout << "Debug: Current User ID: " << currentUserId << std::endl;  

    // Load transactions  
    std::string transactionFile = "transactions.txt";  
    std::ifstream inFile(transactionFile);  
    
    if (!inFile.is_open()) {  
        std::cout << "Error: Cannot open transactions file.\n";  
        return;  
    }  

    // Tampung transaksi aktif  
    std::vector<std::string> activeTransactions;  
    std::string line;  
    bool hasActiveRentals = false;  

    std::cout << "\nYour Active Rentals:\n";  
    while (std::getline(inFile, line)) {  
        std::istringstream ss(line);  
        std::string field;  
        std::vector<std::string> fields;  

        // Parsing fields  
        while (std::getline(ss, field, ',')) {  
            fields.push_back(field);  
        }  

        // Cek transaksi aktif user  
        if (fields.size() >= 4 &&   
            std::stoi(fields[1]) == currentUserId &&   
            fields[3] == "0") {  
            hasActiveRentals = true;  
            std::cout << "Transaction ID: " << fields[0]   
                      << ", Book ID: " << fields[2] << "\n";  
            activeTransactions.push_back(line);  
        }  
    }  
    inFile.close();  

    // Cek ada transaksi aktif  
    if (!hasActiveRentals) {  
        std::cout << "You have no active rentals.\n";  
        return;  
    }  

    // Pilih transaksi untuk dikembalikan  
    int transId;  
    std::cout << "Enter transaction ID to return: ";  
    std::cin >> transId;  

    // Proses pengembalian  
    std::vector<std::string> updatedTransactions;  
    bool transactionFound = false;  

    for (const auto& transLine : activeTransactions) {  
        std::istringstream ss(transLine);  
        std::string field;  
        std::vector<std::string> fields;  

        while (std::getline(ss, field, ',')) {  
            fields.push_back(field);  
        }  

        if (std::stoi(fields[0]) == transId) {  
            // Update status menjadi dikembalikan (1)  
            fields[3] = "1";  
            
            // Hitung denda (contoh sederhana)  
            double fine = 0.0;  
            // Logika denda bisa ditambahkan di sini  
            
            // Gabungkan kembali fields  
            std::string updatedTrans =   
                fields[0] + "," +   
                fields[1] + "," +   
                fields[2] + "," +   
                "1," +   
                std::to_string(fine);  
            
            updatedTransactions.push_back(updatedTrans);  
            transactionFound = true;  

            std::cout << "Book returned successfully!\n";  
            if (fine > 0) {  
                std::cout << "Late return fine: Rp" << fine << "\n";  
            }  
        } else {  
            updatedTransactions.push_back(transLine);  
        }  
    }  

    // Jika transaksi tidak ditemukan  
    if (!transactionFound) {  
        std::cout << "Transaction not found.\n";  
        return;  
    }  

    // Simpan transaksi yang diupdate  
    std::ofstream outFile(transactionFile);  
    if (!outFile.is_open()) {  
        std::cout << "Error: Cannot update transactions file.\n";  
        return;  
    }  

    for (const auto& trans : updatedTransactions) {  
        outFile << trans << "\n";  
    }  
    outFile.close();  

    // Update ketersediaan buku (opsional, bisa dibuat method terpisah)  
    std::string bookFile = "books.txt";  
    std::ifstream bookInFile(bookFile);  
    std::vector<std::string> updatedBooks;  

    if (bookInFile.is_open()) {  
        while (std::getline(bookInFile, line)) {  
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
        bookInFile.close();  

        // Simpan buku yang diupdate  
        std::ofstream bookOutFile(bookFile);  
        for (const auto& book : updatedBooks) {  
            bookOutFile << book << "\n";  
        }  
        bookOutFile.close();  
    }  
}  

// Pastikan method calculateFine() di-implement  
void Transaction::calculateFine() {  
    // Misalnya, denda Rp5000 per hari keterlambatan  
    std::time_t now = std::time(nullptr);  
    
    if (now > dueDate) {  
        // Hitung selisih hari  
        int daysLate = (now - dueDate) / (24 * 60 * 60);  
        totalFine = daysLate * 5000.0;  
    } else {  
        totalFine = 0.0;  
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
            for (const auto& book : books) {  
                if (book.getBookId() == trans.getBookId()) {  
                    bookTitle = book.getTitle();  
                    break;  
                }  
            }  

            std::cout << "Transaction ID: " << trans.getTransactionId()   
                      << " | User ID: " << trans.getUserId()  
                      << " | Book: " << bookTitle  
                      << " | Rental Price: Rp" << trans.getRentalPrice()
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
                          << " | Rental Price : Rp" << trans.getRentalPrice()
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