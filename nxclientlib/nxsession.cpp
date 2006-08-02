/***************************************************************************
                               nxsession.h
                             -------------------
    begin                : Wed 26th July 2006
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
 
// Enumerated type defining the stages through which the client goes when connecting
enum { HELLO_NXCLIENT, ACKNOWLEDGE, SHELL_MODE, AUTH_MODE, LOGIN, LIST_SESSIONS, PARSESESSIONS, STARTSESSION };

/*
	0 HELLO NXCLIENT
	1 Acknowledgement from server
	2 SET SHELL_MODE SHELL
	3 SET AUTH_MODE PASSWORD
	4 login
	5 listsession
	6 resumesession/startsession
*/

#define CLIENT_VERSION "1.5.0"


#include <iostream>

#include <QVector>

using namespace std;

#include "nxsession.h"

NXSession::NXSession()
{
	stage = 0;
	sessionSet = false;
}

NXSession::~NXSession()
{
}

QString NXSession::parseSSH(QString message)
{
	int response = parseResponse(message);

	QString returnMessage;
	
	if (response == 211) {
		if (doSSH == true) {
			returnMessage = "yes";
			doSSH = false;
		} else
			returnMessage = "no";
	}
	
	switch (stage) {
		case HELLO_NXCLIENT:
			if (message.contains("HELLO NXSERVER - Version")) {
				emit authenticated();
				returnMessage = "hello NXCLIENT - Version ";
				returnMessage.append(CLIENT_VERSION);
				stage++;
			}
			break;
		case ACKNOWLEDGE:
			if (response == 105)
				stage++;
			break;
		case SHELL_MODE:
			if (response == 105) {
				returnMessage = "SET SHELL_MODE SHELL";
				stage++;
			}
			break;
		case AUTH_MODE:
			if (response == 105) {
				returnMessage = "SET AUTH_MODE PASSWORD";
				stage++;
			}
			break;
		case LOGIN:
			switch (response) {
				case 105:
					returnMessage = "login";
					break;
				case 101:
					returnMessage = nxUsername;
					break;
				case 102:
					returnMessage = nxPassword;
					break;
				case 103:
					stage++;
					break;
				}
			break;
		case LIST_SESSIONS:
			if (response == 105) {
				// Get a list of the available sessions on the server
				returnMessage = "listsession --user=\"" + nxUsername + "\" --status=\"suspended,running\" --geometry=\"" + xRes + "x" + yRes + "+" + renderSet + "\" --type=\"" + type + "\"";
				stage++;
			}
			break;
		case PARSESESSIONS:
			if (response != 148)
				resumeSessions << message;
			if (response == 148)
				parseResumeSessions(resumeSessions);
			break;
		case STARTSESSION:
			if (response == 105 && sessionSet) {
				int media = 0;
				int render = 0;
				int encryption = 0;

				if (sessionData.media)
					media = 1;
				if (sessionData.render)
					render = 1;
				if (sessionData.encryption)
					encryption = 1;
					
				if (sessionData.suspended) {
					// These are the session parameters that NoMachine's client sends for resume
					returnMessage = "restoresession --id=\"" + sessionData.id +
					"\" --session=\"" + sessionData.sessionName +
					"\" --type=\"" + sessionData.sessionType +
					"\" --cache=\"" + sessionData.cache +
					"M\" --images=\"" + sessionData.images +
					"M\" --cookie=\"" + sessionData.sessionCookie +
					"\" --link=\"" + sessionData.linkType +
					"\" --kbtype=\"" + sessionData.kbtype +
					"\" --nodelay=\"1\" --encryption=\"" + encryption +
					"\" --backingstore=\"" + sessionData.backingstore +
					"\" --geometry=\"" + sessionData.geometry +
					"\" --media=\"" + sessionData.media +
					"\" --agent_server=\"" + sessionData.agentServer +
					"\" --agent_user=\"" + sessionData.agentUser +
					"\" --agent_password=\"" + sessionData.agentPass + "\"";
				} else {
					returnMessage = "startsession --session=\"" + sessionData.sessionName +
					"\" --type=\"" + sessionData.sessionType +
					"\" --cache=\"" + sessionData.cache +
					"M\" --images=\"" + sessionData.images +
					"M\" --cookie=\"" + sessionData.sessionCookie +
					"\" --link=\"" + sessionData.linkType +
					"\" --render=\"" + render +
					"\" --encryption=\"" + encryption +
					"\" --backingstore=\"" + sessionData.backingstore +
					"\" --imagecompressionmethod=\"" + sessionData.imageCompressionMethod +
					"\" --geometry=\"" + sessionData.geometry +
					"\" --keyboard=\"" + sessionData.keyboard +
					"\" --kbtype=\"" + sessionData.kbtype +
					"\" --media=\"" + media +
					"\" --agent_server=\"" + sessionData.agentServer +
					"\" --agent_user=\"" + sessionData.agentUser +
					"\" --agent_password=\"" + sessionData.agentPass +
					"\" --screeninfo=\"" + sessionData.screenInfo + "\"";
				}
			}
			break;
	}

	if (!returnMessage.isEmpty()) {
		returnMessage.append("\n");
		return returnMessage;
	} else
		return 0;
}

void NXSession::setSession(NXSessionData session)
{
	sessionData = session;
	sessionSet = true;
}

int NXSession::parseResponse(QString message)
{
	int idx1, idx2, response;
	// Find out the server response number
	// This will only be present in strings which contain "NX>"
	if (message.contains("NX>")) {
		idx1 = message.indexOf("NX>") + 4;
		idx2 = message.indexOf(" ", idx1);
		response = message.mid(idx1, idx2-idx1).toInt();
	} else {
		response = 0;
	}

	return response;
}

void NXSession::parseResumeSessions(QStringList rawdata)
{
	int at, i;
	QStringList sessions;

	for (i = 0; i < rawdata.size(); ++i) {
		if (rawdata.at(i) == "------- ---------------- -------------------------------- -------- ----- -------------- --------- ----------------------") {
			at = i;
		}
	}

	for (i = at+1; i < rawdata.size(); ++i) {
		if (!rawdata.at(i).contains("NX> 148"))
			sessions << rawdata.at(i);
	}

	QVector<QStringList> rawsessions;

	for (i = 0; i < sessions.size(); ++i)
		rawsessions.append(sessions.at(i).simplified().split(' '));

	NXResumeData resData;

	for (i = 0; i < rawsessions.size(); ++i) {
		resData.display = rawsessions.at(i).at(0).toInt();
		resData.sessionType = rawsessions.at(i).at(1);
		resData.sessionID = rawsessions.at(i).at(2);
		resData.options = rawsessions.at(i).at(3);
		resData.depth = rawsessions.at(i).at(4).toInt();
		resData.screen = rawsessions.at(i).at(5);
		resData.available = rawsessions.at(i).at(6);
		resData.sessionName = rawsessions.at(i).at(7);
		runningSessions.push_back(resData);
	}

	if (runningSessions.size() != 0)
		suspendedSessions = true;

	stage++;
}

void NXSession::setUsername(QString user)
{
	nxUsername = user;
}

void NXSession::setPassword(QString pass)
{
	nxPassword = pass;
}

void NXSession::reset()
{
	stage = 0;
}

void NXSession::setXRes(int x)
{
	xRes.setNum(x);
}

void NXSession::setYRes(int y)
{
	yRes.setNum(y);
}

void NXSession::setDepth(int d)
{
	depth.setNum(d);
}

void NXSession::setRender(bool isRender)
{
	if (isRender)
		renderSet = "render";
}

void NXSession::setContinue(bool allow)
{
		doSSH = allow;
}
