#include <iostream>
#include <memory>
#include <gtk/gtk.h>
#include "gui.h"          // Customer GUI
#include "admin_gui.h"    // Admin GUI (we'll create)
#include "product.h"
#include "cart.h"
#include "user.h"
#include "customer.h"
#include "admin.h"

// Forward declaration of the login dialog
static void showLoginDialog();

int main(int argc, char* argv[]) {
    gtk_init(&argc, &argv);
    showLoginDialog();
    return 0;
}

static void onCustomerLogin(GtkWidget* widget, gpointer data) {
    gtk_widget_destroy(GTK_WIDGET(data)); // close dialog
    // Launch Customer GUI
    ECommerce::AppGUI* customerApp = new ECommerce::AppGUI();
    customerApp->run();
}

static void onAdminLogin(GtkWidget* widget, gpointer data) {
    gtk_widget_destroy(GTK_WIDGET(data));
    // Launch Admin GUI
    ECommerce::AdminGUI* adminApp = new ECommerce::AdminGUI();
    adminApp->run();
}

static void showLoginDialog() {
    GtkWidget* dialog = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Login - E-Commerce Simulator");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 150);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    // Create content area
    GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget* label = gtk_label_new("Select your role:");
    gtk_container_add(GTK_CONTAINER(content), label);

    // Button box for role buttons
    GtkWidget* buttonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_spacing(GTK_BOX(buttonBox), 20);
    gtk_container_add(GTK_CONTAINER(content), buttonBox);

    GtkWidget* customerBtn = gtk_button_new_with_label("Customer");
    GtkWidget* adminBtn = gtk_button_new_with_label("Admin");

    gtk_container_add(GTK_CONTAINER(buttonBox), customerBtn);
    gtk_container_add(GTK_CONTAINER(buttonBox), adminBtn);

    g_signal_connect(customerBtn, "clicked", G_CALLBACK(onCustomerLogin), dialog);
    g_signal_connect(adminBtn, "clicked", G_CALLBACK(onAdminLogin), dialog);

    // Also connect destroy to quit app if dialog is closed
    g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_main_quit), nullptr);

    gtk_widget_show_all(dialog);
    gtk_main();
}