TEMPLATE	= lib

CONFIG		= qt warn_on debug dll

HEADERS		= nxclientlib.h nxsession.h

SOURCES		= nxclientlib.cpp nxsession.cpp

INCLUDEPATH	+= $(QTDIR)/include .

DEPENDPATH	+= $(QTDIR)/include

QT		= core

TARGET		= nxclientlib