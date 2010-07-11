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

#include "database.h"

#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QStringList>

using namespace asaal;

Database *mDatabaseInstance = NULL;
static QSqlDatabase mCurrentDatabase;

static QString mDatabaseUserName;
static QString mDatabaseUserPassword;
static QString mDatabaseName;
static QString mDatabaseHost;
static int mDatabasePort;

Database::Database( QObject *parent )
  : QObject( parent ),
    mLastErrorMessage(""),
    mConnectionIsAvailable(false) {
      
  mDatabaseUserName = "";
  mDatabaseUserPassword = "";
  mDatabaseName = "";
  mDatabaseHost = "";
  mDatabasePort = 3306;

  mDatabaseInstance = this;
}

Database *Database::databaseInstance() {

  if( !mDatabaseInstance )
    mDatabaseInstance = new Database();
  return mDatabaseInstance;
}

void Database::setDatabaseInformation( const int port, const QString &host, QString &database, const QString &user, const QString &userPassword ) {

  mDatabaseUserName = database;
  mDatabaseUserPassword = userPassword;
  mDatabaseName = user;
  mDatabaseHost = host;

  if( port < 0 )
    mDatabasePort = 3306;
  else
    mDatabasePort = port;
}

bool Database::createConnection( const DatabaseType type ) {

  switch( type ) {
    case MySQL:

      mLastErrorMessage.clear();

      mCurrentDatabase = QSqlDatabase::addDatabase("QMYSQL");
      mCurrentDatabase.setHostName(mDatabaseHost);
      mCurrentDatabase.setPort(mDatabasePort);
      mCurrentDatabase.setDatabaseName(mDatabaseName);
      mCurrentDatabase.setUserName(mDatabaseUserName);
      mCurrentDatabase.setPassword(mDatabaseUserPassword);

      mConnectionIsAvailable = mCurrentDatabase.open();
      if( !mConnectionIsAvailable )
        mLastErrorMessage = mCurrentDatabase.lastError().text();

      initializeDatabase();

      break;
    case SQLite3:

      mLastErrorMessage.clear();
      mLastErrorMessage = tr("SQLite 3 connection not supported at this time.");

      mConnectionIsAvailable = false;
      return mConnectionIsAvailable;
  }

  return mConnectionIsAvailable;
}

bool Database::closeConnection() {

  return mConnectionIsAvailable;
}

bool Database::login( const User *user ) {

  return mConnectionIsAvailable;
}

bool Database::logout() {

  return mConnectionIsAvailable;
}

void Database::createUser( const User *user ) {
}

const QList<User *> Database::users() const {

  return QList<User *>();
}

void Database::createDocument( const Document *doc ) {
}

const QList<Document *> Database::documents() const {

  return QList<Document *>();
}

void Database::createGroup( const Groups *group ) {
}

const QList<Groups *> Database::groups() const {

  return QList<Groups *>();
}

void Database::addDocumentToGroup( const QString &documentId, const QString &groupId ) {
}

void Database::addDocumentToUser( const QString &documentId, const QString &userId ) {
}

void Database::initializeDatabase() {
}