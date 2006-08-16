/***************************************************************************
                               qtnxsettings.h
                             -------------------
    begin                : Saturday August 12th 2006
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

#ifndef _QTNXSETTINGS_H_
#define _QTNXSETTINGS_H_

#include <QDialog>

#include "nxdata.h"

#include "ui_settingsdialog.h"

class QtNXSettings : public QDialog
{
	Q_OBJECT
	public:
		QtNXSettings(QString);
		~QtNXSettings();
		void parseFile();
	private:
		Ui::SettingsDialog ui_sd;
		NXConfigData config;
		QString fileName;

};

#endif
