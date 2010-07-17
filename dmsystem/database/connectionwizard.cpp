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

#include "dmsystemglobal.h"
#include "connectionwizard.h"
#include "base64.h"
#include "database.h"

#include <QAbstractButton>
#include <QApplication>
#include <QCloseEvent>
#include <QDir>
#include <QByteArray>
#include <QDialog>
#include <QMessageBox>
#include <QSettings>
#include <QVariant>

using namespace asaal;

static bool mConnectionEstablished = false;

ConnectionWizard::ConnectionWizard( QWidget *parent, Qt::WindowFlags flags )
  : QWizard(parent, flags) {

  setupUi(this);

  connect(mButtonTestConnection, SIGNAL(clicked()), this, SLOT(slotTestConnection()));
  connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(slotCurrentIdChanged(int)));

  setFiniheButtonEnabled(false);
  setPixmap(QWizard::BackgroundPixmap, QPixmap(":/wizard"));
}

ConnectionWizard::~ConnectionWizard() {
}

void ConnectionWizard::closeEvent( QCloseEvent *event ) {

  QDialog::reject();
}

void ConnectionWizard::accept() {

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

  QSettings *mysqlSettings = new QSettings(homeFolder, QSettings::IniFormat, this);
  QString userPassword = Base64::encode(QVariant(mLineEditPassword->text()).toByteArray());

  mysqlSettings->beginGroup(DMSServerSection);
  mysqlSettings->setValue(DMSServerHostKey, mLineEditHost->text());
  mysqlSettings->setValue(DMSServerPortKey, mSpinBoxPort->value());
  mysqlSettings->endGroup();

  mysqlSettings->beginGroup(DMSServerUserSection);
  mysqlSettings->setValue(DMSServerUserKey, mLineEditUsername->text());
  mysqlSettings->setValue(DMSServerPasswordKey, userPassword);
  mysqlSettings->endGroup();
  mysqlSettings->sync();

  userPassword.clear();
  homeFolder.clear();

  delete mysqlSettings;
  mysqlSettings = 0;

  QDialog::accept();
}

void ConnectionWizard::reject() {

  QDialog::reject();
}

void	ConnectionWizard::slotCurrentIdChanged( int id ) {

  Q_UNUSED(id)

  if( !mConnectionEstablished )
    setFiniheButtonEnabled(false);
}

void ConnectionWizard::setFiniheButtonEnabled( bool enabled ) {

  button(QWizard::FinishButton)->setEnabled(enabled);
}

void ConnectionWizard::slotTestConnection() {

  mButtonTestConnection->setEnabled(false);

  QString userName = mLineEditUsername->text();
  QString userPassword = mLineEditPassword->text();
  QString userHost = mLineEditHost->text();
  int userPort = mSpinBoxPort->value();

  if( userName.isEmpty() || userName.isNull() ) {

    mButtonTestConnection->setEnabled(true);
    QMessageBox::critical(this, QApplication::applicationName(), tr("Enter a valid user name."));
    return;
  }

  if( userHost.isEmpty() || userHost.isNull() ) {

    mButtonTestConnection->setEnabled(true);
    QMessageBox::critical(this, QApplication::applicationName(), tr("Enter a valid host name or a TCP/IP address."));
    return;
  }

  Database::databaseInstance()->setDatabaseInformation(userHost, userName, userPassword, userPort);
  mConnectionEstablished = Database::databaseInstance()->openConnection();
  if( mConnectionEstablished ) {

    setFiniheButtonEnabled(true);
    mButtonTestConnection->setEnabled(false);
    mLabelConnectionInfo->setText("<html><body><font color=\"green\">" + tr("Connection established ...") + "</font></body></html>");
    mLabelConnectionInfo->update();
  }
  else {

    setFiniheButtonEnabled(false);
    mLabelConnectionInfo->setText("");
    mLabelConnectionInfo->update();
    mButtonTestConnection->setEnabled(true);
    QMessageBox::critical(this, QApplication::applicationName(), Database::databaseInstance()->lastErrorMessage());
  }

  userName.clear();
  userPassword.clear();
  userHost.clear();
  userPort = -1;
}
