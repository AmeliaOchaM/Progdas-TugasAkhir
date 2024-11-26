#ifndef BOOK_RENTAL_H
#define BOOK_RENTAL_H

#include <string>
#include <vector>

struct Book {
    int id;
    std::string title;
    std::string author;
    bool isAvailable;
    double rentalPrice;
};

struct Transaction {
    int id;
    int bookId;
    std::string customerName;
    std::string rentDate;
    std::string returnDate;
    double totalPrice;
};

class BookRentalSystem {
private:
    std::vector<Book> books;
    std::vector<Transaction> transactions;
    int nextBookId;
    int nextTransactionId;
    std::string dataFileName;

public:
    BookRentalSystem(const std::string& filename);
    
    // Book management
    void addBook(const std::string& title, const std::string& author, double price);
    void editBook(int id, const std::string& title, const std::string& author, double price);
    void deleteBook(int id);
    void displayBooks() const;
    Book* findBook(int id);
    
    // Transaction management
    void createTransaction(int bookId, const std::string& customerName, 
                         const std::string& rentDate, const std::string& returnDate);
    void displayTransactions() const;
    
    // File operations
    bool loadData();
    bool saveData() const;
    
    // Navigation
    void displayMenu() const;
    void run();
};

#endif
