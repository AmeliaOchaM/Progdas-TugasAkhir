#include <iostream>
#include <string>

using namespace std;

class Car {
public:
    string brand;
    string model;
    int year;
    double pricePerDay;

    Car(string b, string m, int y, double p) : brand(b), model(m), year(y), pricePerDay(p) {}

    void displayInfo() {
        cout << "Brand: " << brand << ", Model: " << model << ", Year: " << year << ", Price per day: $" << pricePerDay << endl;
    }
};

class Rental {
public:
    Car car;
    int rentalDays;

    Rental(Car c, int days) : car(c), rentalDays(days) {}

    double calculateTotalCost() {
        return rentalDays * car.pricePerDay;
    }

    void displayRentalInfo() {
        car.displayInfo();
        cout << "Rental days: " << rentalDays << ", Total cost: $" << calculateTotalCost() << endl;
    }
};

int main() {
    Car car1("Toyota", "Camry", 2020, 50.0);
    Rental rental1(car1, 5);

    rental1.displayRentalInfo();

    return 0;
}