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

#include "connectionwizard.h"
#include "base64.h"
#include "database.h"

#include <QAbstractButton>
#include <QApplication>
#include <QCloseEvent>
#include <QByteArray>
#include <QDialog>
#include <QMessageBox>
#include <QSettings>
#include <QVariant>

using namespace asaal;

ConnectionWizard::ConnectionWizard( QWidget *parent, Qt::WindowFlags flags )
  : QWizard(parent, flags) {

  setupUi(this);

  connect(mButtonTestConnection, SIGNAL(clicked()), this, SLOT(slotTestConnection()));

  mFinishButton = button(QWizard::FinishButton);
  mFinishButton->setEnabled(false);
}

ConnectionWizard::~ConnectionWizard() {

  delete mFinishButton;
}

void ConnectionWizard::closeEvent( QCloseEvent *event ) {

  QDialog::reject();
}

void ConnectionWizard::accept() {

  QString applicationFolder = QApplication::applicationDirPath();
  applicationFolder.append("/mysql.dms");

  QSettings *mysqlSettings = new QSettings(applicationFolder, QSettings::IniFormat, this);
  QString userPassword = Base64::encode(QVariant(mLineEditPassword->text()).toByteArray());

  mysqlSettings->beginGroup("Server");
  mysqlSettings->setValue("Host", mLineEditHost->text());
  mysqlSettings->setValue("Port", mSpinBoxPort->value());
  mysqlSettings->endGroup();

  mysqlSettings->beginGroup("User");
  mysqlSettings->setValue("User", mLineEditUsername->text());
  mysqlSettings->setValue("Password", userPassword);
  mysqlSettings->endGroup();
  mysqlSettings->sync();

  delete mysqlSettings;
  mysqlSettings = 0;

  userPassword.clear();
  applicationFolder.clear();

  QDialog::accept();
}

void ConnectionWizard::reject() {

  QDialog::reject();
}

void ConnectionWizard::slotTestConnection() {

  QString userName = mLineEditUsername->text();
  QString userPassword = mLineEditPassword->text();
  QString userHost = mLineEditHost->text();
  int userPort = mSpinBoxPort->value();

  if( userName.isEmpty() || userName.isNull() ) {

    QMessageBox::critical(this, QApplication::applicationName(), tr("Enter a valid user name."));
    return;
  }

  if( userHost.isEmpty() || userHost.isNull() )
    userHost = "localhost";

  Database::databaseInstance()->setDatabaseInformation(userHost, userName, userPassword, userPort);
  if( Database::databaseInstance()->openConnection() ) {

    mFinishButton->setEnabled(true);
  }
  else {

    mFinishButton->setEnabled(false);
    QMessageBox::critical(this, QApplication::applicationName(), Database::databaseInstance()->lastErrorMessage());
  }

  userName.clear();
  userPassword.clear();
  userHost.clear();
  userPort = -1;
}
