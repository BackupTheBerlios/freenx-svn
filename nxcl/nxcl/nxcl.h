/* -*-c++-*- */
/*
 * nxcl: The NXCL dbus daemon.
 *
 * See main.cpp for general notes.
 *
 * This file contains the definition of the Nxcl class used by the
 * nxcl program.
 *
 * (C) 2007 Embedded Software Foundry Ltd. (U.K.)
 * Author: Sebastian James
 * Email:  seb@esfnet.co.uk
 *
 * Released under the terms of the GNU GPL version 2.
 */

#ifndef _NXCL_H_
#define _NXCL_H_ 1

#include "nxdata.h"
#include "../lib/nxclientlib.h"

/* This define is required for slightly older versions of dbus as
 * found, for example, in Ubuntu 6.06. */
#define DBUS_API_SUBJECT_TO_CHANGE 1
extern "C" {
#include <dbus/dbus.h>
}

using namespace std;

namespace nxcl 
{

	class NxclBase
	{
	public:
		NxclBase() {}
		virtual ~NxclBase() {}

		virtual void haveResumableSessions (list<NXResumeData> resumeData) {}
		virtual void noResumableSessions (void) {}
		virtual void serverCapacityReached (void) {}
	};

	class NxclCallbacks : public NXClientLibExternalCallbacks
	{
	public:
		NxclCallbacks ();
		~NxclCallbacks ();

		/*!
		 * Send a message to the user. Here, this is
		 * implemented as a message to stdout. It could be
		 * (any may yet be) send on the dbus interface
		 */
		void write (string msg);
		/*!
		 * Send a debugging message out. Here that's
		 * implemented as a message on stdout.
		 */
		void debug (string msg);
		/*!
		 * This is called when some data comes from the stdout
		 * of the nxssh process. This implementation sends a
		 * copy of that data to stdout of nxcl, for debugging
		 * purposes (and only when DEBUG==1,
		 * i.e. --enable-debug-output was given as an option
		 * to the configure script.
		 */
		void stdoutSignal (string msg);
		/*!
		 * Outputs a copy of stderr output from the nxssh
		 * process.
		 */
		void stderrSignal (string msg);
		/*!
		 * Outputs a copy of the stdin input sent to the nxssh
		 * process to the stdout of nxcl for debugging
		 * purposes.
		 */
		void stdinSignal (string msg);
		/*!
		 * Sends the list of resumable sessions (resumeData)
		 * to the client of the nxcl program via a dbus
		 * message.
		 */
		void resumeSessionsSignal (list<NXResumeData> resumeData);		
		/*!
		 * Tells nxcl that there are no suspended sessions and
		 * that connection is proceeding with a new
		 * connection. This fact is sent to the client of nxcl
		 * as a dbus message.
		 */
		void noSessionsSignal (void);
		/*!
		 * This callback sends a message via dbus to the
		 * client of the nxcl program saying "the server has
		 * no more capacity". This basically means that there
		 * are not enough licences to allow the connection,
		 * but it may also occur if there is any sort of RAM
		 * limit checking in nxserver (I don't know if this is
		 * true).
		 */
		void serverCapacitySignal (void);

		/*!
		 * Accessor function to set a pointer to the parent Nxcl object.
		 */
		void setParent (NxclBase * p) { this->parent = p; }
	private:
		NxclBase * parent;

	};

	class Nxcl : public NxclBase
	{
	public:
		Nxcl();
		Nxcl(int n);
		~Nxcl();

		// Public Methods
		/*!
		 * \brief Sets up a connection to the dbus daemon, based on
		 * the member attribute \see dbusNum. 
		 *
		 * Sets the connection up to listen for messages on
		 * the interface org.freenx.nxcl.clientX where X
		 * starts at 0 and the first available number is taken
		 * (if another nxcl is running with
		 * org.freenx.nxcl.client0, then 1 is
		 * chosen). Messages are sent out on the interface
		 * org.freenx.nxcl.nxclX
		 */
		void setupDbus (void);
		/*!
		 * \brief Sets dbusNum to be \arg id, then calls \see
		 * setupDbus().
		 *
		 * \param id The number to use to set up the dbus
		 * connection (used as a suffix for interface names).
		 */
		void setupDbus (int id);
		/*!
		 * \brief Wait for a dbus message containing session settings.
		 *
		 * This waits for a dbus signal message called
		 * 'sessionConfig' to come in on the listen interface,
		 * then sets \see sessionData based on its contents.
		 *
		 * \return 0 if settings received ok, -1 if we didn't
		 * receive at least the nxserver host and the nxuser
		 * name.
		 */
		int receiveSettings (void);
		/*!
		 * \brief Set default values for \see sessionData
		 */
		void setSessionDefaults (void);
		/*!
		 * \brief Configure the \see nxclientlib object and
		 * then call its \see invokeSSH method.
		 *
		 * This also calls nxclientlib.setResolution to detect
		 * the size of your screen (that's why nxcl links to X
		 * libraries).
		 */
		void startTheNXConnection (void);

