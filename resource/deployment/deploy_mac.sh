#!/bin/sh
#/*
# *
# * Copyright (C) 2010 Alexander Saal.
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

clear

QTDIR=$1

if [ "$QTDIR" != "" ]
then
  QMAKE=$QTDIR/bin/qmake
  MACDEPLOY=$QTDIR/bin/macdeployqt

  if [ "$MACDEPLOY" != "" ]
  then
    MACNAMETOOL=/usr/bin/install_name_tool
    COPYTOOL=/bin/cp

    echo "Build DMSystem in release mode ..."
    cd ../
    make -j 2 distclean &>/dev/null
    $QMAKE "CONFIG+=release" -recursive &>/dev/null
    make &>/dev/null
    cd deployment

    echo "Copying binary for deploying ..."
    $COPYTOOL -R ../build/bin/DMSystem.app .
  
    echo "Deploying DMSystem ..."
    $MACDEPLOY DBoxFE.app &>/dev/null

    $COPYTOOL -R Info.plist DMSystem.app/Contents
    #$COPYTOOL -R dboxfe.icns DBoxFE.app/Contents/Resources

    echo "Cleanup build ..."
    cd ../
    make -j 2 distclean &>/dev/null
    cd deployment
  else
    echo "Mac deploying tool not available for this Qt version."
    exit -1
  fi
else
  echo "Please enter the path to your Qt root directory."
  exit -1
fi

