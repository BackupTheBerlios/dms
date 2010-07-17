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

#ifndef DATABASEUSERDETAILS_H
#define DATABASEUSERDETAILS_H

#include "database.h"

#include "ui_databaseuserdetails.h"

#include <QDialog>

namespace asaal {

  class DatabaseUserDetails : public QDialog, private Ui::UiDatabaseUserDetails {

      Q_OBJECT

    public:
      DatabaseUserDetails( QWidget *parent = 0 );
      ~DatabaseUserDetails();

      const User *databaseUserDetails();

    protected:
      void closeEvent( QCloseEvent *event );

    private slots:
      void slotOk();
      void slotCancel();
  };
}

#endif // DATABASEUSERDETAILS_H