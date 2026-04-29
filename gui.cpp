#include "gui.h"
#include "cart.h"
#include "filemanager.h"
#include "billgenerator.h"
#include <iostream>
#include <algorithm>

namespace ECommerce {

// Helper function to pack a button with a label
static GtkWidget* createButton(const char* label, GCallback callback, gpointer data) {
    GtkWidget* btn = gtk_button_new_with_label(label);
    g_signal_connect(btn, "clicked", callback, data);
    return btn;
}

// Constructor
AppGUI::AppGUI() {
    // Create main window
    mainWindow.reset(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    gtk_window_set_title(GTK_WINDOW(mainWindow.get()), "E-Commerce Simulator");
    gtk_window_set_default_size(GTK_WINDOW(mainWindow.get()), 600, 400);
    g_signal_connect(mainWindow.get(), "destroy", G_CALLBACK(gtk_main_quit), nullptr);

    // Create a vertical box as main container
    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(mainWindow.get()), vbox);

    // Title label
    GtkWidget* title = gtk_label_new(nullptr);
    gtk_label_set_markup(GTK_LABEL(title), "<big><b>Product Catalog</b></big>");
    gtk_box_pack_start(GTK_BOX(vbox), title, FALSE, FALSE, 5);

    // Scrolled window for product list
    GtkWidget* scrolled = gtk_scrolled_window_new(nullptr, nullptr);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled, TRUE, TRUE, 5);

    // Product list (GtkListBox)
    productList = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scrolled), productList);

    // Status bar label
    statusBar = gtk_label_new("Ready");
    gtk_box_pack_start(GTK_BOX(vbox), statusBar, FALSE, FALSE, 5);

    // Button box
    GtkWidget* buttonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_spacing(GTK_BOX(buttonBox), 10);
    gtk_box_pack_start(GTK_BOX(vbox), buttonBox, FALSE, FALSE, 5);

    // Buttons
    GtkWidget* sortBtn = createButton("Sort by Price", G_CALLBACK(onSortByPrice), this);
    GtkWidget* checkoutBtn = createButton("Checkout", G_CALLBACK(onCheckout), this);
    
    gtk_container_add(GTK_CONTAINER(buttonBox), sortBtn);
    gtk_container_add(GTK_CONTAINER(buttonBox), checkoutBtn);

    // In AppGUI constructor, replace the dummy products with:
try {
    products = FileManager<Product>::loadFromFile("products.dat");
    if (products.empty()) throw std::runtime_error("empty");
} catch(...) {
    // fallback dummy products
    products.emplace_back("Laptop", "Electronics", 999.99, 10);
    products.emplace_back("Mouse", "Electronics", 19.99, 50);
    products.emplace_back("Keyboard", "Electronics", 49.99, 30);
    products.emplace_back("Monitor", "Electronics", 199.99, 15);
    products.emplace_back("USB Cable", "Accessories", 5.99, 100);
}

    // Populate the list
    populateProductList();
}

void AppGUI::populateProductList() {
    // Clear existing rows
    GtkListBoxRow* row;
    while ((row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(productList), 0)) != nullptr) {
        gtk_container_remove(GTK_CONTAINER(productList), GTK_WIDGET(row));
    }

    // Add each product with an "Add to Cart" button
    for (size_t i = 0; i < products.size(); ++i) {
        const Product& p = products[i];
        
        // Horizontal box for product info + button
        GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        
        // Product label
        char buf[256];
        snprintf(buf, sizeof(buf), "%s - $%.2f (Stock: %d)", 
                 p.getName().c_str(), p.getPrice(), p.getStock());
        GtkWidget* label = gtk_label_new(buf);
        gtk_widget_set_halign(label, GTK_ALIGN_START);
        
        // Add to cart button
        GtkWidget* addBtn = gtk_button_new_with_label("Add to Cart");
        // Store the product index in the button's data
        g_object_set_data(G_OBJECT(addBtn), "product-index", GUINT_TO_POINTER(i));
        g_signal_connect(addBtn, "clicked", G_CALLBACK(onAddToCart), this);
        
        gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(hbox), addBtn, FALSE, FALSE, 0);
        
        gtk_list_box_insert(GTK_LIST_BOX(productList), hbox, -1);
    }
    
    gtk_widget_show_all(productList);
}

void AppGUI::addToCart(int productIndex) {
    if (productIndex < 0 || productIndex >= (int)products.size()) return;
    
    try {
        const Product& p = products[productIndex];
        Cart::getInstance()->addProduct(p);
        showMessage("Added " + p.getName() + " to cart");
    } catch (const std::exception& e) {
        showMessage(std::string("Error: ") + e.what());
    }
}

void AppGUI::checkout() {
    Cart* cart = Cart::getInstance();
    if (cart->getItems().empty()) {
        showMessage("Cart is empty. Add products first.");
        return;
    }
    
    try {
        // Generate bill in current directory
        BillGenerator::generateBill(*cart, products, "invoice.txt", 10.0, 0.0);
        showMessage("Bill saved to invoice.txt");
        
        // Optionally clear cart after checkout
        cart->clear();
    } catch (const std::exception& e) {
        showMessage(std::string("Checkout error: ") + e.what());
    }
}

void AppGUI::showMessage(const std::string& msg) {
    gtk_label_set_text(GTK_LABEL(statusBar), msg.c_str());
    // Also print to console for debugging
    std::cout << msg << std::endl;
}

// Static callbacks
void AppGUI::onAddToCart(GtkWidget* widget, gpointer data) {
    AppGUI* gui = static_cast<AppGUI*>(data);
    guintptr idx = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(widget), "product-index"));
    gui->addToCart(static_cast<int>(idx));
}

void AppGUI::onCheckout(GtkWidget* widget, gpointer data) {
    AppGUI* gui = static_cast<AppGUI*>(data);
    gui->checkout();
}

void AppGUI::onSortByPrice(GtkWidget* widget, gpointer data) {
    AppGUI* gui = static_cast<AppGUI*>(data);
    
    // Lambda for sorting
    std::sort(gui->products.begin(), gui->products.end(),
              [](const Product& a, const Product& b) {
                  return a.getPrice() < b.getPrice();
              });
    gui->populateProductList();  // refresh display
    gui->showMessage("Sorted by price (low to high)");
}

void AppGUI::run() {
    gtk_widget_show_all(mainWindow.get());
    gtk_main();
}

} // namespace ECommerce