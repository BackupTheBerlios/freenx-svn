/***************************************************************************
                               qtnxwindow.cpp
                             -------------------
    begin                : Thursday August 3rd 2006
    copyright            : (C) 2006 by George Wright
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

#include "qtnxwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <QX11Info>

QtNXWindow::QtNXWindow() : QMainWindow()
{
	loginDialog = new QWidget(this);
	menuBar = new QMenuBar(this);
	statusBar = new QStatusBar(this);

	fileMenu = new QMenu(tr("&File"));
	connectionMenu = new QMenu(tr("Conn&ection"));
	
	ui_lg.setupUi(loginDialog);
	setCentralWidget(loginDialog);
	setStatusBar(statusBar);
	setMenuBar(menuBar);

	statusBar->showMessage(tr("Ready"));
	statusBar->setSizeGripEnabled(false);
	
	menuBar->addMenu(fileMenu);
	menuBar->addMenu(connectionMenu);

	fileMenu->addAction(tr("Quit"), qApp, SLOT(quit()), QKeySequence(tr("CTRL+Q")));

	connectionMenu->addAction(tr("Connect..."), this, SLOT(startConnect()));

	QDir dir(QDir::homePath()+"/.qtnx","*.nxml");
	for (unsigned int i=0;i<dir.count();i++) {
		QString conn=dir[i];
		ui_lg.session->addItem(conn.left(conn.length()-5));
	}
	ui_lg.session->addItem(tr("Create new session"));


	connect(ui_lg.connectButton, SIGNAL(pressed()), this, SLOT(startConnect()));
	connect(ui_lg.configureButton, SIGNAL(pressed()), this, SLOT(configure()));
}

QtNXWindow::~QtNXWindow()
{
}

void QtNXWindow::startConnect()
{
	QDesktopWidget dw;
	QX11Info info;

	NXParseXML handler;
	handler.setData(&config);

	QFile file(QDir::homePath() + "/.qtnx/" + ui_lg.session->currentText() + ".nxml");
	QXmlInputSource inputSource(&file);

	QXmlSimpleReader reader;
	reader.setContentHandler(&handler);
	reader.setErrorHandler(&handler);
	reader.parse(inputSource);

	session.sessionName = config.sessionName;
	session.sessionType = config.sessionType;
	session.cache = config.cache;
	session.images = config.images;
	session.linkType = config.linkType;
	session.render = config.render;
	session.backingstore = "when_requested";
	session.imageCompressionMethod = config.imageCompressionMethod;
	session.imageCompressionLevel = config.imageCompressionLevel;
	session.geometry = config.geometry;
	session.keyboard = "defkeymap";
	session.kbtype = "pc102/defkeymap";
	session.media = config.media;
	session.agentServer = config.agentServer;
	session.agentUser = config.agentUser;
	session.agentPass = config.agentPass;
	session.cups = config.cups;
	session.fullscreen = config.fullscreen;
	
	if (!config.key.isEmpty())
		session.key = config.key;
	else
		session.key = "default";

	if (config.sessionType == "unix-application")
		session.customCommand = config.customCommand;

	if (config.encryption == false)
		nxClient.invokeNXSSH(session.key, config.serverHost, false, 0, config.serverPort);
	else
		nxClient.invokeNXSSH(session.key, config.serverHost, true, 0, config.serverPort);
		
	nxClient.setUsername(ui_lg.username->text());
	nxClient.setPassword(ui_lg.password->text());
	nxClient.setResolution(dw.screenGeometry(this).width(), dw.screenGeometry(this).height());
	nxClient.setDepth(info.depth());
	connect(&nxClient, SIGNAL(resumeSessions(QList<NXResumeData>)), this, SLOT(loadResumeDialog(QList<NXResumeData>)));
	connect(&nxClient, SIGNAL(noSessions()), this, SLOT(noSessions()));
	//nxClient.setSession(&session);
}

void QtNXWindow::configure()
{
	if (ui_lg.session->currentText() == tr("Create new session"))
		settingsDialog = new QtNXSettings("");
	else
		settingsDialog = new QtNXSettings(ui_lg.session->currentText());

	connect(settingsDialog, SIGNAL(closing()), this, SLOT(configureClosed()));

	settingsDialog->show();
}

void QtNXWindow::configureClosed()
{
	while (ui_lg.session->count() != 0) {
		ui_lg.session->removeItem(0);
	}

	QDir dir(QDir::homePath()+"/.qtnx","*.nxml");
	for (unsigned int i=0;i<dir.count();i++) {
		QString conn=dir[i];
		ui_lg.session->addItem(conn.left(conn.length()-5));
	}
	ui_lg.session->addItem(tr("Create new session"));
}

void QtNXWindow::loadResumeDialog(QList<NXResumeData> data)
{
	delete sessionsDialog;
	sessionsDialog = new QtNXSessions(data);
	sessionsDialog->show();

	connect(sessionsDialog, SIGNAL(newPressed()), this, SLOT(resumeNewPressed()));
	connect(sessionsDialog, SIGNAL(resumePressed(QString)), this, SLOT(resumeResumePressed(QString)));
}

void QtNXWindow::resumeNewPressed()
{
	nxClient.setSession(&session);
}

void QtNXWindow::resumeResumePressed(QString id)
{
	session.id = id;
	session.suspended = true;
	nxClient.setSession(&session);
}

void QtNXWindow::noSessions()
{
	nxClient.setSession(&session);
}
