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

#include <QSettings>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QDebug>

#include "util.h"
#include "Log.h"
#include "Preferences.h"
#include "YoutubeFeed.h"

#define QSETTINGS_GROUP "YoutubeFeed"

// main.cpp
extern SharedPreferences shPrefs;

// Ctor.

YoutubeFeed::YoutubeFeed (QWidget *parent)
  : QDialog(parent), _got_items (false)
{
  setupUi (this);

  QSettings s;
  NomNom::restore_size (s, this, QSETTINGS_GROUP, QSize (215,165));

  _proc = new ProcessProgressDialog (this);

  connect (_proc, SIGNAL (finished (QString)),
           this, SLOT (onFinished (QString)));

  _proc->setLabelText (tr ("Checking ..."));
  _proc->setMaximum (0);
}

bool YoutubeFeed::gotItems () const
{
  return _got_items;
}

// main.cpp
extern NomNom::Feed feed;

bool
YoutubeFeed::fromJSON (const QString& data, QString& error)
{
  QScriptEngine e;

  QScriptValue v = e.evaluate ("(" +data+ ")");

  if (e.hasUncaughtException ())
    {
      error = tr ("Uncaught exception at line %1: %2")
              .arg (e.uncaughtExceptionLineNumber ())
              .arg (v.toString ());
      return false;
    }

  QScriptValueIterator i (v.property ("video"));

  if (!i.hasNext ())
    {
      error = tr ("Expected at least one video from umph(1), got none.");
      return false;
    }

  feed.clear ();

  QString t,u;

  while (i.hasNext ())
    {

      i.next ();

      if (i.flags() & QScriptValue::SkipInEnumeration)
        continue;

      v = i.value ();
      t = v.property ("title").toString ();
      u = v.property ("url").toString ();

      QPair<QString,QString> p(t,u);
      feed.append(p);
    }

  return true;
}

// Slot: type changed.

void
YoutubeFeed::onTypeChanged (int n)
{
  static const QString s[] =
  { tr ("&Playlist ID:"), tr ("&Username:") };

  static const int m =
    static_cast<int>(sizeof (s) / sizeof (QString));

  if (n >= m)
    n = 1;

  idLabel->setText (s[n]);

}

// Slot: Umph: finished.

void
YoutubeFeed::onFinished (QString s)
{
  QString error = tr ("Could not find the JSON data in umph output.");
  const int n   = s.indexOf ("{");

  if (n != -1)
    {
      _got_items = fromJSON (s.mid (n), error);
      if (_got_items)
        return;
    }

  NomNom::crit (this, error);
}

#ifdef _0
// Slot: Umph: error.

void
YoutubeFeed::onError ()
{
  NomNom::crit (this, s);
}

#endif

// Done. QDialog and closeEvent design glitch workaround.

void
YoutubeFeed::done (int r)
{
  if (r == QDialog::Accepted)
    {

      const QString id = idEdit->text ();

      if (id.isEmpty ())
        return;

      QStringList args =
        shPrefs.get (SharedPreferences::UmphPath).toString ().split (" ");

      QString type;

      switch (typeCombo->currentIndex ())
        {
        default:
        case  0:
          type = "p";
          break;
        case  1:
          type = "f";
          break;
        case  2:
          type = "u";
          break;
        }

      args.replaceInStrings ("%t", type);
      args.replaceInStrings ("%s", startIndexSpin->text ());
      args.replaceInStrings ("%m", maxResultsSpin->text ());
      args.replaceInStrings ("%i", id);

      _proc->start (args);

      if (!_got_items)
        return;
    }

  QSettings s;
  NomNom::save_size (s, this, QSETTINGS_GROUP);

  QDialog::done (r);
  close ();
}

// Close.

void
YoutubeFeed::closeEvent (QCloseEvent *e)
{
  QDialog::closeEvent (e);
}

// vim: set ts=2 sw=2 tw=72 expandtab:
