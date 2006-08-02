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
	lib.setXRes(640);
	lib.setYRes(480);
	lib.setDepth(24);
	lib.setRender(true);
	return qApp->exec();
}
