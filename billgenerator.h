#ifndef BILLGENERATOR_H
#define BILLGENERATOR_H

#include <string>
#include <vector>

namespace ECommerce {

// Forward declarations
class Cart;
class Product;

class BillGenerator {
public:
    // Generate bill and save to file
    static void generateBill(const Cart& cart,
                             const std::vector<Product>& productDB,
                             const std::string& filename,
                             double taxPercent = 0.0,
                             double discountPercent = 0.0);

    // Calculate total without saving
    static double calculateTotal(const Cart& cart,
                                 const std::vector<Product>& productDB);
};

} // namespace ECommerce

#endif