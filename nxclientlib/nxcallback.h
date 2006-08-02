/***************************************************************************
                                nxcallback.h
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

#include <iostream>
#include <vector>

#include "nxsession.h"

// Callback interface for parent app to get status updates from NXClientLib.
class NXCallback
{
	public:
		virtual void write(std::string *msg) = 0;
		// Called by nxssh if we get a request for ssh authenticity
		// msg is the message passed by nxssh
		virtual void sshRequestAuthenticity(std::string *msg) = 0;
		// Called if the NX server replies that authentication has failed
		virtual void authenticationFailed() = 0;

		// Stdout/stderr from the nxssh process
		virtual void stdout(std::string *msg) = 0;
		virtual void stderr(std::string *msg) = 0;
		// Stdin sent to the nxssh process
		virtual void stdin(std::string *msg) = 0;

		virtual void resumeSessions(std::vector<NXResumeData> *sessions) = 0;
};
