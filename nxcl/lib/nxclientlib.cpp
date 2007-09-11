/***************************************************************************
                               nxclientlib.cpp
                             -------------------
    begin                : Sat 22nd July 2006
    modifications        : July 2007
    copyright            : (C) 2006 by George Wright
    modifications        : (C) 2007 Embedded Software Foundry Ltd. (U.K.)
                         :     Author: Sebastian James
    email                : seb@esfnet.co.uk, gwright@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "nxclientlib_i18n.h"

#include "nxclientlib.h"

#include <fstream>

extern "C" {
#include "errno.h"
}

/*
 * On the location of nxproxy and nxssh binaries
 * --------------------------------------------- 
 * We expect them to be installed in PACKAGE_BIN_DIR (See
 * Makefile.am). So, if nxcl is installed in /usr/bin/nxcl then we
 * call /usr/bin/nxssh and /usr/bin/nxproxy, etc etc.
 */

using namespace std;
using namespace nxcl;

/*!
 * Implementation of the NXClientLibCallbacks class
 */
//@{
NXClientLibCallbacks::NXClientLibCallbacks()
{
}

NXClientLibCallbacks::~NXClientLibCallbacks()
{
}

void
NXClientLibCallbacks::startedSignal (string name)
{
	this->parent->externalCallbacks->write(name + _(" process started"));
}

void
NXClientLibCallbacks::processFinishedSignal (string name)
{
	this->parent->externalCallbacks->write(name + _(" process exited"));
	parent->setIsFinished (true);
}

void
NXClientLibCallbacks::errorSignal (int error)
{
	string message;
	switch (error) {
	case NOTQPROCFAILEDTOSTART:
		message = _("The process failed to start");
		break;
	case NOTQPROCCRASHED:
		message = _("The process has crashed");
		break;
	case NOTQPROCTIMEDOUT:
		message = _("The process timed out");
		break;
	case NOTQPROCWRITEERR:
		message = _("There was an error writing to the process");
		break;
	case NOTQPROCREADERR:
		message = _("There was an error reading from the process");
		break;
	default:
		message = _("There was an unknown error with the process");
		break;
	}
		
	this->parent->externalCallbacks->write (message);
}

void 
NXClientLibCallbacks::readyReadStandardOutputSignal (void)
{
	this->parent->processParseStdout();
}

void 
NXClientLibCallbacks::readyReadStandardErrorSignal (void)
{
	this->parent->processParseStderr();
}
/*!
 * This gets called from within the NXSession object...
 */
void 
NXClientLibCallbacks::noSessionsSignal (void)
{
	/* ...and it calls another callback function in the client code */
	this->parent->externalCallbacks->noSessionsSignal();
}
void
NXClientLibCallbacks::loginFailedSignal (void)
{
	this->parent->loginFailed();
}
void
NXClientLibCallbacks::readyForProxySignal (void)
{
	this->parent->readyproxy();
}
void
NXClientLibCallbacks::authenticatedSignal (void)
{
	this->parent->doneAuth();
}
void
NXClientLibCallbacks::sessionsSignal (list<NXResumeData> data)
{
	this->parent->externalCallbacks->resumeSessionsSignal (data);
}
//@}

/*!e
 * Implementation of the NXClientLib class
 */
//@{
NXClientLib::NXClientLib()
{
	isFinished = false;
	readyForProxy = false;
	proxyData.encrypted = false;
	password = false;

	dbgln ("In NXClientLib constructor");

	this->pNxsshProcess = &this->nxsshProcess;
	this->pNxproxyProcess = &this->nxproxyProcess;

	/* Set up callback pointers */
	this->nxsshProcess.setCallbacks (&callbacks);
	this->nxproxyProcess.setCallbacks (&callbacks);
	this->session.setCallbacks (&callbacks);
	this->callbacks.setParent (this);

	dbgln ("Returning from NXClientLib constructor");
}

NXClientLib::~NXClientLib()
{
	dbgln ("In NXClientLib destructor");
	nxsshProcess.terminate();
}

