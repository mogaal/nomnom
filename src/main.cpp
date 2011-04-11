/*
* Copyright (C) 2010 Toni Gundogdu.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "config.h"

#include <QApplication>
#include <QLibraryInfo>
#include <QDebug>

#include "util.h"
#include "Recent.h"
#include "Log.h"
// UI:
#include "Preferences.h"
#include "MainWindow.h"

// Global: Hosts (or websites) that quvi supports.
QMap<QString,QStringList> hosts;

// Global: quvi version.
QString quviVersion;

// Global: Language.
QMap<QString,QString> qmFiles;
QStringList qmLangNames;

// Global: Preferences.
SharedPreferences shPrefs;

// Global: Recent.
Recent recent;

// Global: Log.
Log log;

// Global: Feed.
NomNom::Feed feed;

int
main (int argc, char *argv[])
{
  qsrand (time (NULL));

  QApplication app(argc, argv);

#define APPNAME   "NomNom"
#define APPDOMAIN "nomnom.sourceforge.net"

  QCoreApplication::setOrganizationName   (APPNAME);
  QCoreApplication::setOrganizationDomain (APPDOMAIN);
  QCoreApplication::setApplicationName    (APPNAME);
  QCoreApplication::setApplicationVersion (VERSION_LONG);

#ifdef _0
  qDebug() << QLibraryInfo::location(QLibraryInfo::TranslationsPath);
  qDebug() << "qt_"+QLocale::system().name();
#endif

  // Load qt translation, chosen based on the system locale setting.

  QTranslator qtTr;

  qtTr.load("qt_" + QLocale::system().name(),
            QLibraryInfo::location(QLibraryInfo::TranslationsPath));

  app.installTranslator(&qtTr);

  // Load NomNom translation, chosen based on user definition.

  //: "English" is the default language. This string is not intended to be translated.
  qmLangNames << QObject::tr ("English");
  qmFiles = NomNom::find_qm(qmLangNames);

  QTranslator *t = NomNom::load_qm();
  if (t) QCoreApplication::installTranslator(t);

#ifdef _0
  // Read preferences. Done in MainWindow ctor.
  shPrefs.read();
#endif

  // Read recent.

  recent.read();

  // Show main window.

  (new MainWindow)->show();

  return app.exec();
}

// vim: set ts=2 sw=2 tw=72 expandtab:
