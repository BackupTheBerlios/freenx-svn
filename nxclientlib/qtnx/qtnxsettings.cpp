/***************************************************************************
                               qtnxsettings.cpp
                             -------------------
    begin                : Saturday August 12th 2006
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

#include <QFile>
#include <QDir>

#include "qtnxsettings.h"

#include "nxdata.h"
#include "nxparsexml.h"

QtNXSettings::QtNXSettings(QString sessionName) : QDialog()
{
	if (!sessionName.isEmpty())
		fileName = QDir::homePath() + ".qtnx/" + sessionName + ".nxml";
	else
		fileName = "";
		
	parseFile();
	
	ui_sd.setupUi(this);
	ui_sd.sessionName->setText(sessionName);

}

QtNXSettings::~QtNXSettings()
{
}

void QtNXSettings::parseFile()
{
	if (!fileName.isEmpty()) {
		NXParseXML handler;
		handler.setSessionData(&config);
	
		QFile file(fileName);
		QXmlInputSource inputSource(&file);

		QXmlSimpleReader reader;
		reader.setContentHandler(&handler);
		reader.setErrorHandler(&handler);
		reader.parse(inputSource);

		ui_sd.hostname->setText(config.serverHost);
		ui_sd.port->setValue(config.serverPort);

		if (config.key.isEmpty())
			ui_sd.defaultKey->setChecked(true);
		else {
			ui_sd.defaultKey->setChecked(false);
			ui_sd.setAuthKeyButton->setEnabled(true);
		}

		if (config.sessionType == "unix-kde") {
			ui_sd.platform->setCurrentIndex(ui_sd.platform->findText(tr("UNIX")));
			ui_sd.type->setCurrentIndex(ui_sd.type->findText(tr("KDE")));
		} else if (config.sessionType == "unix-gnome") {
			ui_sd.platform->setCurrentIndex(ui_sd.platform->findText(tr("UNIX")));
			ui_sd.type->setCurrentIndex(ui_sd.type->findText(tr("KDE")));
		} else if (config.sessionType == "unix-cde") {
			ui_sd.platform->setCurrentIndex(ui_sd.platform->findText(tr("UNIX")));
			ui_sd.type->setCurrentIndex(ui_sd.type->findText(tr("CDE")));
		} else if (config.sessionType == "unix-application") {
			ui_sd.platform->setCurrentIndex(ui_sd.platform->findText(tr("UNIX")));
			ui_sd.type->setCurrentIndex(ui_sd.type->findText(tr("Custom")));
			ui_sd.desktopSettingButton->setEnabled(true);
		}

		if (config.linkType == "modem")
			ui_sd.link->setCurrentIndex(ui_sd.link->findText("Modem"));
		else if (config.linkType == "isdn")
			ui_sd.link->setCurrentIndex(ui_sd.link->findText("ISDN"));
		else if (config.linkType == "adsl")
			ui_sd.link->setCurrentIndex(ui_sd.link->findText("ADSL"));
		else if (config.linkType == "wan")
			ui_sd.link->setCurrentIndex(ui_sd.link->findText("WAN"));
		else if (config.linkType == "lan")
			ui_sd.link->setCurrentIndex(ui_sd.link->findText("LAN"));

		if (config.imageCompressionMethod == -1) {
			ui_sd.imageCompressionType->setCurrentIndex(ui_sd.imageCompressionType->findText(tr("JPEG")));
			ui_sd.imageQualityLevel->setValue(config.imageCompressionLevel);
		} else if (config.imageCompressionMethod == 2)
			ui_sd.imageCompressionType->setCurrentIndex(ui_sd.imageCompressionType->findText(tr("PNG")));
		else if (config.imageCompressionMethod == 0)
			ui_sd.imageCompressionType->setCurrentIndex(ui_sd.imageCompressionType->findText(tr("Raw X11")));

		if (config.geometry == "640x480+0+0")
			ui_sd.resolution->setCurrentIndex(ui_sd.resolution->findText(tr("640x480")));
		else if (config.geometry == "800x600+0+0")
			ui_sd.resolution->setCurrentIndex(ui_sd.resolution->findText(tr("640x480")));
		else if (config.geometry == "1024x768+0+0")
			ui_sd.resolution->setCurrentIndex(ui_sd.resolution->findText(tr("640x480")));
		else {
			if (config.fullscreen) {
				ui_sd.resolution->setCurrentIndex(ui_sd.resolution->findText(tr("Fullscreen")));
			} else {
				ui_sd.resolution->setCurrentIndex(ui_sd.resolution->findText(tr("Custom")));
				
				QString res;
				res = config.geometry.left(config.geometry.length() - 4);
				ui_sd.width->setValue(res.split('x').at(0).toInt());
				ui_sd.height->setValue(res.split('x').at(1).toInt());
				ui_sd.width->setEnabled(true);
				ui_sd.height->setEnabled(true);
			}
		}

		ui_sd.render->setChecked(config.render);
	}
}
