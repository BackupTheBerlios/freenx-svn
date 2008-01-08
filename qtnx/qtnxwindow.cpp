/***************************************************************************
                               qtnxwindow.cpp
                            -------------------
        begin                : Thursday August 3rd 2006
        copyright            : (C) 2006 by George Wright
                               (C) 2007 Defuturo Ltd
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

#include "qtnxwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QX11Info>

using namespace nxcl;
using namespace std;

QtNXWindow::QtNXWindow() : 
    QMainWindow(),
    processProbe(new QTimer()),
    sessionsDialog(NULL)
{
    nxClient.setExternalCallbacks(&callback);

    setupUI();

    QDir dir(QDir::homePath()+"/.qtnx","*.nxml");

    for (unsigned int i=0;i<dir.count();i++) {
        QString conn=dir[i];
        ui_lg.session->addItem(conn.left(conn.length()-5));
    }

    ui_lg.session->addItem(tr("Create new session"));

    connect(ui_lg.connectButton, SIGNAL(pressed()), this, SLOT(startConnect()));
    connect(ui_lg.configureButton, SIGNAL(pressed()), this, SLOT(configure()));
    connect(processProbe, SIGNAL(timeout()), this, SLOT(processProbeTimeout()));
    connect(&callback, SIGNAL(logging(QString)), this, SLOT(logStd(QString)));
    connect(&callback, SIGNAL(status(QString)), this, 
            SLOT(updateStatusBar(QString)));
    connect(&callback, SIGNAL(suspendedSessions(QList<NXResumeData>)), this,
            SLOT(loadResumeDialog(QList<NXResumeData>)));
    connect(&callback, SIGNAL(noSessions()), this, SLOT(noSessions()));
    connect(&callback, SIGNAL(progress(int, QString)), this, 
            SLOT(handleProgress(int, QString)));
}

QtNXWindow::~QtNXWindow()
{
}

void QtNXWindow::setupUI()
{
    logWindow = new QDialog(0);
    ui_lw.setupUi(logWindow);

    log = new QTextDocument();
    ui_lw.output->setDocument(log);

    loginDialog = new QWidget(this);
    menuBar = new QMenuBar(this);
    statusBar = new QStatusBar(this);

    fileMenu = new QMenu(tr("&File"));
    connectionMenu = new QMenu(tr("Conn&ection"));

    ui_lg.setupUi(loginDialog);
    setCentralWidget(loginDialog);
    setStatusBar(statusBar);
    setMenuBar(menuBar);

    statusBar->showMessage(tr("Ready"));
    statusBar->setSizeGripEnabled(false);

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(connectionMenu);

    fileMenu->addAction(tr("Quit"),
            qApp,
            SLOT(quit()),
            QKeySequence(tr("CTRL+Q")));

    connectionMenu->addAction(tr("Show log window"),
            this,
            SLOT(showLogWindow()),
            QKeySequence(tr("CTRL+L")));

    connectionMenu->addAction(tr("Connect..."),
            this,
            SLOT(startConnect()));
}

void QtNXWindow::showLogWindow()
{
    if (logWindow->isHidden())
        logWindow->show();
    else
        logWindow->hide();
}

void QtNXWindow::handleProgress(int id, QString message)
{
    switch (id) {
        case NXCL_PROCESS_STARTED:
            updateStatusBar("Process started");
            break;
        case NXCL_PROCESS_EXITED:
            updateStatusBar("Process exited");
            break;
        case NXCL_AUTH_FAILED:
            updateStatusBar("Invalid authentication key");
            break;
        case NXCL_AUTHENTICATING:
            updateStatusBar("Authenticating client");
            break;
        case NXCL_LOGIN_FAILED:
            updateStatusBar("Invalid username or password");
            failedLogin();
            break;
        case NXCL_HOST_KEY_VERIFAILED:
            updateStatusBar("Host key verification failed");
            break;
        case NXCL_INVOKE_PROXY:
            updateStatusBar("Starting NX proxy");
            break;
        case NXCL_STARTING:
            updateStatusBar("Starting session");
            break;
        case NXCL_FINISHED:
            updateStatusBar("Finished connecting");
            break;
        case NXCL_ALIVE:
            updateStatusBar("NX session active");
            break;
        case NXCL_PROCESS_ERROR:
            updateStatusBar("Process error");
            break;
        default:
            break;
    }
}

void QtNXWindow::failedLogin()
{
    QMessageBox::critical(this, tr("Authentication failure"),
            tr("You have supplied an incorrect username or password for this \
                NX server."), QMessageBox::Ok, QMessageBox::NoButton,
            QMessageBox::NoButton);

    statusBar->showMessage(tr("Login failed"));
}
void QtNXWindow::sshContinue(QString message)
{
    /*TODO: this is not functionally available in nxcl

    int reply = QMessageBox::question(this,
            tr("SSH Request"),
            message,
            QMessageBox::Yes,
            QMessageBox::No,
            QMessageBox::NoButton);

    if (reply == QMessageBox::Yes)
        nxClient.allowSSHConnect(true);
    else
        nxClient.allowSSHConnect(false);
    */
}

