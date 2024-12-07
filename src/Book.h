#ifndef BOOK_H
#define BOOK_H

#include <string>


class Book {
private:
    int bookId;
    std::string title;
    std::string author;
    std::string isbn;
    bool isAvailable;
    double rentalPrice;

public:
    Book(int id, const std::string& title, const std::string& author, 
         const std::string& isbn, double price);
    
    // Getters
    int getBookId() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getIsbn() const;
    bool getAvailability() const;
    double getRentalPrice() const;

    // Setters
    void setAvailability(bool status);
    void setRentalPrice(double price);

    static void addBook();
    static void rentBook();
    static void viewBooks();

};

#endif
