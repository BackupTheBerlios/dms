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

#ifndef DMSYSTEMGLOBAL_H
#define DMSYSTEMGLOBAL_H

#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QWidget>

/* Application defines */
#define DMSApplicationName "DMS - Document Management System"
#define DMSSoftVersion "0.9.5"
#define DMSSoftOrganization "Alexander Saal"
#define DMSSoftOrganizationDomain "http://dms.berlios.de/index/"

/* Configuration file defines*/
#define DMSDatabaseConfigFile "database.conf"
#define DMSServerSection "Server"
#define DMSServerHostKey "Host"
#define DMSServerPortKey "Port"
#define DMSServerUserSection "ServerUser"
#define DMSServerUserKey "User"
#define DMSServerPasswordKey "Password"

/* Configuration folder defines */
#if defined(Q_OS_WIN) || defined(Q_OS_WIN32)
  #undef DMSConfigDirectory
  #define DMSConfigDirectory QApplication::applicationDirPath()
#elif defined(Q_WS_MAC) || defined(Q_OS_MAC) || defined(Q_WS_MAC64) || defined(Q_WS_MACX) || defined(Q_OS_MACX)
  #undef DMSConfigDirectory
  #define DMSConfigDirectory "/Library/Application Support/DMSystem"
#elif defined(Q_OS_UNIX)
  #undef DMSConfigDirectory
  #define DMSConfigDirectory "/.dms"
#endif

namespace asaal {

  enum DMSWizardPage {
    WelcomePage = 0,
    SetupPage = 1,
    FinalPage = 2
  };

  static inline void centerWidget( QWidget *widget, QWidget *subWidget, bool isSubWidget) {

    if( !widget )
      return;

    if( !isSubWidget ) {

      QDesktopWidget *desktop = qApp->desktop();
      const QRect rect = desktop -> availableGeometry( desktop->primaryScreen() );
      int left = ( rect.width() - widget->width() ) / 2;
      int top = ( rect.height() - widget->height() ) / 2;
      widget->setGeometry( left, top, widget->width(), widget->height() );
    }
    else {

      if( !subWidget )
        return;

      const QRect rect = widget->geometry();
      int left = ( rect.width() - subWidget->width() ) / 2;
      int top = ( rect.height() - subWidget->height() ) / 2;
      subWidget->setGeometry( left, top, subWidget->width(), subWidget->height() );
    }
  }
}

#endif // DMSYSTEMGLOBAL_H
