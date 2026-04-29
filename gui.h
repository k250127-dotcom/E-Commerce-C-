#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <memory>
#include <vector>
#include "product.h"

namespace ECommerce {

class AppGUI {
private:
    // Main window
    std::unique_ptr<GtkWidget, decltype(&gtk_widget_destroy)> mainWindow{nullptr, gtk_widget_destroy};
    
    // Containers
    GtkWidget* productList = nullptr;      // GtkListBox
    GtkWidget* statusBar = nullptr;        // GtkLabel for messages
    
    // Data
    std::vector<Product> products;         // product database
    
    // Private methods
    void populateProductList();
    void addToCart(int productIndex);
    void checkout();
    void showMessage(const std::string& msg);
    
    // Static callbacks (need to be static to work with GTK)
    static void onAddToCart(GtkWidget* widget, gpointer data);
    static void onCheckout(GtkWidget* widget, gpointer data);
    static void onSortByPrice(GtkWidget* widget, gpointer data);
    
public:
    AppGUI();
    void run();
};

} // namespace ECommerce

#endif