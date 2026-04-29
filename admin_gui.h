#ifndef ADMIN_GUI_H
#define ADMIN_GUI_H

#include <gtk/gtk.h>
#include <memory>
#include <vector>
#include "product.h"

namespace ECommerce {

class AdminGUI {
private:
    std::unique_ptr<GtkWidget, decltype(&gtk_widget_destroy)> mainWindow{nullptr, gtk_widget_destroy};
    GtkWidget* productList = nullptr;
    GtkWidget* statusBar = nullptr;
    std::vector<Product> products;

    void refreshProductList();
    void addProductDialog();
    void editProductDialog(int index);
    void deleteProduct(int index);
    void showMessage(const std::string& msg);
    void saveProductsToFile();

    static void onAddProduct(GtkWidget* widget, gpointer data);
    static void onEditProduct(GtkWidget* widget, gpointer data);
    static void onDeleteProduct(GtkWidget* widget, gpointer data);

public:
    AdminGUI();
    void run();
};

} // namespace ECommerce

#endif