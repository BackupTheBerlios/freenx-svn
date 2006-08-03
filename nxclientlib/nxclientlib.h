/***************************************************************************
                                nxclientlib.h
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

#ifndef _NXCLIENTLIB_H_
#define _NXCLIENTLIB_H_

#include <QProcess>
#include <QTemporaryFile>

#include <iostream>

#include "nxsession.h"
#include "nxcallback.h"

using namespace std;

struct ProxyData {
	QString id;
	int display;
	QString cookie;
	QString proxyIP;
	bool encrypted;
};

class NXClientLib : public QObject
{
	Q_OBJECT
	public:
		NXClientLib(QObject *parent = 0);
		~NXClientLib();

		// publicKey is the path to the ssh public key file to authenticate with. Pass "default" to use the default NoMachine key
		// serverHost is the hostname of the NX server to connect to
		// encryption is whether to use an encrypted NX session
		void invokeNXSSH(QString publicKey = "default", QString serverHost = "", bool encryption = true);

		// Overloaded to give callback data on write
		void write(QString);

		// Set the username and password for NX to log in with
		void setUsername(const char *user) { session.setUsername(user); };
		void setPassword(const char *pass) { session.setPassword(pass); };

		void setCallback(NXCallback *cb) { callback = cb; };

		void setResolution(int x, int y) { session.setResolution(x, y); };
		void setDepth(int depth) { session.setDepth(depth); };
		void setRender(bool render) { session.setRender(render); };
		void allowSSHConnect(bool auth);

		void setSession(NXSessionData);

		void invokeProxy();

		QString parseSSH(QString);
	public slots:
		void processStarted();
		void processError(QProcess::ProcessError);
		
		void processParseStdout();
		void processParseStderr();

		void doneAuth();
		void failedLogin();

		void finished() { isFinished = true; };
	private:
		bool usingHardcodedKey;
		bool isFinished;
		
		QProcess nxsshProcess;
		QProcess nxproxyProcess;
		
		QTemporaryFile *keyFile;
		
		NXSession session;
		NXCallback *callback;
		
		QStringList splitString(QString);
		void writeCallback(QString);

		string callbackMessage;
		string callbackStdin;
		string callbackStdout;
		string callbackStderr;
		
		ProxyData proxyData;
};

#endif
