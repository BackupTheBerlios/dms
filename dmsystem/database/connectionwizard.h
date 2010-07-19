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

#ifndef CONNECTIONWIZARD_H
#define CONNECTIONWIZARD_H

#include "ui_connectionwizard.h"

#include <QWizard>

namespace asaal {

  class ConnectionWizard : public QWizard, private Ui::UiConnectionWizard {

      Q_OBJECT

    public:
      ConnectionWizard( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
      ~ConnectionWizard();

      void accept();
      void reject();

    private:
      void setFiniheButtonEnabled( bool enabled = true );
      void setNextButtonEnabled( bool enabled = true );

    private slots:
      void slotCurrentIdChanged( int id );
      void slotTestConnection();
      void slotAddUser();
      void slotRemoveUser();

    protected:
      void closeEvent( QCloseEvent *event );
  };
}

#endif // CONNECTIONWIZARD_H
