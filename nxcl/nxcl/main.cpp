/*
 * nxcld: The NXCL dbus daemon.
 *
 * This program should be launched by a client - that might be gnxc,
 * qtnxc, fltknxc, tknxc, emacsnxc.... etc etc you get the picture.
 *
 * This graphical launcher program is the place where the user can
 * select the connection type, and enter his username and password. It
 * launches nxcld via a dbus call (or some other way?), then transmits
 * the settings to nxcld via dbus. nxcld then negotiates the NX
 * connection - launching nxssh and nxproxy. nxcld will send back a
 * list of sessions to the launching NX client (if there are
 * multiple available sessions) and a signal to say if the connection
 * has been launched or if an error occurred.
 *
 * (C) 2007 Embedded Software Foundry Ltd. (U.K.)
 * Author: Sebastian James
 * Email:  seb@esfnet.co.uk
 *
 * Released under the terms of the GNU GPL version 2.
 */

#include "../config.h"
#include "nxclientlib.h"
#include "nxcl.h"
#include <fstream>

/* This define is required for slightly older versions of dbus as
 * found, for example, in Ubuntu 6.06. */
#define DBUS_API_SUBJECT_TO_CHANGE 1
extern "C" {
#include <string.h>
#include <dbus/dbus.h>
}

using namespace nxcl;
using namespace std;

/*!
 * Log file ofstream is a global variable, as required by nxcl.
 */
ofstream debugLogFile;

/*!
 * This probing routine is executed on a regular basis. It will set
 * off suitable signals if there is stdout or stderr to be analysed
 * from the nxssh process. It also recognises when the nxssh program
 * has exited.
 */
bool probeNXCL (NXClientLib* lib);

int main (int argc, char **argv)
{
	Nxcl nxcl;

	debugLogFile.open("/tmp/nxcl.log", ios::out|ios::trunc);
	if (!debugLogFile.is_open()) {
		nxcl.callbacks.write ("Odd, couldn't open /tmp/nxcl.log");
	}

	if (argc!=2) {
		nxcl.callbacks.write ("This program is usually executed by another program. "
				      "Trying to execute it alone is probably not the right "
				      "thing to do, unless you are sure it is. Provide a single "
				      "argument - the identifier for the dbus messages");
		return -1;
	}

	stringstream ss;
	int id;
	ss << argv[1];
	ss >> id;
	nxcl.setupDbus(id);

	if (-1 == (nxcl.receiveSettings())) {
		cerr << "Failed to obtain server and user for the session." << endl;
		return -1;
	}

	nxcl.startTheNXConnection();

	NXClientLib* c = nxcl.getNXClientLib();
	while (probeNXCL(c) == true) {
		usleep (30000); // 3 100ths of a second => about 30 
                                // probes a second (a lot).
	}

	nxcl.callbacks.write ("Program finished.");
	debugLogFile.close();
	return 0;
}


bool
probeNXCL (NXClientLib* lib)
{
	notQProcess* p = lib->getNXSSHProcess();

	if ((lib->getIsFinished()) == false) {
		// We need to repeatedly check if there is any output to parse.
		if (lib->getReadyForProxy() == false) {
			//p = lib->getNXSSHProcess();
			p->probeProcess();
		} else {
			//p = lib->getNXSSHProcess();
			p->probeProcess();
			p = lib->getNXProxyProcess();
			p->probeProcess();
		}
		return true;
	} else {
		return false;
	}
}
