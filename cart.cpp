#include "cart.h"
#include <stdexcept>

namespace ECommerce {

Cart* Cart::instance = nullptr;

Cart* Cart::getInstance() {
    if (!instance) {
        instance = new Cart();
    }
    return instance;
}

void Cart::addProduct(const Product& p) {
    if (p.getStock() <= 0) {
        throw std::runtime_error("Product out of stock");
    }
    items[p.getId()]++;
}

void Cart::updateQuantity(int productId, int quantity) {
    if (quantity <= 0) {
        removeProduct(productId);
    } else {
        auto it = items.find(productId);
        if (it != items.end()) {
            it->second = quantity;
        } else {
            throw std::out_of_range("Product not in cart");
        }
    }
}

void Cart::removeProduct(int productId) {
    items.erase(productId);
}

void Cart::clear() {
    items.clear();
}

double Cart::getTotal() const {
    // This is a stub – real implementation requires product database.
    // We'll compute total inside BillGenerator instead.
    return 0.0;
}

Cart& Cart::operator+=(const Product& p) {
    addProduct(p);
    return *this;
}

} // namespace ECommerce