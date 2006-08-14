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
	if (!sessionName.isEmpty()) {
		NXParseXML handler;
		handler.setSessionData(&config);
	
		QFile file(QDir::homePath() + ".qtnx/" + sessionName + ".nxml");
		QXmlInputSource inputSource(&file);

		QXmlSimpleReader reader;
		reader.setContentHandler(&handler);
		reader.setErrorHandler(&handler);
		reader.parse(inputSource);
	}
	
	ui_sd.setupUi(this);
}

QtNXSettings::~QtNXSettings()
{
}
