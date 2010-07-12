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

#include <QByteArray>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QStringList>
#include <QUuid>
#include <QVariant>

using namespace asaal;

static QSqlDatabase mCurrentDatabase;
static QString mDatabaseUserName = "DMS";
static QString mDatabaseUserPassword = QString::null;
static QString mDatabaseName = QString::null;
static QString mDatabaseHost = QString::null;
static int mDatabasePort = 3306;

Database *mDatabaseInstance = NULL;
Database::Database( QObject *parent )
  : QObject( parent ),
    mLastErrorMessage(""),
    mConnectionIsAvailable(false) {

  mDatabaseInstance = this;
}

Database *Database::databaseInstance() {

  if( !mDatabaseInstance )
    mDatabaseInstance = new Database();
  return mDatabaseInstance;
}

void Database::setDatabaseInformation( const int port, const QString &host, const QString &user, const QString &userPassword ) {

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

  bool connectionClosed = false;
  if( mConnectionIsAvailable ) {
    mCurrentDatabase.commit();
    mCurrentDatabase.close();
    mConnectionIsAvailable = false;
    connectionClosed = true;
  }

  return connectionClosed;
}

bool Database::login( const User *user ) {

  bool loggedIn = false;
  if( user && mConnectionIsAvailable ) {

    mLastErrorMessage.clear();

    QSqlQuery loginQuery(mCurrentDatabase);
    loginQuery.exec(QString("SELECT USERPASSWD FROM USERS WHERE UID = '%1'").arg(user->mId));
    if( loginQuery.isActive() ) {

      while( loginQuery.next() ) {

        if( loginQuery.value(0).toString() == Base64::encode(QVariant(user->mPassword).toByteArray()) ) {

          loginQuery.clear();
          loginQuery.exec(QString("UPDATE USERS SET LOGGEDIN = 1 WHERE UID = '%1'").arg(user->mId));
          {
            if( loginQuery.isActive() ) {

              if( mCurrentUser )
                delete mCurrentUser;
              mCurrentUser = NULL;
              mCurrentUser = const_cast<User *>(user);

              loggedIn = true;
            }
            else {
              loggedIn = false;
              mLastErrorMessage = loginQuery.lastError().text();
            }
          }
          loginQuery.clear();
        }

        break;
      }
    }
    else
      mLastErrorMessage = loginQuery.lastError().text();
  }

  return loggedIn;
}

bool Database::logout() {

  bool loggedOut = false;
  if( mCurrentUser && mConnectionIsAvailable ) {

    mLastErrorMessage.clear();

    QSqlQuery logoutQuery(mCurrentDatabase);
    logoutQuery.exec(QString("UPDATE USERS SET LOGGEDIN = 0 WHERE UID = '%1'").arg(mCurrentUser->mId));
    {
      if( logoutQuery.isActive() ) {
        
        if( mCurrentUser )
          delete mCurrentUser;
        mCurrentUser = NULL;
        
        loggedOut = true;
      }
      else {
        loggedOut = false;
        mLastErrorMessage = logoutQuery.lastError().text();
      }
    }
    logoutQuery.clear();
  }

  return loggedOut;
}

void Database::createUser( const User *user ) {

  if( user && mConnectionIsAvailable ) {

    mLastErrorMessage.clear();

    QString dateTime = QDateTime::currentDateTime().toString( Qt::ISODate );
    QString newUserStatement = QString("INSERT INTO USERS(UID, USERNAME, USERPASSWD, LOGGEDIN, CREATED, UPDATED)"
                                       "VALUES ('%1', '%2', '%3', '%4', '%5', '%6')")
                                      .arg(createUniqueId())
                                      .arg(user->mName)
                                      .arg(QVariant(Base64::decode(user->mPassword)).toString())
                                      .arg(0)
                                      .arg(dateTime)
                                      .arg(dateTime);

    QSqlQuery newUserQuery(mCurrentDatabase);
    newUserQuery.exec(newUserStatement);
    if( newUserQuery.isActive() ) {

      newUserStatement.clear();
      newUserStatement = QString::null;

      dateTime.clear();
      dateTime = QString::null;

      newUserQuery.clear();
    }
    else
      mLastErrorMessage = newUserQuery.lastError().text();
  }
}

const QList<User *> Database::users() {

  User *user = 0;
  QList<User *> userList;
  if( mConnectionIsAvailable ) {

    mLastErrorMessage.clear();

    QSqlQuery userQuery(mCurrentDatabase);
    userQuery.exec("SELECT UID, USERNAME, USERPASSWD FROM USERS ORDER BY USERNAME");
    if( userQuery.isActive() ) {

      while( userQuery.next() ) {

        user = new User;
        user->mId = userQuery.value(0).toString();
        user->mName = userQuery.value(1).toString();
        user->mPassword = Base64::encode(QVariant(userQuery.value(2).toString()).toByteArray());
        userList.append(user);
      }
      userQuery.clear();
    }
    else
      mLastErrorMessage = userQuery.lastError().text();
  }

  return userList;
}

void Database::createDocument( const Document *doc ) {
}

const QList<Document *> Database::documents() {

  return QList<Document *>();
}

void Database::createGroup( const Groups *group ) {
}

const QList<Groups *> Database::groups() {

  return QList<Groups *>();
}

void Database::addDocumentToGroup( const QString &documentId, const QString &groupId ) {
}

void Database::addDocumentToUser( const QString &documentId, const QString &userId ) {
}

void Database::initializeDatabase() {
}

const QString Database::createUniqueId() const {

  QString uniqueId = QUuid::createUuid().toString();
  uniqueId = uniqueId.replace("{", "");
  uniqueId = uniqueId.replace("}", "");
  uniqueId = uniqueId.replace("-", "");
  return uniqueId;
}
