/***************************************************************************
                                mainwindow.cpp
                             -------------------
    begin                : Saturday 17th February 2007
    copyright            : (C) 2007 by George Wright
    email                : gwright@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "mainwindow.h"
#include "nxhandler.h"

#include <stdio.h>

/* Widgets for the login screen */

GtkWidget *username_input;
GtkWidget *password_input;
GtkWidget *sessions_combo;

GtkWidget *login_button;
GtkWidget *config_button;

GtkWidget *fixed_layout;
GtkWidget *main_layout;
GtkWidget *separator;

/* Widgets for the configuration dialog */

GtkWidget *config_dialog;

GtkWidget *config_pages;
GtkWidget *config_server_page;
GtkWidget *config_desktop_page;
GtkWidget *config_advanced_page;

GtkWidget *session_name;
GtkWidget *host_name;
GtkWidget *port_number;
GtkWidget *use_default_key;
GtkWidget *advanced_key_settings;
GtkWidget *server_labels_layout;
GtkWidget *server_inputs_vlayout;
GtkWidget *server_inputs_hlayout;
GtkWidget *server_layout;
GtkWidget *server_key_layout;

GtkWidget *platform_select;
GtkWidget *type_select;
GtkWidget *link_select;
GtkWidget *desktop_advanced;
GtkWidget *compression_type;
GtkWidget *use_render;
GtkWidget *desktop_labels_layout;
GtkWidget *desktop_selects_layout;
GtkWidget *desktop_type_hlayout;
GtkWidget *desktop_type_vlayout;
GtkWidget *desktop_hlayout;

GtkWidget *use_ssh_encryption;
GtkWidget *cache_size;
GtkWidget *disk_size;
GtkWidget *cache_layout;

NXHandler m_nxhandler;

void setup_gui(GtkWidget *window)
{
    main_layout = gtk_vbox_new(FALSE, 10);

    fixed_layout = gtk_layout_new(NULL, NULL);
    gtk_layout_set_size(GTK_LAYOUT(fixed_layout), 300, 20);
    gtk_container_add(GTK_CONTAINER(window), main_layout);

    gtk_box_pack_start(GTK_BOX(main_layout), fixed_layout, TRUE, FALSE, 0);
    
    username_input = gtk_entry_new();
    password_input = gtk_entry_new();

    sessions_combo = gtk_combo_box_new_text();

    separator = gtk_hseparator_new();

    config_button = gtk_button_new_with_label("Configure");
    login_button = gtk_button_new_with_label("Login");

    g_signal_connect(G_OBJECT(config_button), "clicked", G_CALLBACK(config_clicked), NULL);
    g_signal_connect(G_OBJECT(login_button), "clicked", G_CALLBACK(login_clicked), NULL);

    gtk_combo_box_append_text(GTK_COMBO_BOX(sessions_combo), "Foo");

    gtk_entry_set_visibility(GTK_ENTRY(password_input), FALSE);

    gtk_widget_set_size_request(password_input, 400, 30);
    gtk_widget_set_size_request(username_input, 400, 30);
    gtk_widget_set_size_request(sessions_combo, 400, 30);

    gtk_widget_set_size_request(config_button, 150, 50);
    gtk_widget_set_size_request(login_button, 150, 50);
    gtk_widget_set_size_request(fixed_layout, 672, 220);
    gtk_widget_set_size_request(separator, 600, 0);

    gtk_layout_put(GTK_LAYOUT(fixed_layout), username_input, 236, 0);
    gtk_layout_put(GTK_LAYOUT(fixed_layout), password_input, 236, 50);
    gtk_layout_put(GTK_LAYOUT(fixed_layout), sessions_combo, 236, 100);

    gtk_layout_put(GTK_LAYOUT(fixed_layout), gtk_label_new("Username:"), 36, 0);
    gtk_layout_put(GTK_LAYOUT(fixed_layout), gtk_label_new("Password:"), 36, 50);
    gtk_layout_put(GTK_LAYOUT(fixed_layout), gtk_label_new("Session:"), 36, 100);

    gtk_layout_put(GTK_LAYOUT(fixed_layout), separator, 20, 150);

    gtk_layout_put(GTK_LAYOUT(fixed_layout), config_button, 36, 170);
    gtk_layout_put(GTK_LAYOUT(fixed_layout), login_button, 486, 170);

    setup_config(window);
}

