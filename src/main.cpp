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
vector<Book> books;
vector<User*> users;
vector<Transaction> transactions;
int nextBookId = 1;
int nextTransactionId = 1;


void displayMenu();


int main() {
    cout << "Welcome to Book Rental System\n";
    
    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: User::registerUser(); break;
            case 2: User::registerAdmin(); break;
            case 3: User::login(); break;
            case 4: if (User::currentUser) Book::viewBooks(); break;
            case 5: if (User::currentUser && User::currentUser->getIsAdmin()) Book::addBook(); break;
            case 6: if (User::currentUser && !User::currentUser->getIsAdmin()) Book::rentBook(); break;
            case 7: if (User::currentUser && !User::currentUser->getIsAdmin()) Transaction::returnBook(); break;
            case 8: if (User::currentUser) Transaction::viewTransactions(); break;
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
    if (!User::currentUser) {
        cout << "1. Register as User\n"
             << "2. Register as Admin\n"
             << "3. Login\n";
    } else {
        cout << "4. View Books\n";
        if (User::currentUser->getIsAdmin()) {
            cout << "5. Add Book\n";
        } else {
            cout << "6. Rent Book\n"
                 << "7. Return Book\n";
        }
        cout << "8. View Transactions\n";
    }
    cout << "0. Exit\n";
};
