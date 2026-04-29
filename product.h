#include <stdio.h>
#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <string>
#include <utility>

using namespace std;

namespace ECommerce {

class Product {
private:
    int id;
    string name;
    string category;
    double price;
    int stock;

    static int nextId;               // static member

public:
    // Constructors
    Product();

    Product(const string& name, const string& category,
            double price, int stock);

    // Copy & Move
    Product(const Product& other);
    Product(Product&& other) noexcept;
    Product& operator=(const Product& other);
    Product& operator=(Product&& other) noexcept;
    ~Product() = default;

    void serialize(std::ostream& os) const;
void deserialize(std::istream& is);

    // Getters (const correct)
    int getId() const { return id; }
    string getName() const { return name; }
    string getCategory() const { return category; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }

    // Setters
    void setPrice(double newPrice);
    void setStock(int newStock);

    // Static method
    static int getNextId() { return nextId; }

    

    // Operators
    bool operator==(const Product& other) const;
    Product& operator+(int quantity);      // add stock
    friend ostream& operator<<(ostream& os, const Product& p);
};

} // namespace ECommerce

#endif