void setup_config(GtkWidget *window)
{
    config_dialog = gtk_dialog_new();

    config_server_page = gtk_vbox_new(FALSE, 10);
    config_desktop_page = gtk_vbox_new(FALSE, 10);
    config_advanced_page = gtk_vbox_new(FALSE, 10);

    gtk_dialog_add_button(GTK_DIALOG(config_dialog), "OK", GTK_RESPONSE_OK);
    gtk_dialog_add_button(GTK_DIALOG(config_dialog), "Cancel", GTK_RESPONSE_CANCEL);

    config_pages = gtk_notebook_new();
    gtk_notebook_append_page(GTK_NOTEBOOK(config_pages), config_server_page, gtk_label_new("Server"));
    gtk_notebook_append_page(GTK_NOTEBOOK(config_pages), config_desktop_page, gtk_label_new("Desktop"));
    gtk_notebook_append_page(GTK_NOTEBOOK(config_pages), config_advanced_page, gtk_label_new("Advanced"));

    gtk_container_add(GTK_CONTAINER(GTK_DIALOG(config_dialog)->vbox), config_pages);

    session_name = gtk_entry_new();
    host_name = gtk_entry_new();
    port_number = gtk_spin_button_new_with_range(1, 65535, 1);
    use_default_key = gtk_check_button_new_with_label("Use default NoMachine key");
    advanced_key_settings = gtk_button_new_with_label("Set custom key");

    server_layout = gtk_hbox_new(FALSE, 10);

    server_labels_layout = gtk_vbox_new(TRUE, 10);
    gtk_box_pack_start(GTK_BOX(server_labels_layout), gtk_label_new("Session name"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(server_labels_layout), gtk_label_new("Hostname"), FALSE, FALSE, 0);

    server_inputs_vlayout = gtk_vbox_new(TRUE, 10);
    server_inputs_hlayout = gtk_hbox_new(FALSE, 10);
    
    gtk_box_pack_start(GTK_BOX(server_inputs_vlayout), session_name, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(server_inputs_vlayout), server_inputs_hlayout, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(server_inputs_hlayout), host_name, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(server_inputs_hlayout), gtk_label_new("Port"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(server_inputs_hlayout), port_number, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(server_layout), server_labels_layout, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(server_layout), server_inputs_vlayout, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(config_server_page), server_layout, TRUE, FALSE, 0);

    server_key_layout = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(server_key_layout), use_default_key, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(server_key_layout), advanced_key_settings, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(config_server_page), server_key_layout, FALSE, FALSE, 0);
 
    platform_select = gtk_combo_box_new();
    type_select = gtk_combo_box_new();
    link_select = gtk_combo_box_new();

    desktop_advanced = gtk_button_new_with_label("Advanced");

    compression_type = gtk_combo_box_new();
    use_render = gtk_check_button_new_with_label("Use RENDER extension");

    desktop_labels_layout = gtk_vbox_new(TRUE, 10);
    desktop_selects_layout = gtk_vbox_new(TRUE, 10);
    desktop_type_hlayout = gtk_hbox_new(FALSE, 10);
    desktop_type_vlayout = gtk_vbox_new(TRUE, 10);

    gtk_box_pack_start(GTK_BOX(desktop_labels_layout), gtk_label_new("Platform"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(desktop_labels_layout), gtk_label_new("Link"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(desktop_labels_layout), gtk_label_new("Compression"), FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(desktop_selects_layout), platform_select, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(desktop_selects_layout), link_select, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(desktop_selects_layout), compression_type, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(desktop_type_hlayout), gtk_label_new("Type"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(desktop_type_hlayout), type_select, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(desktop_type_vlayout), desktop_type_hlayout, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(desktop_type_vlayout), desktop_advanced, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(desktop_type_vlayout), use_render, FALSE, FALSE, 0);

    desktop_hlayout = gtk_hbox_new(FALSE, 10);

    gtk_box_pack_start(GTK_BOX(desktop_hlayout), desktop_labels_layout, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(desktop_hlayout), desktop_selects_layout, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(desktop_hlayout), desktop_type_vlayout, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(config_desktop_page), desktop_hlayout, TRUE, FALSE, 0);

    use_ssh_encryption = gtk_check_button_new_with_label("Use SSH encryption");
    cache_size = gtk_combo_box_new();
    disk_size = gtk_combo_box_new();

    cache_layout = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(cache_layout), gtk_label_new("Memory cache"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(cache_layout), cache_size, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(cache_layout), gtk_label_new("Disk cache"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(cache_layout), disk_size, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(config_advanced_page), use_ssh_encryption, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(config_advanced_page), cache_layout, TRUE, FALSE, 0);


    gtk_widget_show_all(config_dialog);
}



static void config_clicked(GtkWidget *widget, gpointer data)
{
    printf("Lalala config pressed\n");
}

static void login_clicked(GtkWidget *widget, gpointer data)
{
    printf("Lalala login pressed\n");
}
