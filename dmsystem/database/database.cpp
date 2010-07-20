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
#include <QFile>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QUuid>
#include <QVariant>

#if defined(DMS_DEBUG)
#include <QDebug>
#endif

using namespace asaal;

static QSqlDatabase mCurrentDatabase;
static QString mDatabaseUserName = QString::null;
static QString mDatabaseUserPassword = QString::null;
static QString mDatabaseName = "DMS";
static QString mDatabaseHost = QString::null;
static int mDatabasePort = 3306;
static Database::DatabaseType mDatabaseType = Database::MySQL;

Database *mDatabaseInstance = NULL;
Database::Database( QObject *parent )
  : QObject( parent ),
    mCurrentUser(0),
    mLastErrorMessage(""),
    mConnectionIsAvailable(false) {

  mDatabaseInstance = this;
}

Database *Database::databaseInstance() {

  if( !mDatabaseInstance )
    mDatabaseInstance = new Database();
  return mDatabaseInstance;
}

void Database::setDatabaseInformation( const QString &host, const QString &user, const QString &userPassword, const int port, const DatabaseType type ) {

  mDatabaseHost = host;
  mDatabaseUserName = user;
  mDatabaseUserPassword = userPassword;
  mDatabaseType = type;

  if( port < 0 )
    mDatabasePort = 3306;
  else
    mDatabasePort = port;
}

