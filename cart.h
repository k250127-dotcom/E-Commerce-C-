#ifndef CART_H
#define CART_H

#include <map>
#include "product.h"

namespace ECommerce {

// Forward declaration for friend
class BillGenerator;

class Cart {
private:
    std::map<int, int> items;          // productID -> quantity
    static Cart* instance;             // singleton instance
    Cart() = default;                  // private constructor
    Cart(const Cart&) = delete;
    Cart& operator=(const Cart&) = delete;

public:
    static Cart* getInstance();        // singleton accessor
    ~Cart() = default;

    // Cart operations (with exception handling)
    void addProduct(const Product& p);
    void updateQuantity(int productId, int quantity);
    void removeProduct(int productId);
    void clear();                      // empty the cart
    double getTotal() const;           // calculate total (needs product DB, so we'll implement in .cpp with helper)

    // Accessor for friend class and for GUI display
    const std::map<int, int>& getItems() const { return items; }

    // Operator overloading
    Cart& operator+=(const Product& p);   // add product to cart

    // Friend class – can access private items
    friend class BillGenerator;
};

} // namespace ECommerce

#endif