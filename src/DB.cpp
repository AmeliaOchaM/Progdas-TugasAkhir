#include "DB.h"
#include "Admin.h"
#include "User.h"
#include "global.h"
#include "Transaction.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

void DB::saveUsers(const std::vector<User*>& userList) {  
    std::ofstream outFile(fileName, std::ios::trunc);  // Use trunc to overwrite  
    if (!outFile.is_open()) {  
        std::cerr << "Unable to open file for writing: " << fileName << std::endl;  
        return;  
    }  

    for (const auto& user : userList) {  
        // Check if it's an admin user  
        const Admin* adminUser = dynamic_cast<const Admin*>(user);  
        if (adminUser) {  
            // Save admin with admin code  
            outFile << user->getUserId() << ","  
                    << adminUser->getUsername() << ","  
                    << adminUser->getPassword() << ","  
                    << adminUser->getName() << ","  
                    << adminUser->getEmail() << ","  
                    << adminUser->getAdminCode() << std::endl;  
        } else {  
            // Save regular user  
            outFile << user->getUserId() << ","  
                    << user->getUsername() << ","  
                    << user->getPassword() << ","  
                    << user->getName() << ","  
                    << user->getEmail() << std::endl;  
        }  
    }  

    outFile.close();  
}

void DB::loadUsers(std::vector<User*>& userList) {  
    userList.clear();  
    
    std::ifstream file(fileName);  
    if (!file.is_open()) {  
        std::cout << "Could not open user file: " << fileName << std::endl;  
        return;  
    }  

    std::string line;  
    while (std::getline(file, line)) {  
        std::vector<std::string> fields;  
        std::stringstream ss(line);  
        std::string field;  

        // Split line by comma  
        while (std::getline(ss, field, ',')) {  
            fields.push_back(field);  
        }  

        // Validate number of fields  
        if (fields.size() == 5 || fields.size() == 6) {  
            int userId = std::stoi(fields[0]);  
            
            if (fields.size() == 6) {  
                // This is an admin user  
                Admin* adminUser = new Admin(  
                    userId,   
                    fields[1],    // username  
                    fields[2],    // password  
                    fields[3],    // name  
                    fields[4],    // email  
                    fields[5]     // admin code  
                );  
                userList.push_back(adminUser);  
            } else {  
                // This is a regular user  
                User* user = new User(  
                    userId,   
                    fields[1],    // username  
                    fields[2],    // password  
                    fields[3],    // name  
                    fields[4]     // email  
                );  
                userList.push_back(user);  
            }  
        }  
    }  

    file.close();  

    // Update nextUserId if needed  
    if (!userList.empty()) {  
        nextUserId = userList.back()->getUserId() + 1;  
    }  
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
//     std::cout << "Jumlah buku yang dimuat: " << books.size() << std::endl;  
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
        std::cerr << "Error: Cannot open transactions file: " << fileName << std::endl;  
        return;  
    }  

    std::string line;  
    while (std::getline(inFile, line)) {  
        std::istringstream ss(line);  
        std::string field;  
        std::vector<std::string> fields;  

        while (std::getline(ss, field, ',')) {  
            fields.push_back(field);  
        }  

        // Debug: Print raw transaction line  
        std::cout << "Loading Transaction Line: " << line << std::endl;  

        // Ensure correct number of fields  
        if (fields.size() == 6) {  
            try {  
                Transaction trans(  
                    std::stoi(fields[0]),    // Transaction ID  
                    std::stoi(fields[1]),    // User ID  
                    std::stoi(fields[2]),    // Book ID  
                    std::stoi(fields[3]),    // Rental Price  
                    fields[4] == "1",        // Is Returned  
                    std::stod(fields[5])     // Total Fine  
                );  
                
                transactions.push_back(trans);  
            } catch (const std::exception& e) {  
                std::cerr << "Error parsing transaction: " << line   
                          << ", Error: " << e.what() << std::endl;  
            }  
        } else {  
            std::cerr << "Invalid transaction format: " << line << std::endl;  
        }  
    }  
}

// void DB::loadTransactions(std::vector<Transaction>& transactions) {  
//     std::ifstream inFile(fileName);  
//     if (!inFile.is_open()) {  
//         std::cerr << "Tidak dapat membuka file transaksi: " << fileName << std::endl;  
//         return;  
//     }  

//     transactions.clear();  // Bersihkan transaksi yang ada sebelumnya  
//     std::string line;  
//     while (std::getline(inFile, line)) {  
//         std::istringstream iss(line);  
//         std::string transIdStr, userIdStr, bookIdStr, isReturnedStr, totalFineStr;  
        
//         // Parsing dengan delimiter koma  
//         if (std::getline(iss, transIdStr, ',') &&  
//             std::getline(iss, userIdStr, ',') &&  
//             std::getline(iss, bookIdStr, ',') &&  
//             std::getline(iss, isReturnedStr, ',') &&  
//             std::getline(iss, totalFineStr)) {  
            
//             int transId = std::stoi(transIdStr);  
//             int userId = std::stoi(userIdStr);  
//             int bookId = std::stoi(bookIdStr);  
//             bool isReturned = (isReturnedStr == "1");  
//             double totalFine = std::stod(totalFineStr);  

//             // Buat objek Transaction   
//             Transaction newTransaction(transId, userId, bookId);  
//             newTransaction.setIsReturned(isReturned);  
//             // newTransaction.setTotalFine(totalFine);  
            
//             transactions.push_back(newTransaction);  
//         }  
//     }  

//     inFile.close();  

//     // Debug: Cetak jumlah transaksi yang dimuat  
//     std::cout << "Jumlah transaksi yang dimuat: " << transactions.size() << std::endl;  
// }