#include "billgenerator.h"
#include "cart.h"
#include "product.h"
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <stdexcept>

namespace ECommerce {

// Helper: get current date/time as string
static std::string currentDateTime() {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void BillGenerator::generateBill(const Cart& cart,
                                 const std::vector<Product>& productDB,
                                 const std::string& filename,
                                 double taxPercent,
                                 double discountPercent) {

    if (cart.getItems().empty()) {
        throw std::runtime_error("Cannot generate bill: cart is empty");
    }

    std::ofstream billFile(filename);
    if (!billFile) {
        throw std::runtime_error("Cannot create bill file: " + filename);
    }

    // Header
    billFile << "========================================\n";
    billFile << "         E-COMMERCE INVOICE            \n";
    billFile << "========================================\n";
    billFile << "Date: " << currentDateTime() << "\n";
    billFile << "----------------------------------------\n";
    billFile << std::left << std::setw(20) << "Product"
             << std::setw(10) << "Qty"
             << std::setw(12) << "Unit Price"
             << std::setw(12) << "Total" << "\n";
    billFile << "----------------------------------------\n";

    double subtotal = 0.0;

    // Iterate over cart items
    for (const auto& pair : cart.getItems()) {
        int productId = pair.first;
        int quantity = pair.second;

        // Find product in database
        auto it = std::find_if(productDB.begin(), productDB.end(),
                               [productId](const Product& p) {
                                   return p.getId() == productId;
                               });
        if (it == productDB.end()) {
            throw std::runtime_error("Product ID " + std::to_string(productId) + " not found in database");
        }

        double unitPrice = it->getPrice();
        double lineTotal = unitPrice * quantity;
        subtotal += lineTotal;

        billFile << std::left << std::setw(20) << it->getName()
                 << std::setw(10) << quantity
                 << std::setw(12) << std::fixed << std::setprecision(2) << unitPrice
                 << std::setw(12) << lineTotal << "\n";
    }

    // Totals and taxes
    double taxAmount = subtotal * (taxPercent / 100.0);
    double discountAmount = subtotal * (discountPercent / 100.0);
    double grandTotal = subtotal + taxAmount - discountAmount;

    billFile << "----------------------------------------\n";
    billFile << std::right << std::setw(42) << "Subtotal: $" << std::setw(10) << std::fixed << std::setprecision(2) << subtotal << "\n";
    if (taxPercent != 0.0)
        billFile << std::setw(42) << "Tax (" << taxPercent << "%): $" << std::setw(10) << taxAmount << "\n";
    if (discountPercent != 0.0)
        billFile << std::setw(42) << "Discount (" << discountPercent << "%): -$" << std::setw(10) << discountAmount << "\n";
    billFile << "========================================\n";
    billFile << std::setw(42) << "GRAND TOTAL: $" << std::setw(10) << grandTotal << "\n";
    billFile << "========================================\n";
    billFile << "          Thank you for shopping!       \n";
    billFile << "========================================\n";

    billFile.close();
}

double BillGenerator::calculateTotal(const Cart& cart,
                                     const std::vector<Product>& productDB) {
    double total = 0.0;
    for (const auto& pair : cart.getItems()) {
        int productId = pair.first;
        int quantity = pair.second;
        auto it = std::find_if(productDB.begin(), productDB.end(),
                               [productId](const Product& p) {
                                   return p.getId() == productId;
                               });
        if (it != productDB.end()) {
            total += it->getPrice() * quantity;
        }
    }
    return total;
}

} // namespace ECommerce