#include <QCoreApplication>

#include "nxclientlib.h"

class Callback : public NXCallback
{
	public:
		virtual void write(std::string *msg) {};
		virtual void sshRequestAuthenticity(std::string *msg) { m_lib->allowSSHConnect(true); };

		virtual void authenticationFailed() {};

		// Stdout/stderr from the nxssh process
		virtual void stdout(std::string *msg) {};
		virtual void stderr(std::string *msg) {};
		// Stdin sent to the nxssh process
		virtual void stdin(std::string *msg) {};
		virtual void resumeSessions(std::vector<NXResumeData> *sessions) {};

		void setClientLib(NXClientLib *lib) { m_lib = lib; };
	private:
		NXClientLib *m_lib;
};

int main(int argc, char **argv)
{
	Callback *cb = new Callback;
	QCoreApplication *qApp = new QCoreApplication(argc, argv);
	NXClientLib lib(qApp);

	cb->setClientLib(&lib);

	lib.setCallback(cb);
	lib.invokeNXSSH("default" ,argv[1], true);
	lib.setUsername(argv[2]);
	lib.setPassword(argv[3]);
	lib.setResolution(640,480);
	lib.setDepth(24);
	lib.setRender(true);

	NXSessionData session;

	// HARDCODED TEST CASE
	session.sessionName = "TEST";
	session.sessionType = "unix-kde";
	session.cache = 8;
	session.images = 32;
	session.linkType = "adsl";
	session.render = true;
	session.backingstore = "when_requested";
	session.imageCompressionMethod = 2;
	// session.imageCompressionLevel;
	session.geometry = "800x600+0+0";
	session.keyboard = "defkeymap";
	session.kbtype = "pc102/defkeymap";
	session.media = false;
	session.agentServer = "";
	session.agentUser = "";
	session.agentPass = "";
	session.screenInfo = "800x600x24+render";
	session.cups = 0;
	session.suspended = false;

	lib.setSession(session);
	return qApp->exec();
}
