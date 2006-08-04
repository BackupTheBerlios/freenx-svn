/***************************************************************************
                                 nxsession.h
                             -------------------
    begin                : Sat 22nd July 2006
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

#ifndef _NXSESSION_H_
#define _NXSESSION_H_

#include <vector>

#include <QObject>
#include <QString>
#include <QStringList>

#include <fcntl.h>
#include <unistd.h>

struct NXSessionData {
	std::string sessionName;
	std::string sessionType;
	int cache;
	int images;
	std::string linkType;
	bool render;
	std::string backingstore;
	int imageCompressionMethod;
	int imageCompressionLevel;
	std::string geometry;
	std::string keyboard;
	std::string kbtype;
	bool media;
	std::string agentServer;
	std::string agentUser;
	std::string agentPass;
	int cups;
	std::string id;
	std::string screenInfo;
	bool suspended;
};

struct NXResumeData {
	int display;
	std::string sessionType;
	std::string sessionID;
	std::string options;
	int depth;
	std::string screen;
	std::string available;
	std::string sessionName;
};

// This class is used to parse the output from the nxssh session to the server

class NXSession : public QObject
{
	Q_OBJECT
	public:
		NXSession();
		~NXSession();
		
		QString parseSSH(QString);
		int parseResponse(QString);
		void setUsername(QString user) { nxUsername = user; };
		void setPassword(QString pass) { nxPassword = pass; };
		void parseResumeSessions(QStringList);

		void setResolution(int x, int y) { xRes.setNum(x); yRes.setNum(y); };
		void setDepth(int d) { depth.setNum(d); };
		void setRender(bool isRender) { if (isRender) renderSet = "+render"; };
		void setContinue(bool allow) { doSSH = allow; };
		void setSession(NXSessionData *);
		void setEncryption(bool enc) { encryption = enc; };
		
		QString generateCookie();
		

	signals:
		// Emitted when the initial public key authentication is successful
		void authenticated();
		void loginFailed();
		void finished();
	private:
		bool doSSH;
		bool suspendedSessions;
		bool sessionSet;
		bool encryption;
		
		void reset();
		void fillRand(unsigned char *, size_t);

		int stage;

		int devurand_fd;

		QString nxUsername;
		QString nxPassword;

		QString xRes;
		QString yRes;
		QString depth;
		QString renderSet;

		QString type;

		QStringList resumeSessions;

		std::vector<NXResumeData> runningSessions;
		NXSessionData *sessionData;

};

#endif