		// Accessors
		//@{
		NXClientLib* getNXClientLib (void) { return &(this->nxclientlib); }
		//@}

		// Public Slots
		//@{
		/*!
		 * Called by \see NxclCallbacks::resumeSessionsSignal
		 */
		void haveResumableSessions (list<NXResumeData> resumable);
		/*!
		 * Called by \see NxclCallbacks::noSessionsSignal
		 */
		void noResumableSessions (void);
		/*!
		 * This _should_ send a dbus message to the client of
		 * nxcl, saying "You've never connected to this
		 * (NX)SSH server before, do you wish to
		 * continue?"
		 *
		 * For now, it just sets
		 * this->nxclientlib.getSession()->setContinue (true);
		 */
		void requestConfirmation (string msg);
		/*!
		 * Called by \see NxclCallbacks::serverCapacitySignal
		 */
		void serverCapacityReached (void);
		//@}

		/*!
		 * Public pointer to the callback functions
		 */
		NxclCallbacks callbacks;

	private:
		/*!
		 * \brief Set up data structures, determine display height and width.
		 *
		 * This is called by the constructors.
		 */
		void initiate (void);
		/*!
		 * Send the resumable sessions as dbus messages. One
		 * message (called "AvailableSession") is sent for
		 * each resumable session available on the NX
		 * server. When they have all been sent, a message is
		 * sent called "NoMoreAvailable".
		 *
		 * Called by \see haveResumableSessions.
		 *
		 * \param resumable a list of resumable sessions to be
		 * sent on the dbus interface.
		 *
		 */
		void sendResumeList (list<NXResumeData>& resumable);
		/*!
		 * Wait for a message on the dbus interface to tell us
		 * which of the resumable sessions available on the NX
		 * server we should start. Call
		 * nxclientlib.chooseResumable() or
		 * nxclientlib.terminateSession based on the contents
		 * of the received message.
		 *
		 * Called by \see haveResumableSessions.
		 */
		void receiveStartInstruction (void);

		/*!
		 * The nxclientlib object whose methods will negotiate
		 * the NX session.
		 */
		NXClientLib nxclientlib;
		/*!
		 * We populate this data structure before passing the
		 * information to the nxclientlib object so it can
		 * start up the desired connection.
		 */
		NXSessionData sessionData;
		/*!
		 * The connection to the dbus daemon.
		 */
		DBusConnection *conn;
		/*!
		 * Used when calling dbus functions.
		 */
		DBusError error;
		/*!
		 * Used to make our dbus interfaces unique. This
		 * number is appended to the dbus interface names.
		 */
		int dbusNum;
		/*!
		 * The name to use for our connection to DBUS.
		 */
		string dbusName;
		/*!
		 * Match messages from this DBUS source.
		 */
		string dbusMatch;
		/*!
		 * The DBUS interface which we will listen to.
		 */
		string dbusMatchInterface;
		/*!
		 * The DBUS interface we'll send on.
		 */
		string dbusSendInterface;
		/*!
		 * Holds the username for the connection to the NX
		 * Server.
		 */
		string nxuser;
		/*!
		 * Holds the password for the connection to the NX
		 * Server.
		 */
		string nxpass;
		/*!
		 * Holds the ip/hostname of the NX Server to which we
		 * will try to connect.
		 */
		string nxserver;
		/*!
		 * Holds the port on the NX Server to which we will
		 * try to connect.
		 */
		int nxport;
		/*!
		 * Width of the screen in pixels.
		 */
		int xRes;
		/*!
		 * Height of the screen in pixels.
		 */
		int yRes;
		/*!
		 * Colour depth of the screen.
		 */
		int displayDepth;
	};

} // namespace

#endif // ifndef _NXCL_H_
