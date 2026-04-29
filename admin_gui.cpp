#include "admin_gui.h"
#include "filemanager.h"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace ECommerce {

AdminGUI::AdminGUI() {
    mainWindow.reset(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    gtk_window_set_title(GTK_WINDOW(mainWindow.get()), "Admin Panel - Product Management");
    gtk_window_set_default_size(GTK_WINDOW(mainWindow.get()), 600, 400);
    g_signal_connect(mainWindow.get(), "destroy", G_CALLBACK(gtk_main_quit), nullptr);

    // Load products from file
    try {
        products = FileManager<Product>::loadFromFile("products.dat");
    } catch(...) {
        // If no file, create dummy products
        products.emplace_back("Laptop", "Electronics", 999.99, 10);
        products.emplace_back("Mouse", "Electronics", 19.99, 50);
        products.emplace_back("Keyboard", "Electronics", 49.99, 30);
        products.emplace_back("Monitor", "Electronics", 199.99, 15);
        products.emplace_back("USB Cable", "Accessories", 5.99, 100);
    }

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(mainWindow.get()), vbox);

    GtkWidget* title = gtk_label_new(nullptr);
    gtk_label_set_markup(GTK_LABEL(title), "<big><b>Admin: Manage Products</b></big>");
    gtk_box_pack_start(GTK_BOX(vbox), title, FALSE, FALSE, 5);

    GtkWidget* scrolled = gtk_scrolled_window_new(nullptr, nullptr);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled, TRUE, TRUE, 5);

    productList = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scrolled), productList);

    statusBar = gtk_label_new("Ready");
    gtk_box_pack_start(GTK_BOX(vbox), statusBar, FALSE, FALSE, 5);

    GtkWidget* buttonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_spacing(GTK_BOX(buttonBox), 10);
    gtk_box_pack_start(GTK_BOX(vbox), buttonBox, FALSE, FALSE, 5);

    GtkWidget* addBtn = gtk_button_new_with_label("Add Product");
    g_signal_connect(addBtn, "clicked", G_CALLBACK(onAddProduct), this);
    gtk_container_add(GTK_CONTAINER(buttonBox), addBtn);

    refreshProductList();
}

void AdminGUI::refreshProductList() {
    // Clear existing
    GtkListBoxRow* row;
    while ((row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(productList), 0)) != nullptr) {
        gtk_container_remove(GTK_CONTAINER(productList), GTK_WIDGET(row));
    }

    for (size_t i = 0; i < products.size(); ++i) {
        Product& p = products[i];
        GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        char buf[256];
        snprintf(buf, sizeof(buf), "[%d] %s - $%.2f (Stock: %d) | %s",
                 p.getId(), p.getName().c_str(), p.getPrice(), p.getStock(), p.getCategory().c_str());
        GtkWidget* label = gtk_label_new(buf);
        gtk_widget_set_halign(label, GTK_ALIGN_START);

        GtkWidget* editBtn = gtk_button_new_with_label("Edit");
        GtkWidget* delBtn = gtk_button_new_with_label("Delete");

        g_object_set_data(G_OBJECT(editBtn), "product-index", GUINT_TO_POINTER(i));
        g_object_set_data(G_OBJECT(delBtn), "product-index", GUINT_TO_POINTER(i));
        g_signal_connect(editBtn, "clicked", G_CALLBACK(onEditProduct), this);
        g_signal_connect(delBtn, "clicked", G_CALLBACK(onDeleteProduct), this);

        gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(hbox), editBtn, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(hbox), delBtn, FALSE, FALSE, 0);
        gtk_list_box_insert(GTK_LIST_BOX(productList), hbox, -1);
    }
    gtk_widget_show_all(productList);
}

