/***************************************************************************
                               nxclientlib.cpp
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

#include "nxclientlib.h"

#define NXSSH_BIN "nxssh"

// Default NoMachine certificate for FALLBACK
QByteArray cert("-----BEGIN DSA PRIVATE KEY-----\nMIIBuwIBAAKBgQCXv9AzQXjxvXWC1qu3CdEqskX9YomTfyG865gb4D02ZwWuRU/9\nC3I9/bEWLdaWgJYXIcFJsMCIkmWjjeSZyTmeoypI1iLifTHUxn3b7WNWi8AzKcVF\naBsBGiljsop9NiD1mEpA0G+nHHrhvTXz7pUvYrsrXcdMyM6rxqn77nbbnwIVALCi\nxFdHZADw5KAVZI7r6QatEkqLAoGBAI4L1TQGFkq5xQ/nIIciW8setAAIyrcWdK/z\n5/ZPeELdq70KDJxoLf81NL/8uIc4PoNyTRJjtT3R4f8Az1TsZWeh2+ReCEJxDWgG\nfbk2YhRqoQTtXPFsI4qvzBWct42WonWqyyb1bPBHk+JmXFscJu5yFQ+JUVNsENpY\n+Gkz3HqTAoGANlgcCuA4wrC+3Cic9CFkqiwO/Rn1vk8dvGuEQqFJ6f6LVfPfRTfa\nQU7TGVLk2CzY4dasrwxJ1f6FsT8DHTNGnxELPKRuLstGrFY/PR7KeafeFZDf+fJ3\nmbX5nxrld3wi5titTnX+8s4IKv29HJguPvOK/SI7cjzA+SqNfD7qEo8CFDIm1xRf\n8xAPsSKs6yZ6j1FNklfu\n-----END DSA PRIVATE KEY-----");

NXClientLib::NXClientLib(QObject *parent) : QObject(parent)
{
	connect(&session, SIGNAL(authenticated()), this, SLOT(doneAuth()));
	connect(&session, SIGNAL(loginFailed()), this, SLOT(failedLogin()));
}

NXClientLib::~NXClientLib()
{
}

void NXClientLib::invokeNXSSH(QString publicKey, QString serverHost, bool encryption)
{
	QStringList arguments;

	if (publicKey == "default") {
		usingHardcodedKey = true;
		cerr << tr("WARNING: Using hardcoded NoMachine public key.").toStdString() << endl;
		keyFile = new QTemporaryFile;
		keyFile->open();
		
		arguments << "-nx" << "-i" << keyFile->fileName();

		keyFile->write(cert);
		keyFile->close();
	} else {
		arguments << "-i" << publicKey;
	}
	
	if (encryption == true) {
		arguments << "-B";
		session.sessionData.encryption = true;
	}
	
	connect(&nxsshProcess, SIGNAL(started()), this, SLOT(processStarted()));
	connect(&nxsshProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
	connect(&nxsshProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(processParseStdout()));
	connect(&nxsshProcess, SIGNAL(readyReadStandardError()), this, SLOT(processParseStderr()));

	nxsshProcess.setEnvironment(nxsshProcess.systemEnvironment());

	arguments << QString("nx@" + serverHost);
	nxsshProcess.start(NXSSH_BIN, arguments);
}

void NXClientLib::processStarted()
{
	writeCallback(tr("Started nxssh process"));
}

void NXClientLib::processError(QProcess::ProcessError error)
{
	QString message;
	switch (error) {
		case QProcess::FailedToStart:
			message = tr("The process failed to start");
			break;
		case QProcess::Crashed:
			message = tr("The process has crashed");
			break;
		case QProcess::Timedout:
			message = tr("The process timed out");
			break;
		case QProcess::WriteError:
			message = tr("There was an error writing to the process");
			break;
		case QProcess::ReadError:
			message = tr("There was an error reading from the process");
			break;
		case QProcess::UnknownError:
			message = tr("There was an unknown error with the process");
			break;
	}
	
	writeCallback(message);
}

void NXClientLib::failedLogin()
{
	writeCallback(tr("Username or password incorrect"));
	nxsshProcess.terminate();
}

void NXClientLib::processParseStdout()
{
	QString message = nxsshProcess.readAllStandardOutput().data();
	
	// Message 211 is sent if ssh is asking to continue with an unknown host
	if (session.parseResponse(message) == 211) {
		callbackMessage = message.toStdString();
		callback->sshRequestAuthenticity(&callbackMessage);
	}
	cout << message.toStdString();

	callbackStdout = message.toStdString();
	callback->stdout(&callbackStdout);
	
	QStringList messages = splitString(message);
	QStringList::const_iterator i;

	for (i = messages.constBegin(); i != messages.constEnd(); ++i) {
		write(session.parseSSH(*i));
	}
}

QStringList NXClientLib::splitString(QString message)
{
	QStringList lines;
	
	// Split the string message into several different strings separated by '\n'
	for (int i = 0;;i++) {
		if (message.section('\n', i, i).isEmpty() && message.section('\n', i+1, i+1).isEmpty()) {
			break;
		} else
			lines << message.section('\n', i, i);
	}
	
	return lines;
}

void NXClientLib::processParseStderr()
{
	QString message = nxsshProcess.readAllStandardError().data();
	
	cout << "STDERR: " << message.toStdString();
	
	callbackStderr = message.toStdString();
	callback->stderr(&callbackStderr);
}

void NXClientLib::write(QString data)
{
	callbackStdin = data.toStdString();
	callback->stdin(&callbackStdin);
	
	cout << data.toStdString();
	nxsshProcess.write(data.toAscii());
}

void NXClientLib::writeCallback(QString message)
{
	// Done to set a string to give a permanent pointer
	callbackMessage = message.toStdString();
	callback->write(&callbackMessage);
}

void NXClientLib::doneAuth()
{
	if (usingHardcodedKey)
		delete keyFile;
}

void NXClientLib::allowSSHConnect(bool auth)
{
	session.setContinue(auth);
}

void NXClientLib::setSession(NXSessionData nxSession)
{
	
}
