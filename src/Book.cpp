#include "Book.h"
#include "DB.h"
#include "User.h"
#include "Transaction.h"
#include "global.h"
#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>

Book::Book(int id, const std::string& title, const std::string& author, 
           const std::string& isbn, double price) 
    : bookId(id), title(title), author(author), isbn(isbn), 
      isAvailable(true), rentalPrice(price) {}

int Book::getBookId() const { return bookId; }
std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
std::string Book::getIsbn() const { return isbn; }
bool Book::getAvailability() const { return isAvailable; }
double Book::getRentalPrice() const { return rentalPrice; }

void Book::setAvailability(bool status) { isAvailable = status; }
void Book::setRentalPrice(double price) { rentalPrice = price; }

extern std::vector<Book> books;
extern int nextBookId;
extern std::vector<Transaction> transactions;
extern int nextTransactionId;

void Book::addBook() {  
    // Pastikan currentUser tidak null  
    if (currentUser == nullptr) {  
        std::cout << "FATAL ERROR: No user is logged in.\n";  
        return;  
    }  
    
    // Periksa status admin  
    if (!currentUser->getIsAdmin()) {  
        std::cout << "Access denied. Admin privileges required.\n";  
        return;  
    }  

    // Load existing books first to get the correct next book ID  
    DB db("books.txt");  
    std::vector<Book> existingBooks;  
    db.loadBooks(existingBooks);  

    // Determine the next book ID based on the last book in the database  
    int newBookId = existingBooks.empty() ? 1 :   
        existingBooks.back().getBookId() + 1;  

    std::string title, author, isbn;  
    double price;  
    
    std::cout << "\nAdd New Book\n";  
    std::cout << "Enter title: ";  
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  
    std::getline(std::cin, title);  
    std::cout << "Enter author: ";  
    std::getline(std::cin, author);  
    std::cout << "Enter ISBN: ";  
    std::getline(std::cin, isbn);  
    std::cout << "Enter rental price: ";  
    std::cin >> price;  

    // Validasi input  
    if (title.empty() || author.empty() || isbn.empty() || price <= 0) {  
        std::cout << "Invalid input. Book not added.\n";  
        return;  
    }  

    // Create new book with the correct ID  
    Book newBook(newBookId, title, author, isbn, price);  
    
    // Append new book to existing books  
    existingBooks.push_back(newBook);  
    
    // Save updated book list  
    db.saveBooks(existingBooks);  

    // Update global books vector  
    books = existingBooks;  

    // Update global nextBookId  
    nextBookId = newBookId + 1;  

    std::cout << "Book added successfully with ID: " << newBookId << "\n";  
}

void Book::viewBooks() {  
      
    // Muat buku dari file sebelum menampilkan  
    DB db("books.txt");  
    db.loadBooks(books);  

    // Header  
    std::cout << "\n" << std::left   
              << std::setw(5) << "ID"   
              << std::setw(20) << "Title"   
              << std::setw(20) << "Author"   
              << std::setw(15) << "ISBN"   
              << std::setw(10) << "Price"   
              << "Available\n";  
    std::cout << std::string(70, '-') << "\n";  

    // Iterate and format  
    for (const Book& book : books) {  
        std::cout << std::left   
                  << std::setw(5) << book.getBookId()   
                  << std::setw(20) << (book.getTitle().length() > 18 ? book.getTitle().substr(0, 18) + "..." : book.getTitle())  
                  << std::setw(20) << (book.getAuthor().length() > 18 ? book.getAuthor().substr(0, 18) + "..." : book.getAuthor())  
                  << std::setw(15) << book.getIsbn()   
                  << std::setw(10) << book.getRentalPrice()   
                  << (book.getAvailability() ? "Yes" : "No") << "\n";  
    }  

    std::cout << std::string(70, '-') << "\n";  
    std::cout << "Total Books: " << books.size() << "\n";  
}

void Book::rentBook() {  
    // Pastikan user sudah login  
    if (currentUser == nullptr) {  
        std::cout << "Please log in first.\n";  
        return;  
    }  

    if (currentUser->getIsAdmin()) {  
        std::cout << "Only regular users can rent books.\n";  
        return;  
    }  

    // Debug: Cetak user ID yang sedang login  
    std::cout << "Current User ID: " << currentUser->getUserId() << std::endl;  

    // Muat buku dari file terlebih dahulu  
    DB db("books.txt");  
    db.loadBooks(books);  

    // Muat transaksi untuk mendapatkan ID terakhir  
    DB dbTrans("transactions.txt");  
    std::vector<Transaction> existingTransactions;  
    dbTrans.loadTransactions(existingTransactions);  

    // Update nextTransactionId jika perlu  
    if (!existingTransactions.empty()) {  
        // Cari ID transaksi terbesar  
        int maxTransactionId = 0;  
        for (const auto& trans : existingTransactions) {  
            maxTransactionId = std::max(maxTransactionId, trans.getTransactionId());  
        }  
        nextTransactionId = maxTransactionId + 1;  
    }  

    viewBooks();  
    int bookId;  
    std::cout << "\nEnter book ID to rent: ";  
    std::cin >> bookId;  

    for (Book& book : books) {  
        if (book.getBookId() == bookId) {  
            // Periksa ketersediaan buku  
            if (!book.getAvailability()) {  
                std::cout << "Book is not available for rent.\n";  
                return;  
            }  
            
            // Set ketersediaan buku menjadi false  
            book.setAvailability(false);  
            
            // Debug: Cetak informasi sebelum membuat transaksi  
            std::cout << "Creating Transaction:" << std::endl;  
            std::cout << "Transaction ID: " << nextTransactionId << std::endl;  
            std::cout << "User ID: " << currentUser->getUserId() << std::endl;  
            std::cout << "Book ID: " << bookId << std::endl;  
            std::cout << "Rental Price: " << book.getRentalPrice() << std::endl;  

            // Buat transaksi baru dengan ID yang bertambah dan rental price sesuai dengan book  
            Transaction newTransaction(  
                nextTransactionId,   
                currentUser->getUserId(),  // Pastikan menggunakan user ID yang benar  
                bookId,   
                book.getRentalPrice(),   
                false,   
                0.0  
            );  

            // Tambahkan transaksi baru ke dalam vektor  
            transactions.push_back(newTransaction);  
            
            // Increment ID untuk transaksi berikutnya  
            nextTransactionId++;  
            
            // Update ketersediaan buku di database  
            db.saveBooks(books);   
            
            // Simpan transaksi ke database  
            dbTrans.saveTransactions(transactions);   
            
            std::cout << "Book rented successfully!\n";  
            return;  
        }  
    }  
    std::cout << "Book not found.\n";  
}