void AdminGUI::addProductDialog() {
    GtkWidget* dialog = gtk_dialog_new_with_buttons("Add Product",
        GTK_WINDOW(mainWindow.get()), GTK_DIALOG_MODAL,
        "Add", GTK_RESPONSE_OK,
        "Cancel", GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 200);

    GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget* grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(content), grid);

    // Entry fields
    GtkWidget* nameEntry = gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(nameEntry), "Name");
    GtkWidget* catEntry = gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(catEntry), "Category");
    GtkWidget* priceEntry = gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(priceEntry), "Price");
    GtkWidget* stockEntry = gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(stockEntry), "Stock");

    gtk_grid_attach(GTK_GRID(grid), nameEntry, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), catEntry, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), priceEntry, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), stockEntry, 0, 3, 1, 1);

    gtk_widget_show_all(dialog);

    int result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        std::string name = gtk_entry_get_text(GTK_ENTRY(nameEntry));
        std::string cat = gtk_entry_get_text(GTK_ENTRY(catEntry));
        double price = atof(gtk_entry_get_text(GTK_ENTRY(priceEntry)));
        int stock = atoi(gtk_entry_get_text(GTK_ENTRY(stockEntry)));
        if (!name.empty() && !cat.empty() && price > 0 && stock >= 0) {
            products.emplace_back(name, cat, price, stock);
            refreshProductList();
            saveProductsToFile();
            showMessage("Product added: " + name);
        } else {
            showMessage("Invalid product data");
        }
    }
    gtk_widget_destroy(dialog);
}

void AdminGUI::editProductDialog(int index) {
    if (index < 0 || index >= (int)products.size()) return;
    Product& p = products[index];
    GtkWidget* dialog = gtk_dialog_new_with_buttons("Edit Product",
        GTK_WINDOW(mainWindow.get()), GTK_DIALOG_MODAL,
        "Save", GTK_RESPONSE_OK,
        "Cancel", GTK_RESPONSE_CANCEL, NULL);

    GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget* grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content), grid);

    GtkWidget* nameEntry = gtk_entry_new(); gtk_entry_set_text(GTK_ENTRY(nameEntry), p.getName().c_str());
    GtkWidget* catEntry = gtk_entry_new(); gtk_entry_set_text(GTK_ENTRY(catEntry), p.getCategory().c_str());
    char priceBuf[32], stockBuf[32];
    snprintf(priceBuf, 32, "%.2f", p.getPrice());
    snprintf(stockBuf, 32, "%d", p.getStock());
    GtkWidget* priceEntry = gtk_entry_new(); gtk_entry_set_text(GTK_ENTRY(priceEntry), priceBuf);
    GtkWidget* stockEntry = gtk_entry_new(); gtk_entry_set_text(GTK_ENTRY(stockEntry), stockBuf);

    gtk_grid_attach(GTK_GRID(grid), nameEntry, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), catEntry, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), priceEntry, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), stockEntry, 0, 3, 1, 1);

    gtk_widget_show_all(dialog);
    int result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        p = Product(gtk_entry_get_text(GTK_ENTRY(nameEntry)),
                    gtk_entry_get_text(GTK_ENTRY(catEntry)),
                    atof(gtk_entry_get_text(GTK_ENTRY(priceEntry))),
                    atoi(gtk_entry_get_text(GTK_ENTRY(stockEntry))));
        refreshProductList();
        saveProductsToFile();
        showMessage("Product updated");
    }
    gtk_widget_destroy(dialog);
}

void AdminGUI::deleteProduct(int index) {
    if (index < 0 || index >= (int)products.size()) return;
    std::string name = products[index].getName();
    products.erase(products.begin() + index);
    refreshProductList();
    saveProductsToFile();
    showMessage("Deleted product: " + name);
}

void AdminGUI::saveProductsToFile() {
    try {
        FileManager<Product>::saveToFile(products, "products.dat");
    } catch (const std::exception& e) {
        showMessage(std::string("Save error: ") + e.what());
    }
}

void AdminGUI::showMessage(const std::string& msg) {
    gtk_label_set_text(GTK_LABEL(statusBar), msg.c_str());
    std::cout << msg << std::endl;
}

void AdminGUI::onAddProduct(GtkWidget*, gpointer data) {
    static_cast<AdminGUI*>(data)->addProductDialog();
}
void AdminGUI::onEditProduct(GtkWidget* widget, gpointer data) {
    AdminGUI* gui = static_cast<AdminGUI*>(data);
    int idx = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(widget), "product-index"));
    gui->editProductDialog(idx);
}
void AdminGUI::onDeleteProduct(GtkWidget* widget, gpointer data) {
    AdminGUI* gui = static_cast<AdminGUI*>(data);
    int idx = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(widget), "product-index"));
    gui->deleteProduct(idx);
}

void AdminGUI::run() {
    gtk_widget_show_all(mainWindow.get());
    gtk_main();
}

} // namespace ECommerce