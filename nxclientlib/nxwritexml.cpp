/***************************************************************************
                               nxwritexml.cpp
                             -------------------
    begin                : Wednesday August 9th 2006
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

#include "nxwritexml.h"

NXWriteXML::NXWriteXML()
{
}

NXWriteXML::~NXWriteXML()
{
}

void NXWriteXML::write(QFile file)
{
	file.open(QFile::WriteOnly);
	
	QTextStream xml(&file);
	xml.setCodec("UTF-8");

	xml << "<!DOCTYPE NXClientLibSettings>\n";
	
	xml << "<option key=\"Connection Name\" value=\"" << escape(sessionData.sessionName) << "\" />\n";
	xml << "<option key=\"Server Hostname\" value=\"" << escape(sessionData.serverHost) << "\" />\n";
	xml << "<option key=\"Server Port\" value=\"" << escape(QString::number(sessionData.serverPort)) << "\" />\n";
	xml << "<option key=\"Session Type\" value=\"" << escape(sessionData.sessionType) << "\" />\n";
	xml << "<option key=\"Custom Session Command\" value=\"" << escape(sessionData.customCommand) << "\" />\n";
	xml << "<option key=\"Disk Cache\" value=\"" << escape(QString::number(sessionData.cache)) << "\" />\n";
	xml << "<option key=\"Image Cache\" value=\"" << escape(QString::number(sessionData.images)) << "\" />\n";
	xml << "<option key=\"Link Type\" value=\"" << escape(sessionData.linkType) << "\" />\n";
	
	if (sessionData.render == true)
		xml << "<option key=\"Use Render Extension\" value=\"True\" />\n";
	else
		xml << "<option key=\"Use Render Extension\" value=\"False\" />\n";

	xml << "<option key=\"Image Compression Method\" value=\"" << escape(sessionData.sessionName) << "\" />\n";
	
	xml << "<option key=\"JPEG Compression Level\" value=\"" << escape(QString::number(sessionData.imageCompressionLevel)) << "\" />\n";
	xml << "<option key=\"Desktop Geometry\" value=\"" << escape(sessionData.geometry) << "\" />\n";
	xml << "<option key=\"Keyboard Layout\" value=\"" << escape(sessionData.keyboard) << "\" />\n";
	xml << "<option key=\"Keyboard Type\" value=\"" << escape(sessionData.kbtype) << "\" />\n";

	if (sessionData.media == true)
		xml << "<option key=\"Media\" value=\"True\" />\n";
	else
		xml << "<option key=\"Media\" value=\"False\" />\n";

	xml << "<option key=\"Agent Server\" value=\"" << escape(sessionData.agentServer) << "\" />\n";
	xml << "<option key=\"Agent User\" value=\"" << escape(sessionData.agentUser) << "\" />\n";
	xml << "<option key=\"CUPS Port\" value=\"" << escape(QString::number(sessionData.cups)) << "\" />\n";
	xml << "<option key=\"Authentication Key\" value=\"" << escape(sessionData.key) << "\" />\n";

	if (sessionData.encryption == true)
		xml << "<option key=\"Use SSL Tunnelling\" value=\"True\" />\n";
	else
		xml << "<option key=\"Use SSL Tunnelling\" value=\"False\" />\n";

	if (sessionData.fullscreen == true)
		xml << "<option key=\"Enable Fullscreen Desktop\" value=\"True\" />\n";
	else
		xml << "<option key=\"Enable Fullscreen Desktop\" value=\"False\" />\n";

	xml << "</NXClientLibSettings>\n";

	file.close();
}

QString NXWriteXML::escape(QString plain)
{
	QString formatted;
	formatted = plain.replace('<', "&lt;");
	formatted = plain.replace('>', "&rt;");
	formatted = plain.replace('&', "&amp;");
	return formatted;
}
