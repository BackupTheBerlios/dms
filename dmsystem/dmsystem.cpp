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
#include "dmsystem.h"

#include <QAction>
#include <QApplication>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>

using namespace asaal;

DMSystem::DMSystem( QWidget *parent )
  : QMainWindow( parent ),
    mActionWorkSheet(0),
    mActionUsers(0),
    mActionGroup(0),
    mActionDocuments(0),
    mActionSearch(0),
    mActionLogin(0),
    mActionPreferences(0) {

  mMdiArea = new QMdiArea();
  setCentralWidget(mMdiArea);

  setWindowTitle(QApplication::applicationName());

#if defined(Q_OS_MAC)
  setUnifiedTitleAndToolBarOnMac(true);
#endif

  initializeToolBar();
  initializeMenus();
  initializeStatusBar();
  initializePlugins();
}

void DMSystem::closeEvent( QCloseEvent *event ) {

  Q_UNUSED(event)

  Database::databaseInstance()->logout();
  Database::databaseInstance()->closeConnection();

  delete Database::databaseInstance();
}

void DMSystem::setArguments( int argc, char **argv ) {
  
  Q_UNUSED(argc)
  Q_UNUSED(argv)
}

void DMSystem::initializeToolBar() {

  QToolBar *toolbar = addToolBar( tr( "DMSystem") );
  toolbar->setMovable(false);
  toolbar->setFloatable(false);
  toolbar->setIconSize(QSize(24, 24));
  toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

  mActionWorkSheet = new QAction(QIcon(":/gohome"), tr("Worksheet"), this);
  mActionUsers = new QAction(QIcon(":/users"), tr("Users"), this);
  mActionGroup = new QAction(QIcon(":/group"), tr("Groups"), this);
  mActionDocuments = new QAction(QIcon(":/book"), tr("Doucments"), this);
  mActionSearch = new QAction(QIcon(":/search"), tr("Search"), this);
  mActionLogin = new QAction(QIcon(":/secury"), tr("Login"), this);
  mActionPreferences = new QAction(QIcon(":/preferences"), tr("Preferences"), this);

  toolbar->addAction(mActionLogin);
  toolbar->addAction(mActionWorkSheet);
  toolbar->addSeparator();

  toolbar->addAction(mActionUsers);
  toolbar->addAction(mActionGroup);
  toolbar->addAction(mActionDocuments);
  toolbar->addSeparator();

  toolbar->addAction(mActionSearch);
  toolbar->addAction(mActionPreferences);
}

void DMSystem::initializeMenus() {
}

void DMSystem::initializeStatusBar() {

  statusBar()->show();
}

void DMSystem::initializePlugins() {
}
