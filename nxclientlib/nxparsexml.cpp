/***************************************************************************
                               nxparsexml.cpp
                             -------------------
    begin                : Friday August 4th 2006
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

#include <iostream>
#include "nxparsexml.h"

using namespace std;

NXParseXML::NXParseXML()
{
}

NXParseXML::~NXParseXML()
{
}

bool NXParseXML::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attributes)
{
	QString key = attributes.value("key");
	QString value = attributes.value("value");
	if (key == "Connection Name"){
		sessionData->sessionName = value;
	}
	
	if (key == "Server Hostname") {
		sessionData->serverHost = value;
	}

	if (key == "Server Port") {
		sessionData->serverPort = value.toInt();
	}

	if (key == "Session Type") {
		sessionData->sessionType = value;
	}

	if (key == "Custom Session Command") {
		sessionData->customCommand = value;
	}

	if (key == "Disk Cache") {
		sessionData->cache = value.toInt();
	}

	if (key == "Image Cache") {
		sessionData->images = value.toInt();
	}

	if (key == "Link Type") {
		sessionData->linkType = value;
	}

	if (key == "Use Render Extension") {
		if (value == "True")
			sessionData->render = true;
		else
			sessionData->render = false;
	}

	if (key == "Image Compression Method") {
		if (value == "JPEG")
			sessionData->imageCompressionMethod = -1;
		else if (value == "PNG")
			sessionData->imageCompressionMethod = 2;
		else if (value == "RAW")
			sessionData->imageCompressionMethod = 0;
	}

	if (key == "JPEG Compression Level") {
		sessionData->imageCompressionLevel = value.toInt();
	}

	if (key == "Desktop Geometry") {
		sessionData->geometry = value;
	}

	if (key == "Keyboard Layout") {
		sessionData->keyboard = value;
	}

	if (key == "Keyboard Type") {
		sessionData->kbtype = value;
	}

	if (key == "Media") {
		if (value == "True")
			sessionData->media = true;
		else
			sessionData->media = false;
	}

	if (key == "Agent Server") {
		sessionData->agentServer = value;
	}

	if (key == "Agent User") {
		sessionData->agentUser = value;
	}

	if (key == "CUPS Port") {
		sessionData->cups = value.toInt();
	}

	if (key == "Authentication Key") {
		sessionData->key = value;
	}

	if (key == "Use SSL Tunnelling") {
		if (value == "True")
			sessionData->encryption = true;
		else
			sessionData->encryption = false;
	}

	if (key == "Enable Fullscreen Desktop") {
		if (value == "True")
			sessionData->fullscreen = true;
		else
			sessionData->fullscreen = false;
	}

	return true;
}

bool NXParseXML::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
	return true;
}

bool NXParseXML::characters(const QString &str)
{
}

bool NXParseXML::fatalError(const QXmlParseException &exception)
{
}