void NXClientLib::invokeNXSSH (string publicKey, string serverHost, bool encryption, string key, int port)
{
	list<string> arguments;
	stringstream argtmp;
	proxyData.server = serverHost;

	dbgln("invokeNXSSH called");

	// We use same environment for the process as was used for the
	// parent, so remove nxsshProcess.setEnvironment();

	// Start to build the arguments for the nxssh command.
	// notQProcess requires that argv[0] contains the program name
	// FIXME: Here, I place the PACKAGE_BIN_DIR path in front of
	// nxssh, but nxssh may be installed in a different prefix...
	arguments.push_back (PACKAGE_BIN_DIR"/nxssh");
	
	argtmp << "-nx";
	arguments.push_back (argtmp.str());

	argtmp.str("");
	argtmp << "-p" << port;
	arguments.push_back (argtmp.str());

	if (publicKey == "supplied") {
		
		this->keyFile = new notQTemporaryFile;
		this->keyFile->open();
		
		argtmp.str("");
		argtmp << "-i" << this->keyFile->fileName();
		arguments.push_back (argtmp.str());

		this->keyFile->write (key);			
		this->keyFile->close();

	} else {
		argtmp.str("");
		argtmp << "-i" << publicKey;
		arguments.push_back (argtmp.str());
	}
	
	argtmp.str("");
	argtmp << "nx@" << serverHost;
	arguments.push_back (argtmp.str());

	// These options copied from the way Nomachine's client
	// specifies the nxssh command - they make good sense.
	arguments.push_back ("-x");
	arguments.push_back ("-2");
	arguments.push_back ("-oRhostsAuthentication no");
	arguments.push_back ("-oPasswordAuthentication no");
	arguments.push_back ("-oRSAAuthentication no");
	arguments.push_back ("-oRhostsRSAAuthentication no");
	arguments.push_back ("-oPubkeyAuthentication yes");

	if (encryption == true) {
		arguments.push_back("-B");
		session.setEncryption (true);
	} else {
		session.setEncryption (false);
	}

	// -E appears in the call to nxssh for Nomachine's nxclient
	// -version 3 but not 1.5. Is it there in 2?
	// nxssh -E gives this message when called:
	// NX> 285 Enabling skip of SSH config files
	// ...so there you have the meaning.
	arguments.push_back ("-E");
	
	this->nxsshProcess.start (PACKAGE_BIN_DIR"/nxssh", arguments);

	if (nxsshProcess.waitForStarted() == false) {
		dbgln ("nx ssh process didn't start :(");
	}
}

void NXClientLib::requestConfirmation (string msg)
{
	this->externalCallbacks->stdoutSignal (_("This is a placeholder method to deal with sending back a yes or a no answer. "
						 "For now, we just set this->session.setContinue(true);"));
	this->session.setContinue(true);
}

void NXClientLib::reset()
{
	this->nxsshProcess.terminate();       
	this->isFinished = false;
	this->proxyData.encrypted = false;
	this->password = false;	
	this->session.resetSession();
}

void NXClientLib::loginFailed()
{
	this->externalCallbacks->write (_("Got \"Login Failed\""));
	this->isFinished = true;
	this->nxsshProcess.terminate();
}

void NXClientLib::processParseStdout()
{
	string message = nxsshProcess.readAllStandardOutput();

	this->externalCallbacks->stdoutSignal (message);

	dbgln ("NXClientLib::processParseStdout() called");

	int response = 0;
	// Message 211 is sent if ssh is asking to continue with an unknown host
	if ((response = session.parseResponse(message)) == 211) {
		this->requestConfirmation (message);
	}

	dbgln ("NXClientLib::processPraseStdout(): response = " << response);

	if (response == 100000) {
		// A program never started.
		this->isFinished = true;
		return;
	} else if (response > 100000) {
		dbgln ("A process crashed or exited");
		int pid = response - 100000;
		if (this->nxsshProcess.getPid() == pid) {
			this->nxsshProcess.setError(NOTQPROCCRASHED);
			this->externalCallbacks->write (_("nxsshProcess crashed or exited"));
			this->isFinished = true;
		} else if (this->nxproxyProcess.getPid() == pid) {
			this->nxproxyProcess.setError(NOTQPROCCRASHED);
			this->externalCallbacks->write (_("nxproxyProcess crashed or exited"));
			this->isFinished = true;
		} else {
			this->externalCallbacks->write (_("Warning: Don't know what crashed (in processParseStdout())"));
		}
		return;
	}

	// If message 204 is picked, that's authentication failed.
	if (response == 204) {
		this->externalCallbacks->write (_("Got \"Authentication Failed\" from nxssh.\n"
						  "Please check the certificate for the first SSL authentication stage,\n"
						  "in which the \"nx\" user is authenticated."));
		this->isFinished = true;
		return;
	}

	// 147 is server capacity reached
	if (response == 147) {
		this->externalCallbacks->write (_("Got \"Server Capacity Reached\" from nxssh.\n"));
		dbgln ("Got \"Server Capacity Reached\" from nxssh.");
		this->externalCallbacks->serverCapacitySignal();
		this->isFinished = true;
		return;
	}
	
	dbgln ("NXClientLib::processParseStdout(): The message is '" + message + "'(msg end)");
	dbgln ("...and response is " << response);

	notQtUtilities::ensureUnixNewlines (message);
	list<string> msglist;
	list<string>::iterator msgiter;
	notQtUtilities::splitString (message, '\n', msglist);

	for (msgiter = msglist.begin(); msgiter != msglist.end(); msgiter++) {
		dbgln ("NXClientLib::processParseStdout(): Processing the message '" + (*msgiter) + "'(end msg)");
		// On some connections this is sent via stdout instead of stderr?
		if (proxyData.encrypted && readyForProxy && ((*msgiter).find("NX> 999 Bye")!=string::npos)) {
			// This is "NX> 299 Switching connection to: " in
			// version 1.5.0. This was changed in nxssh version
			// 2.0.0-8 (see the nxssh CHANGELOG).
			string switchCommand = "NX> 299 Switch connection to: ";
			stringstream ss;
			ss << "127.0.0.1:" << proxyData.port << " cookie: " << proxyData.cookie << "\n";
			switchCommand += ss.str();
			this->write(switchCommand);
		} else if ((*msgiter).find("NX> 287 Redirected I/O to channel descriptors") != string::npos) {
			dbgln ("287 message found on stdout");
			this->externalCallbacks->write(_("The session has been started successfully"));
		}

		if ((*msgiter).find("Password") != string::npos) {
			this->externalCallbacks->write(_("Authenticating with NX server"));
			password = true;
		}

		if (!readyForProxy) {
			string msg = session.parseSSH (*msgiter);
			if (msg == "204\n" || msg == "147\n") {
				// Auth failed.
				dbgln ("NXClientLib::processParseStdout: Got auth failed or capacity reached, calling this->parseSSH.");
				msg = this->parseSSH (*msgiter);
			}
			if (msg.size() > 0) {
				this->write (msg);
			}
		} else {
			this->write (this->parseSSH (*msgiter));
		}
	}
	return;
}

