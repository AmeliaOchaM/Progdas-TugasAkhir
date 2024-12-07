#include "Book.h"

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
