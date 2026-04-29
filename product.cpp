#include "product.h"
#include <stdexcept>

namespace ECommerce {

int Product::nextId = 1000;

Product::Product() : id(nextId++), name(""), category(""), price(0.0), stock(0) {}

Product::Product(const std::string& name, const std::string& category,
                 double price, int stock)
    : id(nextId++), name(name), category(category), price(price), stock(stock) {}

Product::Product(const Product& other)
    : id(other.id), name(other.name), category(other.category),
      price(other.price), stock(other.stock) {}

Product::Product(Product&& other) noexcept
    : id(other.id), name(std::move(other.name)),
      category(std::move(other.category)), price(other.price), stock(other.stock) {}

Product& Product::operator=(const Product& other) {
    if (this != &other) {
        id = other.id;
        name = other.name;
        category = other.category;
        price = other.price;
        stock = other.stock;
    }
    return *this;
}

Product& Product::operator=(Product&& other) noexcept {
    if (this != &other) {
        id = other.id;
        name = std::move(other.name);
        category = std::move(other.category);
        price = other.price;
        stock = other.stock;
    }
    return *this;
}

void Product::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&id), sizeof(id));
    size_t len = name.size();
    os.write(reinterpret_cast<const char*>(&len), sizeof(len));
    os.write(name.c_str(), len);
    // ... similarly for category, price, stock
}

void Product::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&id), sizeof(id));
    size_t len;
    is.read(reinterpret_cast<char*>(&len), sizeof(len));
    name.resize(len);
    is.read(&name[0], len);
    // ... etc.
}

void Product::setPrice(double newPrice) {
    if (newPrice < 0) throw std::invalid_argument("Price cannot be negative");
    price = newPrice;
}

void Product::setStock(int newStock) {
    if (newStock < 0) throw std::invalid_argument("Stock cannot be negative");
    stock = newStock;
}

bool Product::operator==(const Product& other) const {
    return id == other.id;
}

Product& Product::operator+(int quantity) {
    if (quantity < 0) throw std::invalid_argument("Cannot add negative stock");
    stock += quantity;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Product& p) {
    os << "[" << p.id << "] " << p.name
       << " | $" << p.price
       << " | Stock: " << p.stock
       << " | Category: " << p.category;
    return os;
}

} // namespace ECommerce