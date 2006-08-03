TEMPLATE	= app

FORMS = mainwindow.ui

SOURCES = main.cpp qtnxwindow.cpp qtnxcallback.cpp

HEADERS = qtnxwindow.h qtnxcallback.h

INCLUDEPATH	+= $(QTDIR)/include . ..

DEPENDPATH	+= $(QTDIR)/include

LIBPATH += ..

QT += ui

LIBS	= -lnxclientlib

TARGET		= qtnx