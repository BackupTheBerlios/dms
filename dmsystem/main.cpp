/*
 *
 * Copyright (C) 2008-2010 Alexander Saal.
 * All rights reserved.
 * Contact: Alexander Saal (alex.saal@gmx.de)
 *
 * This file is part of DMSystem.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#include "base64.h"
#include "database.h"
#include "dmsystem.h"
#include "dmsystemglobal.h"
#include "connectionwizard.h"

#include <QApplication>
#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QVariant>

using namespace asaal;

int main( int argc, char **argv ) {

  QApplication app(argc, argv);
  app.setApplicationName(DMSApplicationName);
  app.setApplicationVersion(DMSSoftVersion);
  app.setOrganizationName(DMSSoftOrganization);
  app.setOrganizationDomain(DMSSoftOrganizationDomain);

  QString homeFolder = QString::null;
#if defined(Q_OS_WIN32)
  homeFolder = DMSConfigDirectory;
#else
  homeFolder = QDir::homePath();
  homeFolder.append(DMSConfigDirectory);
  QDir settingsDir(homeFolder);
  if( !settingsDir.exists() )
    settingsDir.mkpath(homeFolder);
#endif

  homeFolder.append(QString("/%1").arg(DMSDatabaseConfigFile));
  QFile settingFile(homeFolder);

#if defined(DMS_DEBUG)
  settingFile.remove();
#endif

  if( !settingFile.exists() ) {

    ConnectionWizard *wizard = new ConnectionWizard();
    if( wizard->exec() == QDialog::Accepted ) {

      delete wizard;
      wizard = 0;
    } else {

      return 0;
    }
  } else {

    QString userName = "";
    QString userPassword = "";
    QString userHost = "";
    int userPort = -1;

    QSettings *mysqlSettings = new QSettings(homeFolder, QSettings::IniFormat);
    mysqlSettings->beginGroup("Server");
    {
      userHost = mysqlSettings->value("Host").toString();
      userPort = mysqlSettings->value("Port").toInt();
    }
    mysqlSettings->endGroup();

    mysqlSettings->beginGroup("User");
    {
      userName = mysqlSettings->value("User").toString();
      userPassword = mysqlSettings->value("Password").toString();
      userPassword = QVariant(Base64::decode(userPassword)).toString();
    }
    mysqlSettings->endGroup();
    homeFolder.clear();

    Database::databaseInstance()->setDatabaseInformation(userHost, userName, userPassword, userPort);
    if( !Database::databaseInstance()->openConnection() ) {

      userName.clear();
      userPassword.clear();
      userHost.clear();
      userPort = -1;

      delete mysqlSettings;
      mysqlSettings = 0;

      QMessageBox::critical(0L, QApplication::applicationName(), Database::databaseInstance()->lastErrorMessage());

      return 0;
    }

    userName.clear();
    userPassword.clear();
    userHost.clear();
    userPort = -1;

    delete mysqlSettings;
    mysqlSettings = 0;
  }

  DMSystem *dmsystem = new DMSystem();
  dmsystem->setArguments(argc, argv);
  dmsystem->show();

  app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

  return app.exec();
}
