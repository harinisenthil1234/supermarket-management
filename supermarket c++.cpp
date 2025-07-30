// Supermarket Billing System with Improvements
// Features: Input Validation, Quantity Management, Admin Login, Product File, Receipts

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
#include <ctime>
#include <algorithm>
#include <sstream> 

using namespace std;

struct Product {
    int code;
    string name;
    float price;
    float discount;
    int quantity;
};

vector<Product> loadProducts() {
    ifstream file("database.txt");
    vector<Product> products;
    Product p;
    while (file >> p.code >> p.name >> p.price >> p.discount >> p.quantity) {
        products.push_back(p);
    }
    return products;
}

void saveProducts(const vector<Product>& products) {
    ofstream file("database.txt");
    for (auto& p : products) {
        file << p.code << " " << p.name << " " << p.price << " " << p.discount << " " << p.quantity << "\n";
    }
}

void listProducts() {
    vector<Product> products = loadProducts();
    cout << left << setw(10) << "Code" << setw(15) << "Name" << setw(10) << "Price"
         << setw(10) << "Discount" << setw(10) << "Qty" << endl;
    for (auto& p : products) {
        cout << left << setw(10) << p.code << setw(15) << p.name << setw(10) << p.price
             << setw(10) << p.discount << setw(10) << p.quantity << endl;
    }
}

bool isCodeUnique(int code, const vector<Product>& products) {
    for (auto& p : products) {
        if (p.code == code) return false;
    }
    return true;
}

void addProduct() {
    vector<Product> products = loadProducts();
    Product p;
    cout << "\nEnter product code: ";
    cin >> p.code;
    if (!cin || !isCodeUnique(p.code, products)) {
        cout << "Invalid or duplicate code!\n";
        return;
    }
    cout << "Enter product name: ";
    cin >> p.name;
    cout << "Enter price: ";
    cin >> p.price;
    cout << "Enter discount (%): ";
    cin >> p.discount;
    cout << "Enter quantity: ";
    cin >> p.quantity;

    if (cin.fail() || p.price < 0 || p.discount < 0 || p.quantity < 0) {
        cout << "Invalid input.\n";
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    products.push_back(p);
    saveProducts(products);
    cout << "Product added successfully.\n";
}

void buyProduct() {
    vector<Product> products = loadProducts();
    listProducts();

    int code, qty;
    cout << "\nEnter product code: ";
    cin >> code;
    cout << "Enter quantity: ";
    cin >> qty;

    if (cin.fail() || qty <= 0) {
        cout << "Invalid input.\n";
        return;
    }

    bool found = false;
    float total = 0;
    stringstream receipt;
    receipt << "Receipt\n";
    receipt << left << setw(15) << "Product" << setw(10) << "Price" << setw(10) << "Qty"
            << setw(10) << "Total" << endl;

    for (auto& p : products) {
        if (p.code == code) {
            found = true;
            if (p.quantity < qty) {
                cout << "Not enough stock.\n";
                return;
            }
            float price = p.price - (p.price * p.discount / 100);
            float lineTotal = price * qty;
            receipt << setw(15) << p.name << setw(10) << price << setw(10) << qty << setw(10) << lineTotal << endl;
            total += lineTotal;
            p.quantity -= qty;
            break;
        }
    }

    if (!found) {
        cout << "Product not found.\n";
        return;
    }

    receipt << "-------------------------------\nTotal: Rs. " << total << "\n";
    cout << receipt.str();

    // Save updated product stock
    saveProducts(products);

    // Save to file
    time_t now = time(0);
    tm* ltm = localtime(&now);
    string filename = "receipt_" + to_string(ltm->tm_mday) + "_" + to_string(ltm->tm_mon + 1) + "_" + to_string(1900 + ltm->tm_year) + ".txt";
    ofstream out(filename);
    out << receipt.str();
    cout << "Receipt saved as " << filename << endl;
}

bool adminLogin() {
    string email, pass;
    cout << "\nAdmin Login\nEmail: "; cin >> email;
    cout << "Password: "; cin >> pass;
    return (email == "admin@email.com" && pass == "admin123");
}

void adminMenu() {
    int choice;
    do {
        cout << "\nAdministrator Menu\n1. Add Product\n2. List Products\n3. Logout\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1: addProduct(); break;
            case 2: listProducts(); break;
            case 3: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 3);
}

void buyerMenu() {
    int choice;
    do {
        cout << "\nBuyer Menu\n1. Buy Product\n2. Back to Main Menu\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1: buyProduct(); break;
            case 2: cout << "Returning to main menu...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 2);
}

int main() {
    int choice;
    do {
        cout << "\nSupermarket Billing System\n1. Administrator\n2. Buyer\n3. Exit\nSelect: ";
        cin >> choice;
        switch (choice) {
            case 1:
                if (adminLogin()) adminMenu();
                else cout << "Invalid credentials.\n";
                break;
            case 2: buyerMenu(); break;
            case 3: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 3);
    return 0;
}

