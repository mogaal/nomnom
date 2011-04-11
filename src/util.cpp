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
#include <QTranslator>
#include <QMessageBox>
#include <QProcess>
#include <QWidget>
#include <QString>
#include <QPoint>
#include <QDebug>
#include <QSize>
#include <QDir>

#include "Log.h"
#include "util.h"

// main.cpp
extern QMap<QString,QStringList> hosts;
extern QMap<QString,QString> qmFiles;
extern QStringList qmLangNames;
extern NomNom::Feed feed;
extern Log log;

namespace NomNom
{

void
restore_size (
  QSettings& s,
  QWidget *w,
  const QString& g,
  const QSize& defaultSize/*=(400,350)*/)
{
  w->resize (s.value (QString ("%1/size").arg (g), defaultSize).toSize ());
}

void
restore_pos (
  QSettings& s,
  QWidget *w,
  const QString& g,
  const QPoint& defaultPos/*=(200,200)*/)
{
  w->move (s.value (QString ("%1/pos").arg (g), defaultPos).toPoint ());
}

void
save_size (QSettings& s, QWidget *w, const QString& g)
{
  s.setValue (QString ("%1/size").arg (g), w->size ());
}

void
save_pos (QSettings& s, QWidget *w, const QString& g)
{
  s.setValue (QString ("%1/pos").arg (g), w->pos ());
}

void
info (QWidget *p, const QString& m)
{
  p->show (); // Make sure window is not hidden (e.g. minimized to tray).
  p->showNormal ();
  QMessageBox::information (p, QCoreApplication::applicationName(), m);
}

void
crit (QWidget *p, const QString& m)
{
  p->show (); // See `info' function above.
  p->showNormal ();
  QMessageBox::critical (p, QCoreApplication::applicationName (), m);
}

QMessageBox::StandardButton
ask (QWidget *p, const QString& m, QMessageBox::StandardButtons b/*=Yes|No*/)
{
  p->show (); // See `info' function above.
  p->showNormal ();
  return QMessageBox::question(p, QCoreApplication::applicationName(), m, b);
}

static QFileInfoList
scan_dir (const QString path, const bool show_paths)
{
  const QDir dir (QDir::toNativeSeparators (path));

  if (show_paths)
    qDebug () << dir.absolutePath ();

  return dir.entryInfoList (QStringList( "*.qm"), QDir::Files);
}

QMap<QString,QString>
find_qm (QStringList& langNames)
{
  QSettings s;

  bool show_paths = false;

  const QString qmShowPaths = "qmShowPaths";
  if (s.contains (qmShowPaths))
    show_paths = s.value (qmShowPaths).toBool ();

  if (show_paths)
    qDebug () << "qm search paths:";

  QStringList paths;

  const QString qmPath = "qmPath";
  if (s.contains (qmPath))
    paths << s.value (qmPath).toString ();

  paths
      << QDir::currentPath () + "/tr"
      << QDir::homePath () + "/.config/nomnom/tr"
      << QDir::homePath () + "/.local/share/nomnom/tr"
#ifdef INSTALL_PREFIX
      << QString (INSTALL_PREFIX) + "/share/nomnom/tr"
#endif
      ;

  QFileInfoList lst;

  foreach (QString p, paths)
  lst << scan_dir (p, show_paths);

  QMap<QString,QString> map;
  QTranslator t;

  foreach (QFileInfo fi, lst)
  {

    t.load (fi.filePath ());

    const QString langName = t.translate ("MainWindow", "English");

    if (map.contains (langName))   // Skip duplicates.
      {
        if (map[langName] == fi.filePath ())
          continue;
      }

    map[langName] = fi.filePath ();

    langNames << langName;
  }

  return map;
}

bool
choose_lang (QWidget *p, QString& langName)
{
  bool showPaths = false;
  QStringList langNamesWithPaths;

  const QString key = "qmShowPaths";
  QSettings s;

  if (s.contains (key))
    {

      showPaths = s.value (key).toBool ();

      if (showPaths)
        {

          langNamesWithPaths << "English [default, built-in]";

          QMapIterator<QString,QString> iter (qmFiles);

          while (iter.hasNext ())
            {

              iter.next ();

              langNamesWithPaths << QString ("%1 [%2]")
                                 .arg (iter.key ())
                                 .arg (iter.value ());
            }

        }
    }

  bool ok  = false;

  langName = QInputDialog::getItem (
               p,
               QObject::tr ("Select language"),
               QObject::tr ("Language:"),
               showPaths ? langNamesWithPaths : qmLangNames,
               0,
               false,
               &ok
             );

  langName = langName.split ("[")[0].simplified ();

  return ok;
}

QTranslator*
load_qm ()
{
  QString langName;

  QSettings s;
  if (s.contains("language"))
    {
      const QString v = s.value("language").toString();

      if (v == "English")
        return NULL;

      if (qmFiles.contains(v))
        langName = v;
    }

  if (langName.isEmpty())
    {
      if (!choose_lang(NULL, langName))
        return NULL;
    }

  s.setValue("language", langName);

  QTranslator *t = new QTranslator;
  t->load(qmFiles[langName]);

  return t;
}

bool
parse_quvi_version (const QString& path, QString& output)
{
  output.clear ();

  // Use command path (arg0) and "--version" only.

  QStringList args =
    QStringList () << path.split (" ").takeFirst () << "--version";

  log << args.join (" ");

  const QString cmdPath = args.takeFirst ();

  QProcess proc;
  proc.setProcessChannelMode (QProcess::MergedChannels);
  proc.start (cmdPath, args);

  if (!proc.waitForFinished ())
    {

      output =
        QObject::tr ("error: %1: %2")
        .arg (cmdPath)
        .arg (proc.errorString ());

      return false;
    }

  output = QString::fromLocal8Bit (proc.readAll ()).simplified ();

  return true;
}

bool
parse_quvi_support (const QString& path, QString& errmsg)
{
  errmsg.clear ();

  // Use command path (arg0) and "--support" only.

  QStringList args =
    QStringList () << path.split (" ").takeFirst () << "--support";

  log << args.join (" ");

  const QString cmdPath = args.takeFirst ();

  QProcess proc;
  proc.setProcessChannelMode(QProcess::MergedChannels);
  proc.start(cmdPath, args);

  if (!proc.waitForFinished())
    {

      errmsg =
        QObject::tr("error: %1: %2")
        .arg(cmdPath)
        .arg(proc.errorString ());

      return false;
    }

  const QRegExp re("(.*)\\s+(.*)$");

  const QString output =
    QString::fromLocal8Bit(proc.readAll());

  foreach (QString ln, output.split("\n"))
  {

    if (ln.isEmpty())
      continue;

    log << ln;

    if (re.indexIn(ln) != -1)
      {

        const QString host  = re.cap (1).simplified ();
        QStringList formats = re.cap (2).simplified ().split ("|");

        // Keep "default" at the beginning of the list.

        const QString top = formats.takeFirst ();
        formats.sort ();
        formats.prepend (top);

        hosts[host] = formats;
      }
  }

  return true;
}

static bool
filter_title (
  QWidget *p,
  const QString& user_regexp,
  const QString& title,
  QString& dst)
{
  QString pattern;

  bool g = false;
  bool i = false;

  QRegExp rx("^\\/(.*)\\/(.*)$");

  if (rx.indexIn (user_regexp) != -1)
    {
      pattern = rx.cap (1);
      g = rx.cap (2).contains ("g");
      i = rx.cap (2).contains ("i");
    }
  else
    {
      NomNom::crit (p,
                    QObject::tr ("Expected Perl-style regular expression, e.g. /pattern/flags"));
      return false;
    }

  rx.setPattern (pattern);

  rx.setCaseSensitivity (
    i
    ? Qt::CaseInsensitive
    : Qt::CaseSensitive
  );

  int pos = 0;

  while ( (pos = rx.indexIn (title, pos)) != -1)
    {
      dst += rx.cap (1);
      pos += rx.matchedLength ();
      if (!g) break;
    }

  dst = dst.simplified ();

  return true;
}

bool
format_filename (
  QWidget *p,
  const QString& user_regexp,
  const QString& title,
  const QString& suffix,
  const QString& host,
  const QString& id,
  QString& dst)
{
  // Assumes dst to contain the "filename format".

  QString filtered_title;

  const bool ok = filter_title (
                    p,
                    user_regexp,
                    title,
                    filtered_title
                  );

  if (!ok)
    return ok;

  dst.replace ("%t", filtered_title);
  dst.replace ("%s", suffix);
  dst.replace ("%h", host);
  dst.replace ("%i", id);

  dst = dst.simplified ();

  return true;
}

QString
to_process_errmsg (QProcess::ProcessError n)
{
  QString e;

  switch (n)
    {

    case QProcess::FailedToStart:
      e = QObject::tr (
            "The process failed to start. "
            "Either the invoked program is missing, or you may have "
            "insufficient permissions to invoke the program."
          );
      break;

    case QProcess::Crashed:
      e = QObject::tr (
            "The process crashed some time after starting successfully."
          );
      break;

    case QProcess::Timedout:
      e = QObject::tr (
            "The last waitFor...() function timed out. "
            "The state of QProcess is unchanged, and you can try calling "
            "waitFor...() again."
          );
      break;

    case QProcess::WriteError:
      e = QObject::tr (
            "An error occurred when attempting to write to the process. "
            "For example, the process may not be running, or it may have closed "
            "its input channel."
          );
      break;

    case QProcess::ReadError:
      e = QObject::tr (
            "An error occurred when attempting to read from the process. "
            "For example, the process may not be running."
          );
      break;

    case QProcess::UnknownError:
    default:
      e = QObject::tr (
            "An unknown error occurred. This is the default return value "
            "of error()."
          );
      break;

    }

  return e;
}

bool
choose_from_feed (QWidget *parent, QString& dst)
{
  if (feed.isEmpty())
    return false;

  FeedIterator i(feed);
  QStringList items;

  while (i.hasNext())
    items << i.next().first;

  bool ok = false;

  QString title = QInputDialog::getItem (
                    parent,
                    QObject::tr ("Choose video"),
                    QObject::tr ("Video"),
                    items,
                    0,
                    false,
                    &ok
                  );

  if (ok)
    {
      i = FeedIterator(feed);
      while (i.hasNext())
        {
          QPair<QString,QString> p = i.next();
          if (p.first == title)
            {
              dst = p.second;
              break;
            }
        }
    }

  return ok;
}

} // End of namespace.

// vim: set ts=2 sw=2 tw=72 expandtab:
