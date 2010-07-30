;/*
; *   Copyright (C) ©'2007 - 2010 by Alexander Saal <alex.saal@gmx.de>
; *
; *   This program is free software; you can redistribute it and/or modify
; *   it under the terms of the GNU General Public License as published by
; *   the Free Software Foundation; either version 3 of the License, or (at
; *   your option) any later version.
; *
; *   This program is distributed in the hope that it will be useful, but
; *   WITHOUT ANY WARRANTY; without even the implied warranty of
; *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; *   GNU General Public License for more details.
; *
; *   You should have received a copy of the GNU General Public License
; *   along with this program; if not, write to the Free Software Foundation,
; *   Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
; */

#define MyAppName "DMSystem"
#define MyAppVerName "Document Managemnt System 0.9.5.0"
#define MyAppVersion "0.9.5.0"
#define MyAppPublisher "Privat - Alexander Saal"
#define MyAppURL "http://dms.berlios.de/index"
#define MyAppCopyright "(C)opyright ©'2007 - 2010 by Alexander Saal"
#define MyAppExeName "DMSystem.exe"
#define MyAppUrlName "DMSystem.url"

; Setup Qt4 directory
#define QtDirectory "C:\Entwicklung\Qt\4.6.3"

; Setup MySQL path
#define MySQLDirectory "C:\Entwicklung\MySQL\Server"

; Setup VC redist path
#define VCRedistPath "C:\Program Files\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT"

[Setup]
AppName={#MyAppName}
AppVerName={#MyAppVerName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}

DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}

VersionInfoVersion={#MyAppVersion}
VersionInfoCopyright={#MyAppCopyright}
VersionInfoCompany={#MyAppPublisher}

AllowNoIcons=true
LicenseFile=..\resource\misc\COPYING
OutputDir=.
OutputBaseFilename=dmsystem-{#MyAppVersion}-setup-win32
SetupIconFile=..\resource\dmsystem.ico
Compression=lzma
SolidCompression=true
InternalCompressLevel=max
MinVersion=4.1.2222,5.0.2195sp3
AppID={{0F40AF50-E524-4B61-9772-CFAA42C0672A}
UninstallDisplayIcon={app}\{#MyAppExeName}
DisableFinishedPage=false
UninstallDisplayName={#MyAppVersion}-{#MyAppName}
AllowRootDirectory=true
UninstallLogMode=new
DisableStartupPrompt=true
;WizardImageFile=..\..\..\WizardImages\Modern\SetupModern16.bmp
;WizardSmallImageFile=..\..\..\WizardImages\Modern\Small\SetupModernSmall16.bmp
ShowLanguageDialog=yes

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; MinVersion: 4.1.2222,4.0.1381sp6
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; MinVersion: 4.1.2222,4.0.1381sp6

[Files]
Source: ..\build\bin\{#MyAppExeName}; DestDir: {app}; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: ..\resource\misc\COPYING; DestDir: {app}; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: ..\resource\misc\AUTHOR; DestDir: {app}; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6

; Needed library (Qt4 Release):
Source: {#QtDirectory}\bin\QtGui4.dll; DestDir: {app}; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\bin\QtCore4.dll; DestDir: {app}; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\bin\QtNetwork4.dll; DestDir: {app}; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\bin\QtSvg4.dll; DestDir: {app}; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\bin\QtSql4.dll; DestDir: {app}; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\bin\QtXml4.dll; DestDir: {app}; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6

; Plugins from Qt4
Source: {#QtDirectory}\plugins\accessible\qtaccessiblecompatwidgets4.dll; DestDir: {app}\plugins\accessible; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\accessible\qtaccessiblewidgets4.dll; DestDir: {app}\plugins\accessible; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\codecs\qcncodecs4.dll; DestDir: {app}\plugins\codecs; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\codecs\qjpcodecs4.dll; DestDir: {app}\plugins\codecs; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\codecs\qkrcodecs4.dll; DestDir: {app}\plugins\codecs; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\codecs\qtwcodecs4.dll; DestDir: {app}\plugins\codecs; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\graphicssystems\qglgraphicssystem4.dll; DestDir: {app}\plugins\graphicssystems; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\graphicssystems\qtracegraphicssystem4.dll; DestDir: {app}\plugins\graphicssystems; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\iconengines\qsvgicon4.dll; DestDir: {app}\plugins\iconengines; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\imageformats\qgif4.dll; DestDir: {app}\plugins\imageformats; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\imageformats\qico4.dll; DestDir: {app}\plugins\imageformats; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\imageformats\qjpeg4.dll; DestDir: {app}\plugins\imageformats; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\imageformats\qmng4.dll; DestDir: {app}\plugins\imageformats; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\imageformats\qsvg4.dll; DestDir: {app}\plugins\imageformats; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\imageformats\qtiff4.dll; DestDir: {app}\plugins\imageformats; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\sqldrivers\qsqlite4.dll; DestDir: {app}\plugins\sqldrivers; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\sqldrivers\qsqlmysql4.dll; DestDir: {app}\plugins\sqldrivers; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\sqldrivers\qsqlodbc4.dll; DestDir: {app}\plugins\sqldrivers; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#QtDirectory}\plugins\sqldrivers\qsqlpsql4.dll; DestDir: {app}\plugins\sqldrivers; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6

; MySQL library needs by Qt4 MySQL plugin
Source: {#MySQLDirectory}\bin\libmySQL.dll; DestDir: {app}; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6

; Needed library (MS VC++ 2008 Release)
Source: {#VCRedistPath}\msvcm90.dll; DestDir: {app}; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#VCRedistPath}\msvcr90.dll; DestDir: {app}; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#VCRedistPath}\msvcp90.dll; DestDir: {app}; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6
Source: {#VCRedistPath}\Microsoft.VC90.CRT.manifest; DestDir: {app}; Flags: ignoreversion uninsremovereadonly 32bit overwritereadonly replacesameversion; MinVersion: 4.1.2222,4.0.1381sp6

[INI]
Filename: {app}\{#MyAppUrlName}; Section: InternetShortcut; Key: URL; String: {#MyAppURL}; Flags: createkeyifdoesntexist; MinVersion: 4.1.2222,4.0.1381sp6

[Icons]
Name: {group}\{#MyAppName}; Filename: {app}\{#MyAppExeName}
Name: {group}\{cm:ProgramOnTheWeb,{#MyAppName}}; Filename: {app}\{#MyAppUrlName}
Name: {group}\{cm:UninstallProgram,{#MyAppName}}; Filename: {uninstallexe}
Name: {userdesktop}\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: desktopicon
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: quicklaunchicon

[Run]
Filename: {app}\{#MyAppExeName}; Description: {cm:LaunchProgram,{#MyAppName}}; Flags: postinstall skipifsilent; MinVersion: 4.1.2222,4.0.1381sp6; WorkingDir: {app}

[UninstallDelete]
Type: files; Name: {app}\{#MyAppUrlName}

[Messages]
BeveledLabel={#MyAppCopyright}
