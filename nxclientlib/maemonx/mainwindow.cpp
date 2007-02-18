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

GtkWidget *username_input;
GtkWidget *password_input;
GtkWidget *sessions_combo;

GtkWidget *login_button;
GtkWidget *config_button;

GtkWidget *fixed_layout;
GtkWidget *main_layout;
GtkWidget *separator;

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
}

static void config_clicked(GtkWidget *widget, gpointer data)
{
    printf("Lalala config pressed\n");
}

static void login_clicked(GtkWidget *widget, gpointer data)
{
    printf("Lalala login pressed\n");
}
