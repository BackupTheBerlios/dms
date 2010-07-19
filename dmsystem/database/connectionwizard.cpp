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
#include "connectionwizard.h"
#include "database.h"
#include "databaseuserdetails.h"
#include "dmsystemglobal.h"

#include <QAbstractButton>
#include <QApplication>
#include <QCloseEvent>
#include <QDir>
#include <QByteArray>
#include <QDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTreeWidget>
#include <QVariant>

using namespace asaal;

static bool mConnectionEstablished = false;

ConnectionWizard::ConnectionWizard( QWidget *parent, Qt::WindowFlags flags )
  : QWizard(parent, flags) {

  setupUi(this);

  setPixmap(QWizard::BackgroundPixmap, QPixmap(":/wizard"));

  connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(slotCurrentIdChanged(int)));

  connect(mButtonTestConnection, SIGNAL(clicked()), this, SLOT(slotTestConnection()));
  connect(mToolButtonAddUser, SIGNAL(clicked()), this, SLOT(slotAddUser()));
  connect(mToolButtonRemoveUser, SIGNAL(clicked()), this, SLOT(slotRemoveUser()));

  page(FinalPage)->setFinalPage(true);

  setFiniheButtonEnabled(false);
  setNextButtonEnabled(false);
}

ConnectionWizard::~ConnectionWizard() {
}

void ConnectionWizard::closeEvent( QCloseEvent *event ) {

  Q_UNUSED(event)
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
  QString userPassword = Base64::encode(QVariant(mLineEditDatabaseUserPassword->text()).toByteArray());

  mysqlSettings->beginGroup(DMSServerSection);
  mysqlSettings->setValue(DMSServerHostKey, mLineEditHost->text());
  mysqlSettings->setValue(DMSServerPortKey, mSpinBoxPort->value());
  mysqlSettings->endGroup();

  mysqlSettings->beginGroup(DMSServerUserSection);
  mysqlSettings->setValue(DMSServerUserKey, mLineEditDatabaseUsername->text());
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

void ConnectionWizard::setFiniheButtonEnabled( bool enabled ) {

  button(QWizard::FinishButton)->setEnabled(enabled);
}

void ConnectionWizard::setNextButtonEnabled( bool enabled ) {

  button(QWizard::NextButton)->setEnabled(enabled);
}

void	ConnectionWizard::slotCurrentIdChanged( int id ) {

  if( !mConnectionEstablished )
    setFiniheButtonEnabled(false);

  if( id == SetupPage && !mConnectionEstablished  )
    setNextButtonEnabled(false);
  else if( id == SetupPage && mConnectionEstablished )
    setNextButtonEnabled(true);
  else if( id == FinalPage && mTreeWidgetUsers->topLevelItemCount() <= 0 )
    setFiniheButtonEnabled(false);
  else if( id == FinalPage && mTreeWidgetUsers->topLevelItemCount() >= 1 )
    setFiniheButtonEnabled(true);
}

void ConnectionWizard::slotTestConnection() {

  mButtonTestConnection->setEnabled(false);

  QString userName = mLineEditDatabaseUsername->text();
  QString userPassword = mLineEditDatabaseUserPassword->text();
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

    setNextButtonEnabled(true);
    mButtonTestConnection->setEnabled(false);
    mLabelConnectionInfo->setText("<html><body><font color=\"green\">" + tr("Connection established ...") + "</font></body></html>");
    mLabelConnectionInfo->update();
  }
  else {

    setNextButtonEnabled(false);
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

void ConnectionWizard::slotAddUser() {

  if( mLineEditGeneralUsername->text().isEmpty() || mLineEditGeneralUsername->text().isNull() ) {

    QMessageBox::critical(this, QApplication::applicationName(), tr("No user name was entered."));
    return;
  }

  if( mLineEditGeneralUserPassword->text().isEmpty() || mLineEditGeneralUserPassword->text().isNull() ) {

    QMessageBox::critical(this, QApplication::applicationName(), tr("No user password was entered."));
    return;
  }

  if( mLineEditGeneralUserPassword->text() == mLineEditGeneralUserPasswordConfirm->text() ) {

    DatabaseUserDetails *dud = new DatabaseUserDetails(mLineEditGeneralUsername->text(),
                                                       mLineEditGeneralUserPassword->text(),
                                                       this);
    if( dud->exec() == QDialog::Accepted ) {

      User *userDetails = const_cast<User *>(dud->databaseUserDetails());
      if( userDetails ) {

        QTreeWidgetItem *userItem = new QTreeWidgetItem();
        userItem->setText(0, userDetails->mName);
        if( userDetails->mUserData && !userDetails->mUserData->isEmpty() ) {

          userItem->setText(1, userDetails->mUserData->mFirstName);
          userItem->setText(2, userDetails->mUserData->mLastName);
          userItem->setText(3, userDetails->mUserData->mStreet);
          userItem->setText(4, userDetails->mUserData->mStreetNumber);
          userItem->setText(5, userDetails->mUserData->mCity);
          userItem->setText(6, userDetails->mUserData->mPostalCode);
          userItem->setText(7, userDetails->mUserData->mCountry);
          userItem->setText(8, userDetails->mUserData->mEMail);
        }
        mTreeWidgetUsers->addTopLevelItem(userItem);

        Database::databaseInstance()->beginTransaction();
        Database::databaseInstance()->createUser(userDetails);

        QString databaseMessage = Database::databaseInstance()->lastErrorMessage();
        if( !databaseMessage.isEmpty() || !databaseMessage.isNull() ) {

          setFiniheButtonEnabled(false);
          Database::databaseInstance()->rollback();
          QMessageBox::critical(this, QApplication::applicationName(), databaseMessage);
        }
        else {

          setFiniheButtonEnabled(true);
          Database::databaseInstance()->commit();
        }
        databaseMessage.clear();

        if( userDetails->mUserData )
          delete userDetails->mUserData;
        userDetails->mUserData = 0;

        delete userDetails;
        userDetails = 0;

        delete dud;
        dud = 0;
      }
    }
    else {

      delete dud;
      dud = 0;
    }
  } else {

    QMessageBox::critical(this, QApplication::applicationName(), tr("The password you entered does not match."));
  }

  setFiniheButtonEnabled(true);
}

void ConnectionWizard::slotRemoveUser() {

  if( mTreeWidgetUsers->topLevelItemCount() <= 0 )
    setFiniheButtonEnabled(false);
  else
    setFiniheButtonEnabled(true);
}