void QtNXWindow::startConnect()
{
    string key = "";

    NXParseXML handler;
    handler.setData(&config);

    QFile file(QDir::homePath() + "/.qtnx/" +
            ui_lg.session->currentText() + ".nxml");

    QXmlInputSource inputSource(&file);

    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    reader.setErrorHandler(&handler);
    reader.parse(inputSource);

    setDefaultData();

    if (!config.key.empty()) {
        key = config.key;
        session.key = "supplied";
    } else
        session.key = "default";

    if (config.sessionType == "unix-application")
        session.customCommand = config.customCommand;

    nxClient.setSessionData(&session);

    nxClient.invokeNXSSH("id.key", config.serverHost, config.encryption, "",
            config.serverPort);
    processProbe->start(30);

    string username = ui_lg.username->text().toStdString();
    string password = ui_lg.password->text().toStdString();

    nxClient.setUsername(username);
    nxClient.setPassword(password);
    nxClient.setResolution(getWidth(), getHeight());

    nxClient.setDepth(getDepth());
}

void QtNXWindow::setDefaultData()
{
    // These seem to be a fairly sane set of defaults for session data
    session.sessionName = config.sessionName;
    session.sessionType = config.sessionType;
    session.cache = config.cache;
    session.images = config.images;
    session.linkType = config.linkType;
    session.render = config.render;
    session.backingstore = "when_requested";
    session.imageCompressionMethod = config.imageCompressionMethod;
    session.imageCompressionLevel = config.imageCompressionLevel;
    session.geometry = config.geometry;
    session.keyboard = "defkeymap";
    session.kbtype = "pc102/defkeymap";
    session.media = config.media;
    session.agentServer = config.agentServer;
    session.agentUser = config.agentUser;
    session.agentPass = config.agentPass;
    session.cups = config.cups;
    session.fullscreen = config.fullscreen;
    session.encryption = true;
    session.virtualDesktop = false;
    session.terminate = false;
}

// This should be the only function that needs porting to other platforms
int QtNXWindow::getDepth()
{
    QX11Info info;
    return info.depth();
}

int QtNXWindow::getWidth()
{
    QDesktopWidget dw;
    return dw.screenGeometry(this).width();
}

int QtNXWindow::getHeight()
{
    QDesktopWidget dw;
    return dw.screenGeometry(this).height();
}

void QtNXWindow::updateStatusBar(QString message)
{
    statusBar->showMessage(message);
}

void QtNXWindow::processProbeTimeout()
{
    notQProcess* p = nxClient.getNXSSHProcess();

    if ((nxClient.getIsFinished()) == false) {
        if (nxClient.getReadyForProxy() == false) {
            p->probeProcess();
        } else {
            p->probeProcess();
            p = nxClient.getNXProxyProcess();
            p->probeProcess();
        }
        if (!nxClient.getSessionRunning())
            processProbe->start(30);
        else {
            processProbe->start(2000);
        }
    } else {
        processProbe->stop();
    }
}

void QtNXWindow::configure()
{
    if (ui_lg.session->currentText() == tr("Create new session"))
        settingsDialog = new QtNXSettings("");
    else
        settingsDialog = new QtNXSettings(ui_lg.session->currentText());

    connect(settingsDialog, SIGNAL(closing()), this, SLOT(configureClosed()));

    settingsDialog->show();
}

void QtNXWindow::configureClosed()
{
    while (ui_lg.session->count() != 0) {
        ui_lg.session->removeItem(0);
    }

    QDir dir(QDir::homePath()+"/.qtnx","*.nxml");
    for (unsigned int i=0;i<dir.count();i++) {
        QString conn=dir[i];
        ui_lg.session->addItem(conn.left(conn.length()-5));
    }
    ui_lg.session->addItem(tr("Create new session"));
}

void QtNXWindow::loadResumeDialog(QList<NXResumeData> data)
{
    delete sessionsDialog;
    sessionsDialog = new QtNXSessions(data);
    sessionsDialog->show();

    connect(sessionsDialog, SIGNAL(newPressed()), this, SLOT(resumeNewPressed()));
    connect(sessionsDialog, SIGNAL(resumePressed(QString)),
            this, SLOT(resumeResumePressed(QString)));
}

void QtNXWindow::resumeNewPressed()
{
    nxClient.setSessionData(&session);
    nxClient.runSession();
}

void QtNXWindow::resumeResumePressed(QString id)
{
    session.id = id.toStdString();
    session.suspended = true;
    nxClient.setSessionData(&session);
    nxClient.runSession();
}

void QtNXWindow::noSessions()
{
    session.suspended = false;
    nxClient.setSessionData(&session);
    nxClient.runSession();
}

void QtNXWindow::logStd(QString message)
{
    if (message.right(1) != "\n")
        cout << message.toStdString() << endl;
    else
        cout << message.toStdString();

    log->setPlainText(log->toPlainText() + message);
}
