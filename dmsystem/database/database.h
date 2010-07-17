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

#ifndef DATABASE_H
#define DATABASE_H

#include <QDateTime>
#include <QList>
#include <QObject>

class QString;
class QStringList;

namespace asaal {

  struct UserData {

    QString mId;
    QString mUserId;
    QString mFirstName;
    QString mLastName;
    QString mStreet;
    QString mStreetNumber;
    QString mCity;
    QString mPostalCode;
    QString mCountry;
    QString mEMail;
    QDateTime mCreated;
    QDateTime mUpdated;
  };

  struct User {
    QString mId;
    QString mName;
    QString mPassword;
    UserData *userData;
  };

  struct Groups {
    QString mId;
    QString mName;
    QString mDescription;
  };

  struct Document {
    QString mId;
    QString mName;
    QString mPath;
    QDateTime mCreated;
    QDateTime mUpdated;

    QList<User *> mUsers;
    QList<Groups *> mGroups;
  };

  class Database : public QObject {

      Q_OBJECT

    public:
      enum DatabaseType {

        MySQL = 0,  // MySQL 5.1 or higher.
        MSSQL,      // Microsoft SQL Server 2005 or higher
        SQLite3     // SQLite3 not supported at this time.
      };

      ~Database() {}
      static Database *databaseInstance();

      void setDatabaseInformation( const QString &host, const QString &user, const QString &userPassword, const int port = 3306, const DatabaseType type = MySQL );

      bool openConnection();
      bool closeConnection();

      bool login( const User *user );
      bool logout();

      void createUser( const User *user );
      const QList<User *> users();

      void createDocument( const Document *doc );
      const QList<Document *> documents();
      const QList<Document *> documents( const User *user );
      const QList<Document *> documents( const Groups *group );

      void createGroup( const Groups *group );
      const QList<Groups *> groups();

      void addDocumentToGroup( const QString &documentId, const QString &groupId );
      void addDocumentToUser( const QString &documentId, const QString &userId );

      inline const User *currentUser() const { return mCurrentUser; }
      inline const QString lastErrorMessage() const { return mLastErrorMessage; }

      const QString createUniqueId() const;

    private:
      User *mCurrentUser;
      QString mLastErrorMessage;
      bool mConnectionIsAvailable;

      bool initializeDatabase();

    protected:
      Database( QObject *parent = 0 );
  };
}

#endif // DATABASE_H
