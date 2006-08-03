/***************************************************************************
                               qtnxcallback.cpp
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

#include "qtnxcallback.h"

void QtNXCallback::write(std::string *msg)
{
	emit message(*msg);
}

void QtNXCallback::sshRequestAuthenticity(std::string *msg)
{
	emit sshAuth(*msg);
}

void QtNXCallback::authenticationFailed()
{
	emit authFailed();
}

void QtNXCallback::resumeSessions(std::vector<NXResumeData> *sessions)
{
	emit resume();
	m_sessions = sessions;
}