bool Database::openConnection() {

  switch( mDatabaseType ) {
    case MySQL:

      mLastErrorMessage.clear();
      mCurrentDatabase.close();

      mCurrentDatabase = QSqlDatabase::addDatabase("QMYSQL");
      mCurrentDatabase.setHostName(mDatabaseHost);
      mCurrentDatabase.setPort(mDatabasePort);
      mCurrentDatabase.setUserName(mDatabaseUserName);
      mCurrentDatabase.setPassword(mDatabaseUserPassword);

      mConnectionIsAvailable = mCurrentDatabase.open();
      if( !mConnectionIsAvailable )
        mLastErrorMessage = mCurrentDatabase.lastError().text();
      else
        mConnectionIsAvailable &= initializeDatabase();

      break;
    case MSSQL:
      mLastErrorMessage.clear();
      mLastErrorMessage = tr("MS SQL connection not supported at this time.");

      mConnectionIsAvailable = false;
      break;
    case SQLite3:

      mLastErrorMessage.clear();
      mLastErrorMessage = tr("SQLite 3 connection not supported at this time.");

      mConnectionIsAvailable = false;
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

bool Database::beginTransaction() {

  if( mConnectionIsAvailable )
    return mCurrentDatabase.transaction();
  return false;
}

bool Database::rollback() {

  if( mConnectionIsAvailable )
    return mCurrentDatabase.rollback();
  return false;
}

bool Database::commit() {

  if( mConnectionIsAvailable )
    return mCurrentDatabase.commit();
  return false;
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

    QString userId = createUniqueId();
    QString dateTime = QDateTime::currentDateTime().toString(Qt::ISODate);
    QString newUserStatement = QString("INSERT INTO USERS(UID, USERNAME, USERPASSWD, LOGGEDIN, CREATED, UPDATED)"
                                       "VALUES('%1', '%2', '%3', '%4', '%5', '%6')")
                                      .arg(userId)
                                      .arg(user->mName)
                                      .arg(Base64::encode(QVariant(user->mPassword).toByteArray()))
                                      .arg(0)
                                      .arg(dateTime)
                                      .arg(dateTime);

    QSqlQuery newUserQuery(mCurrentDatabase);
    newUserQuery.exec(newUserStatement);
    if( newUserQuery.isActive() ) {

      if( user->mUserData && !user->mUserData->isEmpty() ) {

        newUserQuery.clear();
        newUserStatement = QString("INSERT INTO USERSDATA(UDID, UID, FNAME, LNAME, STREETNAME, STREETNR, CITY, ZIPCODE, COUNTRY, EMAIL, CREATED, UPDATED)"
                                   "VALUES ( '%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12')")
                                   .arg(createUniqueId())
                                   .arg(userId)
                                   .arg(user->mUserData->mFirstName)
                                   .arg(user->mUserData->mLastName)
                                   .arg(user->mUserData->mStreet)
                                   .arg(user->mUserData->mStreetNumber)
                                   .arg(user->mUserData->mCity)
                                   .arg(user->mUserData->mPostalCode)
                                   .arg(user->mUserData->mCountry)
                                   .arg(user->mUserData->mEMail)
                                   .arg(dateTime)
                                   .arg(dateTime);

        newUserQuery.exec(newUserStatement);
        if( !newUserQuery.isActive() )
          mLastErrorMessage = newUserQuery.lastError().text();
      }

      newUserStatement.clear();
      newUserStatement = QString::null;

      dateTime.clear();
      dateTime = QString::null;

      newUserQuery.clear();
    }
    else {
      mLastErrorMessage = newUserQuery.lastError().text();
    }
  }
  else {
    mLastErrorMessage = tr("");
  }
}

const QList<User *> Database::users() {

  User *user = 0;
  QList<User *> userList;
  if( mConnectionIsAvailable ) {

    mLastErrorMessage.clear();

    QString userQueryStatement = "SELECT UID, USERNAME, USERPASSWD FROM USERS ORDER BY USERNAME";
    QSqlQuery userQuery(mCurrentDatabase);
    userQuery.exec(userQueryStatement);
    if( userQuery.isActive() ) {

      while( userQuery.next() ) {

        user = new User;
        user->mId = userQuery.value(0).toString();
        user->mName = userQuery.value(1).toString();

        userQueryStatement.clear();
        userQueryStatement = QString("SELECT UDID, UID, FNAME, LNAME, STREETNAME, STREETNR, CITY, ZIPCODE, COUNTRY, EMAIL, CREATED, UPDATED "
                                     "FROM USERSDATA WHERE UID = '%1'").arg(user->mId);
        userQuery.clear();
        userQuery.exec(userQueryStatement);
        if( userQuery.isActive() ) {

          if( userQuery.record().count() >= 1 ) {

            while( userQuery.next() ) {

              user->mUserData = new UserData;
              user->mUserData->mId = userQuery.value(0).toString();
              user->mUserData->mUserId = userQuery.value(1).toString();
              user->mUserData->mFirstName = userQuery.value(2).toString();
              user->mUserData->mLastName = userQuery.value(3).toString();
              user->mUserData->mStreet = userQuery.value(4).toString();
              user->mUserData->mStreetNumber = userQuery.value(5).toString();
              user->mUserData->mCity = userQuery.value(6).toString();
              user->mUserData->mPostalCode = userQuery.value(7).toString();
              user->mUserData->mCountry = userQuery.value(8).toString();
              user->mUserData->mEMail = userQuery.value(9).toString();
              user->mUserData->mCreated = userQuery.value(11).toDateTime();
              user->mUserData->mUpdated = userQuery.value(12).toDateTime();
            }
          }
        }
        userList.append(user);
        userQueryStatement.clear();
      }
      userQuery.clear();
    }
    else
      mLastErrorMessage = userQuery.lastError().text();
  }

  return userList;
}

void Database::createDocument( const Document *doc ) {

  if( doc && mConnectionIsAvailable ) {

    QSqlQuery documentQuery(mCurrentDatabase);
    documentQuery.exec("");
    if( documentQuery.isActive() ) {

      while( documentQuery.next() ) {

      }
    }
  }
}

const QList<Document *> Database::documents() {

  Document *document = 0;
  User *user = 0;
  UserData *userData = 0;
  Groups *group = 0;

  QList<Document *> documentList;

  if( mConnectionIsAvailable ) {

    QSqlQuery documentQuery(mCurrentDatabase);
    documentQuery.exec("SELECT DID, UID, GID, DOCNAME, DOCPATH, UPDATED, CHECKEDOUT FROM DOCUMENTS ORDER BY DOCNAME");
    if( documentQuery.isActive() ) {

      while( documentQuery.next() ) {

      }
    }
  }

  return documentList;
}

const QList<Document *> Database::documents( const User *user ) {

  Document *document = 0;
  QList<Document *> documentList;
  if( user && mConnectionIsAvailable ) {
  }

  Q_UNUSED(document)
  return documentList;
}

const QList<Document *> Database::documents( const Groups *group ) {

  QList<Document *> documentList;
  if( group && mConnectionIsAvailable ) {
  }

  return documentList;
}

void Database::createGroup( const Groups *group ) {

  if( group && mConnectionIsAvailable ) {

    QSqlQuery groupQuery(mCurrentDatabase);
  }
}

const QList<Groups *> Database::groups() {

  return QList<Groups *>();
}

void Database::addDocumentToGroup( const QString &documentId, const QString &groupId ) {

  if( documentId.isNull() || documentId.isEmpty() )
    return;

  if( groupId.isNull() || groupId.isEmpty() )
    return;
}

void Database::addDocumentToUser( const QString &documentId, const QString &userId ) {

  if( documentId.isNull() || documentId.isEmpty() )
    return;

  if( userId.isNull() || userId.isEmpty() )
    return;
}

bool Database::initializeDatabase() {

  bool initializeFinished = false;

  QFile dbSqlFile(":/mysql_script");
  if( dbSqlFile.open(QIODevice::ReadOnly | QIODevice::Text) ) {

    QTextStream inStream(&dbSqlFile);
    QSqlQuery dbQuery(mCurrentDatabase);
    while( !inStream.atEnd() ) {

      QString sqlLine = inStream.readLine();
      if( !sqlLine.isEmpty() || !sqlLine.isNull() ) {

        initializeFinished = dbQuery.exec(sqlLine);
        dbQuery.clear();
      }
    }

    dbQuery.exec("INSERT INTO `GROUPS` ( `GID`, `GROUPNAME`, `GROUPDESCRIPTION`, `CREATED`, `UPDATED` ) VALUES ( 'b6d80cef14084a5b95643ea010484855', 'Default', 'Default group for all documents.', CURRENT_DATE, CURRENT_DATE );");
    dbQuery.clear();

    if( initializeFinished )
      mCurrentDatabase.setDatabaseName(mDatabaseName);

    dbSqlFile.close();
  }

  return initializeFinished;
}

const QString Database::createUniqueId() const {

  QString uniqueId = QUuid::createUuid().toString();
  uniqueId = uniqueId.replace("{", "");
  uniqueId = uniqueId.replace("}", "");
  uniqueId = uniqueId.replace("-", "");
  return uniqueId;
}