void NXClientLib::processParseStderr()
{
	string message = nxsshProcess.readAllStandardError();

	dbgln ("In NXClientLib::processParseStderr for message: '" + message + "'(msg end)");

	this->externalCallbacks->stderrSignal (message);

	// Now we need to split the message if necessary based on the \n or \r characters
	notQtUtilities::ensureUnixNewlines (message);

	list<string> msglist;
	list<string>::iterator msgiter;
	notQtUtilities::splitString (message, '\n', msglist);

	for (msgiter=msglist.begin(); msgiter!=msglist.end(); msgiter++) {
		dbgln ("NXClientLib::processParseStderr: Processing the message '" + (*msgiter) + "'(end msg)");
		if (proxyData.encrypted && readyForProxy && ((*msgiter).find("NX> 999 Bye") != string::npos)) {
			string switchCommand = "NX> 299 Switch connection to: ";
			stringstream ss;
			ss << "127.0.0.1:" << proxyData.port << " cookie: " << proxyData.cookie << "\n";
			switchCommand += ss.str();
			this->write(switchCommand);

		} else if ((*msgiter).find("NX> 287 Redirected I/O to channel descriptors") != string::npos) {
			this->externalCallbacks->write(_("The session has been started successfully"));

		} else if ((*msgiter).find("NX> 209 Remote host identification has changed") != string::npos) {
			this->externalCallbacks->write(_("SSH Host Key Problem"));
			this->isFinished = true;

		} else if ((*msgiter).find("NX> 280 Ignoring EOF on the monitored channel") != string::npos) {
			this->externalCallbacks->write(_("Unknown problem..."));
			this->isFinished = true;

		} else if ((*msgiter).find("Host key verification failed") != string::npos) {
			this->externalCallbacks->write(_("SSH host key verification failed"));
			this->isFinished = true;			
		}
	}
}

void NXClientLib::write (string data)
{
	if (data.size() == 0) { return; }

	dbgln ("Writing '" << data << "' to nxssh process.");
	
	this->nxsshProcess.writeIn(data);

	if (password) {
		data = "********";
		password = false;
	}

	// Output this to the user via a signal - this is data going in to nxssh.
	this->externalCallbacks->stdinSignal (data);
}

void NXClientLib::doneAuth()
{
	if (this->keyFile != NULL) {
		this->keyFile->remove();
		delete this->keyFile;
	}
	return;
}

void NXClientLib::allowSSHConnect (bool auth)
{
	session.setContinue (auth);
}

void NXClientLib::setSessionData (NXSessionData *nxSessionData)
{
	session.setSessionData (nxSessionData);
	string a = "NX> 105";
	string d = session.parseSSH(a);
	if (d.size()>0) {
		this->write(d);
	}
}

