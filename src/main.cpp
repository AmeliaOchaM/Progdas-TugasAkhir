#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "User.h"
#include "Admin.h"
#include "Book.h"
#include "Transaction.h"
#include "global.h"

using namespace std;

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
            case 4: User::loginAdmin(); break; // Added admin login option
            case 5: if (currentUser) Book::viewBooks(); break;
            case 6: if (currentUser && currentUser->getIsAdmin()) Book::addBook(); break;
            case 7: if (currentUser && !currentUser->getIsAdmin()) Book::rentBook(); break;
            case 8: if (currentUser && !currentUser->getIsAdmin()) Transaction::returnBook(); break;
            case 9: if (currentUser) Transaction::viewTransactions(); break;
            case 0: cout << "Thank you for using Book Rental System!\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);
    
    // Cleanup
    // Cleanup memory allocated for users
    // Iterate through all users and delete them
    for (auto user : users) {
        delete user;
    }
    

    return 0;
}

/**
 * Displays the main menu of the Book Rental System.
 * If no user is logged in, the menu shows options to register as a user or admin, and login options.
 * If a user is logged in, additional options to view books and transactions are displayed.
 * Admins have the option to add books, while regular users can rent and return books.
 * The function also provides an option to exit the system.
 */
void displayMenu() {
    cout << "\n=== Book Rental System ===\n";
    if (!currentUser) {
        cout << "1. Register as User\n"
             << "2. Register as Admin\n"
             << "3. User Login\n"
             << "4. Admin Login\n"; // Added admin login option
    } else {
        cout << "5. View Books\n";
        if (currentUser->getIsAdmin()) {
            cout << "6. Add Book\n";
        } else {
            cout << "7. Rent Book\n"
                 << "8. Return Book\n";
        }
        cout << "9. View Transactions\n";
    }
    cout << "0. Exit\n";
};

