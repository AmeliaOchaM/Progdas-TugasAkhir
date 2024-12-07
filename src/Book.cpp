#include "Book.h"
#include "User.h"
#include "Transaction.h"
#include "global.h"
#include <iostream>
#include <vector>
#include <limits>

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

    books.push_back(Book(nextBookId++, title, author, isbn, price));  
    std::cout << "Book added successfully!\n";  
}
// void Book::addBook() {  
//     // Periksa apakah pengguna saat ini null atau bukan admin  
//     if (!currentUser || !currentUser->getIsAdmin()) {  
//         std::cout << "Access denied. Admin privileges required.\n";  
//         return;  
//     }  

//     std::string title, author, isbn;  
//     double price;  
    
//     std::cout << "\nAdd New Book\n";  
//     std::cout << "Enter title: ";  
//     std::cin.ignore();  
//     std::getline(std::cin, title);  
//     std::cout << "Enter author: ";  
//     std::getline(std::cin, author);  
//     std::cout << "Enter ISBN: ";  
//     std::cin >> isbn;  
//     std::cout << "Enter rental price: ";  
//     std::cin >> price;  

//     books.push_back(Book(nextBookId++, title, author, isbn, price));  
//     std::cout << "Book added successfully!\n";  
// }

void Book::viewBooks() {
    for (const Book& book : books) {
        std::cout << book.getBookId() << "\t"
                  << book.getTitle() << "\t"
                  << book.getAuthor() << "\t"
                  << book.getIsbn() << "\t"
                  << book.getRentalPrice() << "\t"
                  << (book.getAvailability() ? "Yes" : "No") << "\n";
    }
}


void Book::rentBook() {
    if (currentUser->getIsAdmin()) {
        std::cout << "Only regular users can rent books.\n";
        return;
    }

    // Assuming viewBooks function is defined elsewhere
    viewBooks();
    int bookId;
    std::cout << "\nEnter book ID to rent: ";
    std::cin >> bookId;

    for (Book& book : books) {
        if (book.getBookId() == bookId) {
            // Check if book is available for rent
            if (!book.getAvailability()) {
                std::cout << "Book is not available for rent.\n";
                return;
            }
            // Set the availability to false
            book.setAvailability(false);
            // Create a new transaction
            transactions.push_back(Transaction(nextTransactionId++, currentUser->getUserId(), bookId));
            std::cout << "Book rented successfully!\n";
            return;
        }
    }
    std::cout << "Book not found.\n";
}






