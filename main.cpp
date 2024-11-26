#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include "BookRental.h"

using namespace std;

BookRentalSystem::BookRentalSystem(const string& filename) 
    : dataFileName(filename), nextBookId(1), nextTransactionId(1) {
    loadData();
}

void BookRentalSystem::addBook(const string& title, const string& author, double price) {
    Book book;
    book.id = nextBookId++;
    book.title = title;
    book.author = author;
    book.rentalPrice = price;
    book.isAvailable = true;
    books.push_back(book);
    saveData();
}

void BookRentalSystem::editBook(int id, const string& title, const string& author, double price) {
    Book* book = findBook(id);
    if (book) {
        book->title = title;
        book->author = author;
        book->rentalPrice = price;
        saveData();
    }
}

void BookRentalSystem::deleteBook(int id) {
    for (auto it = books.begin(); it != books.end(); ++it) {
        if (it->id == id) {
            books.erase(it);
            saveData();
            return;
        }
    }
}

void BookRentalSystem::displayBooks() const {
    cout << "\nBook List:\n";
    cout << "ID\tTitle\tAuthor\tPrice\tStatus\n";
    cout << "----------------------------------------\n";
    for (const auto& book : books) {
        cout << book.id << "\t" 
             << book.title << "\t" 
             << book.author << "\t" 
             << book.rentalPrice << "\t"
             << (book.isAvailable ? "Available" : "Rented") << "\n";
    }
}

Book* BookRentalSystem::findBook(int id) {
    for (auto& book : books) {
        if (book.id == id) {
            return &book;
        }
    }
    return nullptr;
}

void BookRentalSystem::createTransaction(int bookId, const string& customerName, 
                                       const string& rentDate, const string& returnDate) {
    Book* book = findBook(bookId);
    if (book && book->isAvailable) {
        Transaction trans;
        trans.id = nextTransactionId++;
        trans.bookId = bookId;
        trans.customerName = customerName;
        trans.rentDate = rentDate;
        trans.returnDate = returnDate;
        trans.totalPrice = book->rentalPrice;
        
        book->isAvailable = false;
        transactions.push_back(trans);
        saveData();
    }
}

void BookRentalSystem::displayTransactions() const {
    cout << "\nTransaction History:\n";
    cout << "ID\tBook ID\tCustomer\tRent Date\tReturn Date\tPrice\n";
    cout << "--------------------------------------------------------\n";
    for (const auto& trans : transactions) {
        cout << trans.id << "\t"
             << trans.bookId << "\t"
             << trans.customerName << "\t"
             << trans.rentDate << "\t"
             << trans.returnDate << "\t"
             << trans.totalPrice << "\n";
    }
}

bool BookRentalSystem::loadData() {
    ifstream file(dataFileName);
    if (!file) return false;
    
    string line;
    // Read books
    while (getline(file, line)) {
        if (line == "---TRANSACTIONS---") break;
        
        Book book;
        stringstream ss(line);
        ss >> book.id >> book.title >> book.author >> book.rentalPrice >> book.isAvailable;
        books.push_back(book);
        if (book.id >= nextBookId) nextBookId = book.id + 1;
    }
    
    // Read transactions
    while (getline(file, line)) {
        Transaction trans;
        stringstream ss(line);
        ss >> trans.id >> trans.bookId >> trans.customerName 
           >> trans.rentDate >> trans.returnDate >> trans.totalPrice;
        transactions.push_back(trans);
        if (trans.id >= nextTransactionId) nextTransactionId = trans.id + 1;
    }
    
    return true;
}

bool BookRentalSystem::saveData() const {
    ofstream file(dataFileName);
    if (!file) return false;
    
    // Save books
    for (const auto& book : books) {
        file << book.id << " " << book.title << " " << book.author << " "
             << book.rentalPrice << " " << book.isAvailable << "\n";
    }
    
    file << "---TRANSACTIONS---\n";
    
    // Save transactions
    for (const auto& trans : transactions) {
        file << trans.id << " " << trans.bookId << " " << trans.customerName << " "
             << trans.rentDate << " " << trans.returnDate << " " << trans.totalPrice << "\n";
    }
    
    return true;
}

void BookRentalSystem::displayMenu() const {
    cout << "\nBook Rental System Menu:\n";
    cout << "1. Add Book\n";
    cout << "2. Edit Book\n";
    cout << "3. Delete Book\n";
    cout << "4. Display Books\n";
    cout << "5. Create Transaction\n";
    cout << "6. Display Transactions\n";
    cout << "0. Exit\n";
    cout << "Choose an option: ";
}

void BookRentalSystem::run() {
    while (true) {
        displayMenu();
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                string title, author;
                double price;
                cout << "Enter book title: ";
                getline(cin, title);
                cout << "Enter author: ";
                getline(cin, author);
                cout << "Enter rental price: ";
                cin >> price;
                addBook(title, author, price);
                break;
            }
            case 2: {
                int id;
                string title, author;
                double price;
                cout << "Enter book ID to edit: ";
                cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter new title: ";
                getline(cin, title);
                cout << "Enter new author: ";
                getline(cin, author);
                cout << "Enter new rental price: ";
                cin >> price;
                editBook(id, title, author, price);
                break;
            }
            case 3: {
                int id;
                cout << "Enter book ID to delete: ";
                cin >> id;
                deleteBook(id);
                break;
            }
            case 4:
                displayBooks();
                break;
            case 5: {
                int bookId;
                string customerName, rentDate, returnDate;
                cout << "Enter book ID: ";
                cin >> bookId;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter customer name: ";
                getline(cin, customerName);
                cout << "Enter rent date (DD-MM-YYYY): ";
                getline(cin, rentDate);
                cout << "Enter return date (DD-MM-YYYY): ";
                getline(cin, returnDate);
                createTransaction(bookId, customerName, rentDate, returnDate);
                break;
            }
            case 6:
                displayTransactions();
                break;
            case 0:
                return;
            default:
                cout << "Invalid option!\n";
        }
    }
}

int main() {
    BookRentalSystem system("rental_data.txt");
    system.run();
    return 0;
}