string NXClientLib::parseSSH (string message)
{
	string rMessage;
	string::size_type pos;
	rMessage = "";

	dbgln ("NXClientLib::parseSSH called for message '" + message + "'");

	if ((pos = message.find("NX> 700 Session id: ")) != string::npos) {
		proxyData.id = message.substr(pos+20, message.length()-pos);
		dbgln ("proxyData.id = " + proxyData.id);
	} else if ((pos = message.find("NX> 705 Session display: ")) != string::npos) {
		stringstream portss;
		int portnum;
		portss << message.substr(pos+25, message.length()-pos);
		portss >> portnum;		
		proxyData.display = portnum;
		dbgln ("proxyData.display = " << proxyData.display);
		proxyData.port = portnum + 4000;
		dbgln ("proxyData.port = " << proxyData.port);
	} else if ((pos = message.find("NX> 706 Agent cookie: ")) != string::npos) {
		proxyData.cookie = message.substr(pos+22, message.length()-pos);
		dbgln ("proxyData.cookie = " + proxyData.cookie);
	} else if ((pos = message.find("NX> 702 Proxy IP: ")) != string::npos) {
		proxyData.proxyIP = message.substr(pos+18, message.length()-pos);
		dbgln ("proxyData.proxyIP = " + proxyData.proxyIP);
	} else if (message.find("NX> 707 SSL tunneling: 1") != string::npos) {
		dbgln ("Setting proxyData.encrypted to true");
		proxyData.encrypted = true;
	} else if (message.find("NX> 147 Server capacity") != string::npos) {
		this->externalCallbacks->write (_("Got \"Server Capacity Reached\" from nxssh."));
		this->externalCallbacks->serverCapacitySignal();
		this->isFinished = true;
	} else if (message.find ("NX> 204 Authentication failed.") != string::npos) {
		this->externalCallbacks->write (_("NX SSH Authentication Failed, finishing"));
		this->isFinished = true;
	}

	if (message.find("NX> 710 Session status: running") != string::npos) {
		invokeProxy();
		session.wipeSessions();
		rMessage = "bye\n";
	}

	dbgln ("NXClientLib::parseSSH returning");
	return rMessage;
}

void NXClientLib::invokeProxy()
{
	dbgln ("invokeProxy called");

	this->externalCallbacks->write(_("Starting NX session"));
	
	int e;
	char * home;
	home = getenv ("HOME");
	stringstream ss;
	ss << home;
	string nxdir = ss.str();
	nxdir += "/.nx";
	// Create the .nx directory first.
	if (mkdir (nxdir.c_str(), 0770)) {
		e = errno;
		if (e != EEXIST) { // We don't mind .nx already
				   // existing, though if there is a
				   // _file_ called $HOME/.nx, we'll
				   // get errors later.
			this->externalCallbacks->write (_("Problem creating .nx directory"));
		}
	}
	// Now the per session directory
	nxdir += "/S-" + proxyData.id;
	if (mkdir (nxdir.c_str(), 0770)) {
		e = errno;
		if (e != EEXIST) { // We don't mind .nx already
			this->externalCallbacks->write (_("Problem creating Session directory"));
		}
	}

	stringstream data;
	if (proxyData.encrypted) {
		data << "nx/nx,session=session,encryption=1,cookie=" << proxyData.cookie 
		     << ",root=" << home << "/.nx"
		     << ",id=" << proxyData.id << ",listen=" << proxyData.port << ":" << proxyData.display << "\n";
		// may also need shmem=1,shpix=1,font=1,product=...

	} else {
		// Not tested yet
		data << "nx,session=session,cookie=" << proxyData.cookie
		     << ",root=" << home
		     << "/.nx,id=" << proxyData.id
		     << ",connect=" << proxyData.server << ":" << proxyData.display << "\n";
	}
	
	// Filename is nxdir plus "/options"
	nxdir += "/options";
	std::ofstream options;
	options.open (nxdir.c_str(), std::fstream::out);
	options << data.str();
	options.close();

	// Build arguments for the call to the nxproxy command
	list<string> arguments;
	arguments.push_back(PACKAGE_BIN_DIR"/nxproxy"); // argv[0] has to be the program name
	arguments.push_back("-S");
	ss.str("");
	ss << "options=" << nxdir;
	ss << ":" << proxyData.display;
	arguments.push_back(ss.str());	

	// This is of type notQProcess.
	this->nxproxyProcess.start(PACKAGE_BIN_DIR"/nxproxy", arguments);

	if (nxproxyProcess.waitForStarted() == false) {
		dbgln ("nxproxy process didn't start :(");
	}
}

bool NXClientLib::chooseResumable (int n)
{
	return (this->session.chooseResumable(n));
}

bool NXClientLib::terminateSession (int n)
{
	return (this->session.terminateSession(n));
}
//@}