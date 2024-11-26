#include <iostream>
#include "user.h"

using namespace std;

struct Book {
    string title;
    string author;
    int publishYear;
    };



int User::count = 1;

int main() {
    User user;

    user.name = "John Doe";
    user.ID = User::count++;
    user.address = "123 Main St";
    user.email = "john.doe@example.com";

    cout << "Name: " << user.name << endl;
    cout << "ID: " << User::count << endl;
    cout << "Address: " << user.address << endl;
    cout << "Email: " << user.email << endl;
    return 0;
}
