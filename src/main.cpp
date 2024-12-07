#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "User.h"
#include "Admin.h"
#include "Book.h"
#include "Transaction.h"

using namespace std;

// Global storage (in a real application, this would be in a database)
vector<User*> users;
vector<Book> books;
vector<Transaction> transactions;
User* currentUser = nullptr;
int nextUserId = 1;
int nextBookId = 1;
int nextTransactionId = 1;

// Function declarations
void displayMenu();
void registerUser();
void registerAdmin();
void login();
void addBook();
void viewBooks();
void rentBook();
void returnBook();
void viewTransactions();

void login() {
    string username, password;
    cout << "\nLogin\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    for (User* user : users) {
        if (user->getUsername() == username && user->verifyPassword(password)) {
            currentUser = user;
            cout << "Login successful!\n";
            return;
        }
    }
    cout << "Invalid username or password.\n";
}

void addBook() {
    if (!currentUser || !currentUser->getIsAdmin()) {
        cout << "Access denied. Admin privileges required.\n";
        return;
    }

    string title, author, isbn;
    double price;
    
    cout << "\nAdd New Book\n";
    cout << "Enter title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter author: ";
    getline(cin, author);
    cout << "Enter ISBN: ";
    cin >> isbn;
    cout << "Enter rental price: ";
    cin >> price;

    books.push_back(Book(nextBookId++, title, author, isbn, price));
    cout << "Book added successfully!\n";
}

void viewBooks() {
    cout << "\nBook List\n";
    cout << "ID\tTitle\tAuthor\tISBN\tPrice\tAvailable\n";
    for (const Book& book : books) {
        cout << book.getBookId() << "\t"
             << book.getTitle() << "\t"
             << book.getAuthor() << "\t"
             << book.getIsbn() << "\t"
             << book.getRentalPrice() << "\t"
             << (book.getAvailability() ? "Yes" : "No") << "\n";
    }
}

void rentBook() {
    if (!currentUser || currentUser->getIsAdmin()) {
        cout << "Only regular users can rent books.\n";
        return;
    }

    viewBooks();
    int bookId;
    cout << "\nEnter book ID to rent: ";
    cin >> bookId;

    for (Book& book : books) {
        if (book.getBookId() == bookId) {
            if (!book.getAvailability()) {
                cout << "Book is not available for rent.\n";
                return;
            }
            book.setAvailability(false);
            transactions.push_back(Transaction(nextTransactionId++, currentUser->getUserId(), bookId));
            cout << "Book rented successfully!\n";
            return;
        }
    }
    cout << "Book not found.\n";
}

void returnBook() {
    if (!currentUser || currentUser->getIsAdmin()) {
        cout << "Only regular users can return books.\n";
        return;
    }

    cout << "\nYour Active Rentals:\n";
    bool hasActiveRentals = false;
    for (Transaction& trans : transactions) {
        if (trans.getUserId() == currentUser->getUserId() && !trans.getIsReturned()) {
            hasActiveRentals = true;
            cout << "Transaction ID: " << trans.getTransactionId()
                 << ", Book ID: " << trans.getBookId() << "\n";
        }
    }

    if (!hasActiveRentals) {
        cout << "You have no active rentals.\n";
        return;
    }

    int transId;
    cout << "Enter transaction ID to return: ";
    cin >> transId;

    for (Transaction& trans : transactions) {
        if (trans.getTransactionId() == transId) {
            if (trans.getIsReturned()) {
                cout << "This book has already been returned.\n";
                return;
            }
            trans.returnBook();
            for (Book& book : books) {
                if (book.getBookId() == trans.getBookId()) {
                    book.setAvailability(true);
                    break;
                }
            }
            cout << "Book returned successfully!\n";
            if (trans.getTotalFine() > 0) {
                cout << "Late return fine: Rp" << trans.getTotalFine() << "\n";
            }
            return;
        }
    }
    cout << "Transaction not found.\n";
}

void viewTransactions() {
    cout << "\nTransaction History\n";
    bool found = false;
    for (const Transaction& trans : transactions) {
        if (!currentUser->getIsAdmin() && trans.getUserId() != currentUser->getUserId()) {
            continue;
        }
        found = true;
        trans.displayInfo();
        cout << "------------------------\n";
    }
    if (!found) {
        cout << "No transactions found.\n";
    }
}

class DB {
public:
    std::ifstream in;
    std::ofstream out;
    std::string fileName;

    DB(const char* fileName) : fileName(fileName) {}

    void save(const User& data) {
        out.open(fileName, std::ios::app);
        out << data.getUserId() << "," << data.getUsername() << ","  << "," << data.getName() << "," << data.getEmail() << "," << (data.getIsAdmin() ? "Admin" : "User") << "\n";
        out.close();
    }
};

int main() {
    cout << "Welcome to Book Rental System\n";
    
    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: registerUser(); break;
            case 2: registerAdmin(); break;
            case 3: login(); break;
            case 4: if (currentUser) viewBooks(); break;
            case 5: if (currentUser && currentUser->getIsAdmin()) addBook(); break;
            case 6: if (currentUser && !currentUser->getIsAdmin()) rentBook(); break;
            case 7: if (currentUser && !currentUser->getIsAdmin()) returnBook(); break;
            case 8: if (currentUser) viewTransactions(); break;
            case 0: cout << "Thank you for using Book Rental System!\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);
    
    // Cleanup
    for (auto user : users) {
        delete user;
    }
    
    return 0;
}

void displayMenu() {
    cout << "\n=== Book Rental System ===\n";
    if (!currentUser) {
        cout << "1. Register as User\n"
             << "2. Register as Admin\n"
             << "3. Login\n";
    } else {
        cout << "4. View Books\n";
        if (currentUser->getIsAdmin()) {
            cout << "5. Add Book\n";
        } else {
            cout << "6. Rent Book\n"
                 << "7. Return Book\n";
        }
        cout << "8. View Transactions\n";
    }
    cout << "0. Exit\n";
}
class Database {
public:
    static void saveUser(const User& user) {
        ofstream out("users.txt", ios::app);
        if (out.is_open()) {
            out << user.getUserId() << ","
                << user.getUsername() << ","
                << user.getName() << ","
                << user.getEmail() << ","
                << (user.getIsAdmin() ? "Admin" : "User") << "\n";
            out.close();
        } else {
            cerr << "Unable to open file for writing.\n";
        }
    }
};

void registerUser() {
    string username, password, name, email;
    cout << "\nUser Registration\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    cout << "Enter name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter email: ";
    cin >> email;

    User* newUser = new User(nextUserId++, username, password, name, email);
    users.push_back(newUser);
    Database::saveUser(*newUser);
    cout << "User registered successfully!\n";
}

void registerAdmin() {
    string username, password, name, email, role;
    cout << "\nAdmin Registration\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    cout << "Enter name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter email: ";
    cin >> email;
    cout << "Enter role: ";
    cin >> role;

    Admin* newAdmin = new Admin(nextUserId++, username, password, name, email, role);
    users.push_back(newAdmin);
    Database::saveUser(*newAdmin);
    cout << "Admin registered successfully!\n";
}