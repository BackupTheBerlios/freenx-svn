TEMPLATE	= app

CONFIG		+= qt warn_on debug

FORMS = settingsdialog.ui logindialog.ui sessionsdialog.ui keydialog.ui logwindow.ui

SOURCES = main.cpp qtnxwindow.cpp qtnxsettings.cpp qtnxsessions.cpp nxparsexml.cpp nxwritexml.cpp

HEADERS = qtnxwindow.h qtnxsettings.h qtnxsessions.h nxparsexml.h nxwritexml.h

INCLUDEPATH	+= $(QTDIR)/include

QMAKE_CXXFLAGS += $$system(pkg-config --cflags nxcl)

DEPENDPATH	+= $(QTDIR)/include

LIBS += $$system(pkg-config --libs nxcl)

QT += ui xml

TARGET		= qtnx
