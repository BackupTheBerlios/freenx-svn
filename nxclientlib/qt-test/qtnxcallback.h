/***************************************************************************
                               qtnxcallback.h
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
 
#ifndef _QTNXCALLBACK_H_
#define _QTNXCALLBACK_H_

#include <QObject>
#include "nxcallback.h"

class QtNXCallback : public QObject, public NXCallback
{
	Q_OBJECT
	public:
		virtual void write(std::string *msg);
		virtual void sshRequestAuthenticity(std::string *msg);

		virtual void authenticationFailed();

		// Stdout/stderr from the nxssh process
		virtual void stdout(std::string *msg) {};
		virtual void stderr(std::string *msg) {};
		// Stdin sent to the nxssh process
		virtual void stdin(std::string *msg) {};
		virtual void resumeSessions(std::vector<NXResumeData> *sessions);
	signals:
		void authFailed();
		void message(std::string);
		void sshAuth(std::string);
		void resume();
	private:
		std::vector<NXResumeData> *m_sessions;
};

#endif
