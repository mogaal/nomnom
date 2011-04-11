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

#include <QTextStream>
#include <QSettings>
#include <QDebug>

#include "util.h"
#include "Preferences.h"
#include "About.h"

#define QSETTINGS_GROUP "About"

// main.cpp

extern QMap<QString,QStringList> hosts;
extern SharedPreferences shPrefs;
extern QString quviVersion;

// Ctor.

About::About (QWidget *parent)
  : QDialog(parent)
{
  setupUi (this);

  const QString quviPath =
    shPrefs.get (SharedPreferences::QuviPath).toString ();

  QString errmsg;

  if (hosts.isEmpty ())
    {
      if (!quviPath.isEmpty ())
        NomNom::parse_quvi_support (quviPath, errmsg);
      else
        errmsg = tr ("You must specify path to the quvi command.");
    }

  if (!quviPath.isEmpty ()
      && quviVersion.isEmpty ()
      && errmsg.isEmpty())
    {
      if (NomNom::parse_quvi_version (quviPath, quviVersion))
        quviVersion.remove ("quvi version ");
      else
        errmsg = quviVersion;
    }

  if (errmsg.isEmpty())
    {
      foreach (QString k, hosts.keys ())
      {
        new QListWidgetItem (k, listWidget);
      }
    }
  else
    {
      new QListWidgetItem (errmsg, listWidget);
    }

  QString version;
  QTextStream qts (&version);

  qts << QCoreApplication::applicationVersion ()
#ifdef TARGET_HOST
      << " (" << TARGET_HOST << ")"
#endif
      ;

  QSettings s;

  QString html = textBrowser->toHtml ();
  html.replace ("$version",       version);
  html.replace ("$quvi_version",  quviVersion);
  html.replace ("$qt_version",    qVersion ());
  html.replace ("$config_file",   s.fileName ());
  textBrowser->setHtml (html);

  NomNom::restore_size(s, this, QSETTINGS_GROUP, QSize(620,375));
}

// Done. QDialog and closeEvent design glitch workaround.

void
About::done (int r)
{
  QSettings s;
  NomNom::save_size(s, this, QSETTINGS_GROUP);
  QDialog::done(r);
  close();
}

// Close.

void
About::closeEvent (QCloseEvent *e)
{
  QDialog::closeEvent(e);
}

// vim: set ts=2 sw=2 tw=72 expandtab:
