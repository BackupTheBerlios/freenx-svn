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
#include <QDir>
#include <QFile>

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

	QDir dir(QDir::homePath()+".qtnx","*.nxml");
	for (unsigned int i=0;i<dir.count();i++) {
		QString conn=dir[i];
		if (conn.compare("nxclient.conf")==0) continue;
		ui_lg.session->addItem(conn.left(conn.length()-3));
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
// 	QDesktopWidget dw;
// 	QX11Info info;

	NXParseXML handler;
	handler.setSessionData(&config);
	
	QFile file(QDir::homePath() + ".qtnx/" + ui_lg.session->currentText() + ".nxml");
	QXmlInputSource inputSource(&file);

	QXmlSimpleReader reader;
	reader.setContentHandler(&handler);
	reader.setErrorHandler(&handler);
	reader.parse(inputSource);

/*	session->sessionName = ui_mw.session_name->text();
	session->sessionType = ui_mw.session_type->currentText();
	session->cache = ui_mw.cache->currentText().toInt();
	session->images = ui_mw.images->currentText().toInt();
	session->linkType = ui_mw.link_type->currentText();
	
	if (ui_mw.render->checkState() == 0)
		session->render = false;
	if (ui_mw.render->checkState() == 2)
		session->render = true;
	
	session->backingstore = "when_requested";

	if (ui_mw.image_compression->currentText() == "JPEG")
		session->imageCompressionMethod = -1;
	if (ui_mw.image_compression->currentText() == "RAW")
		session->imageCompressionMethod = 0;
	if (ui_mw.image_compression->currentText() == "PNG")
		session->imageCompressionMethod = 2;
		
	session->imageCompressionLevel = ui_mw.jpeg_quality->value();

	if (ui_mw.resolution->currentText() == "640x480")
		session->geometry = "640x480+0+0";
	if (ui_mw.resolution->currentText() == "800x600")
		session->geometry = "800x600+0+0";
	if (ui_mw.resolution->currentText() == "1024x768")
		session->geometry = "1024x768+0+0";
	if (ui_mw.resolution->currentText() == "1600x1200")
		session->geometry = "1600x1200+0+0";

	session->keyboard = "defkeymap";
	session->kbtype = "pc102/defkeymap";
	
	session->media = false;
	session->agentServer = ui_mw.rdp_server->text();
	session->agentUser = ui_mw.rdp_username->text();
	session->agentPass = ui_mw.rdp_password->text();
	if (QApplication::desktop()->paintEngine()->hasFeature(QPaintEngine::Antialiasing))
		m_lib.setRender(true);
	else
		m_lib.setRender(false);
		
	// session->screenInfo = QString::number(dw.screenGeometry(this).width()) + "x" + QString::number(dw.screenGeometry(this).height()) + "x" + QString::number(info.depth()) + "render";
	session->suspended = false;

	if (ui_mw.encryption->checkState() == 0)
		m_lib.invokeNXSSH("default" , ui_mw.server->text(), false);
	if (ui_mw.encryption->checkState() == 2)
		m_lib.invokeNXSSH("default" , ui_mw.server->text(), true);
		
	m_lib.setUsername(ui_mw.username->text());
	m_lib.setPassword(ui_mw.password->text());
	m_lib.setResolution(dw.screenGeometry(this).width(), dw.screenGeometry(this).height());
	m_lib.setDepth(info.depth());
	m_lib.setSession(session);*/
}

void QtNXWindow::configure()
{
	if (ui_lg.session->currentText() == tr("Create new session"))
		settingsDialog = new QtNXSettings(0);
	else
		settingsDialog = new QtNXSettings(ui_lg.session->currentText());
		
	settingsDialog->show();
}
