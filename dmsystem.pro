#/*
# *
# * Copyright (C) 2008 - 2010 Alexander Saal.
# * All rights reserved.
# * Contact: Alexander Saal (alex.saal@gmx.de)
# *
# * This file is part of DMSystem.
# *
# * This program is free software; you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation; either version 3 of the License, or (at
# * your option) any later version.
# *
# * This program is distributed in the hope that it will be useful, but
# * WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with this program; if not, write to the Free Software Foundation,
# * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
# *
# */

include(dmsystem/dmsystem.pri)

# Qt 4 Settings
TEMPLATE = app
DESTDIR	= build/bin

RESOURCES += resource/dmsystem.qrc
CONFIG += qt thread warn_on
QT += network sql xml

# Config settings
CONFIG(debug, debug|release) {

  TARGET = DMSystemD
  RCC_DIR = build/DMSystemD/rcc
  MOC_DIR += build/DMSystemD/moc
  OBJECTS_DIR += build/DMSystemD/obj
  UI_DIR += build/DMSystemD/ui

  DEFINES += DMS_DEBUG

} else {

  TARGET = DMSystem
  RCC_DIR = build/DMSystem/rcc
  MOC_DIR += build/DMSystem/moc
  OBJECTS_DIR += build/DMSystem/obj
  UI_DIR += build/DMSystem/ui

  DEFINES -= DMS_DEBUG

  mac {
    QMAKE_POST_LINK = strip -s build/bin/DMSystem.app/Contents/MacOS/DMSystem
  }
  unix:!mac {
    QMAKE_POST_LINK = strip -s build/bin/DMSystem
  }
}

# Windows settings
win32 {
  CONFIG += embed_manifest_dll
  CONFIG += embed_manifest_exe
  CONFIG += debug_and_release build_all
  RC_FILE = resource/dmsystem.rc
  CONFIG -= console
  CONFIG -= flat
}

# Unix settings
unix:!mac {
  CONFIG += debug
}

# Mac settings
mac {
  contains(QT_VERSION, ^4\.[0-6]\..*) {

    QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.6.sdk
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6
    CONFIG(x86_64) {
      CONFIG += x86_64
    }
    else {
      CONFIG += x86
    }
  }
  else {
    contains(QT_VERSION, ^4\.[0-5]\..*) {
      error("Can't build DBoxFE with Qt version $${QT_VERSION}.")
    }
  }
}
