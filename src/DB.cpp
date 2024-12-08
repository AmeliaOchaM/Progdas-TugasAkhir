#include "DB.h"
#include "Admin.h"
#include "User.h"
#include "global.h"
#include "Transaction.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

void DB::saveUser(const User& user) {  
    std::ofstream outFile(fileName, std::ios::app);  
    if (!outFile.is_open()) {  
        std::cerr << "Unable to open file for writing" << std::endl;  
        return;  
    }  

    // Cek apakah ini admin  
    const Admin* adminUser = dynamic_cast<const Admin*>(&user);  
    if (adminUser) {  
        // Simpan dengan format: username,password,name,email,code  
        outFile << adminUser->getUsername() << ","  
                << adminUser->getPassword() << ","  
                << adminUser->getName() << ","  
                << adminUser->getEmail() << ","  
                << adminUser->getAdminCode() << std::endl;  
    } else {  
        // Simpan user biasa  
        outFile << user.getUsername() << ","  
                << user.getPassword() << ","  
                << user.getName() << ","  
                << user.getEmail() << std::endl;  
    }  

    outFile.close();  
}

void DB::saveBooks(const std::vector<Book>& books) {
    std::ofstream outFile(fileName, std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "Unable to open file for writing" << std::endl;
        return;
    }

    for (const auto& book : books) {
        outFile << book.getBookId() << ","
                << book.getTitle() << ","
                << book.getAuthor() << ","
                << book.getIsbn() << ","
                << book.getRentalPrice() << ","
                << (book.getAvailability() ? "1" : "0") << std::endl;
    }

    outFile.close();
}
void DB::loadBooks(std::vector<Book>& books) {  
    std::ifstream inFile(fileName);  
    if (!inFile.is_open()) {  
        std::cerr << "Tidak dapat membuka file: " << fileName << std::endl;  
        return;  
    }  

    books.clear();  // Bersihkan buku yang ada sebelumnya  
    std::string line;  
    while (std::getline(inFile, line)) {  
        std::istringstream iss(line);  
        std::string idStr, title, author, isbn, priceStr, availabilityStr;  
        
        // Parsing dengan delimiter koma  
        if (std::getline(iss, idStr, ',') &&  
            std::getline(iss, title, ',') &&  
            std::getline(iss, author, ',') &&  
            std::getline(iss, isbn, ',') &&  
            std::getline(iss, priceStr, ',') &&  
            std::getline(iss, availabilityStr)) {  
            
            int id = std::stoi(idStr);  
            double price = std::stod(priceStr);  
            bool availability = (availabilityStr == "1");  

            Book newBook(id, title, author, isbn, price);  
            newBook.setAvailability(availability);  
            books.push_back(newBook);  
        }  
    }  

    inFile.close();  

    // Update nextBookId jika perlu  
    if (!books.empty()) {  
        nextBookId = books.back().getBookId() + 1;  
    }  

    // Debug: Cetak jumlah buku yang dimuat  
    std::cout << "Jumlah buku yang dimuat: " << books.size() << std::endl;  
}

void DB::saveTransactions(const std::vector<Transaction>& transactions) {  
    std::ofstream outFile(fileName, std::ios::app);  
    if (!outFile.is_open()) {  
        std::cerr << "Error: Unable to open transactions file for writing.\n";  
        return;  
    }  

    for (const auto& trans : transactions) {  
        outFile << trans.getTransactionId() << ","  
                << trans.getUserId() << ","  
                << trans.getBookId() << ","  
                << trans.getRentalPrice() << ","
                << (trans.getIsReturned() ? "1" : "0") << ","  
                << trans.getTotalFine() << "\n";  
    }  
    outFile.close();  
} 

void DB::loadTransactions(std::vector<Transaction>& transactions) {  
    std::ifstream inFile(fileName);  
    if (!inFile.is_open()) {  
        std::cerr << "Tidak dapat membuka file transaksi: " << fileName << std::endl;  
        return;  
    }  

    transactions.clear();  // Bersihkan transaksi yang ada sebelumnya  
    std::string line;  
    while (std::getline(inFile, line)) {  
        std::istringstream iss(line);  
        std::string transIdStr, userIdStr, bookIdStr, isReturnedStr, totalFineStr;  
        
        // Parsing dengan delimiter koma  
        if (std::getline(iss, transIdStr, ',') &&  
            std::getline(iss, userIdStr, ',') &&  
            std::getline(iss, bookIdStr, ',') &&  
            std::getline(iss, isReturnedStr, ',') &&  
            std::getline(iss, totalFineStr)) {  
            
            int transId = std::stoi(transIdStr);  
            int userId = std::stoi(userIdStr);  
            int bookId = std::stoi(bookIdStr);  
            bool isReturned = (isReturnedStr == "1");  
            double totalFine = std::stod(totalFineStr);  

            // Buat objek Transaction   
            Transaction newTransaction(transId, userId, bookId);  
            newTransaction.setIsReturned(isReturned);  
            // newTransaction.setTotalFine(totalFine);  
            
            transactions.push_back(newTransaction);  
        }  
    }  

    inFile.close();  

    // Debug: Cetak jumlah transaksi yang dimuat  
    std::cout << "Jumlah transaksi yang dimuat: " << transactions.size() << std::endl;  
}