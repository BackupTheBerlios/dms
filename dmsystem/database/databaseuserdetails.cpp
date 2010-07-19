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

#include "databaseuserdetails.h"

#include <QCloseEvent>
#include <QDateTime>

using namespace asaal;

DatabaseUserDetails::DatabaseUserDetails( const QString &userName, const QString &userPassword, QWidget *parent )
  : QDialog( parent ),
    mUserName(userName),
    mUserPassword(userPassword),
    mUserDetails(0) {

  setupUi(this);

  connect(mButtonCancel, SIGNAL(clicked()), this, SLOT(slotCancel()));
  connect(mButtonOk, SIGNAL(clicked()), this, SLOT(slotOk()));
}

DatabaseUserDetails::~DatabaseUserDetails() {
}

void DatabaseUserDetails::closeEvent( QCloseEvent *event ) {

  event->ignore();
}

void DatabaseUserDetails::slotOk() {

  mUserDetails = new User;
  mUserDetails->mName = mUserName;
  mUserDetails->mPassword = mUserPassword;

  mUserDetails->mUserData = new UserData;
  mUserDetails->mUserData->mFirstName = mLineEditFirstname->text();
  mUserDetails->mUserData->mLastName = mLineEditLastname->text();
  mUserDetails->mUserData->mStreet = mLineEditStreet->text();
  mUserDetails->mUserData->mStreetNumber = mLineEditStreetNumber->text();
  mUserDetails->mUserData->mCity = mLineEditCity->text();
  mUserDetails->mUserData->mPostalCode = mLineEditZipCode->text();
  mUserDetails->mUserData->mCountry = mLineEditCountry->text();
  mUserDetails->mUserData->mEMail = mLineEditEMail->text();
  mUserDetails->mUserData->mCreated = QDateTime::currentDateTime();
  mUserDetails->mUserData->mUpdated = mUserDetails->mUserData->mCreated;

  QDialog::accept();
}

void DatabaseUserDetails::slotCancel() {

  if( mUserDetails ) {

    if( mUserDetails->mUserData )
      delete mUserDetails->mUserData;
    mUserDetails->mUserData = 0;

    delete mUserDetails;
    mUserDetails = 0;
  }

  QDialog::reject();
}
