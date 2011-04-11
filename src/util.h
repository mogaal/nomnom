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

#ifndef nomnom_util_h
#define nomnom_util_h

#include <QInputDialog>
#include <QMessageBox>
#include <QStringList>
#include <QTranslator>
#include <QSettings>
#include <QProcess>

namespace NomNom
{

typedef QListIterator<QPair<QString,QString> > FeedIterator;
typedef QList< QPair<QString,QString> > Feed;

void
restore_size(
  QSettings&,
  QWidget*,
  const QString&,
  const QSize& defaultSize=QSize(400,350)
);

void
restore_pos (
  QSettings&,
  QWidget*,
  const QString&,
  const QPoint& defaultPos=QPoint(200,200)
);

void
save_size   (
  QSettings&,
  QWidget*,
  const QString&
);

void
save_pos    (
  QSettings&,
  QWidget*,
  const QString&
);

void
info    (QWidget*, const QString&);

void
crit    (QWidget*, const QString&);

#define DEFAULTBTNS QMessageBox::Yes|QMessageBox::No

QMessageBox::StandardButton
ask (QWidget*, const QString&, QMessageBox::StandardButtons=DEFAULTBTNS);

#undef DEFAULTBTNS

QMap<QString,QString>
find_qm (QStringList&);

bool
choose_lang (QWidget*, QString&);

QTranslator*
load_qm ();

bool
parse_quvi_version (const QString& path, QString& output);

bool
parse_quvi_support (const QString& path, QString& errmsg);

bool
format_filename (
  QWidget*,
  const QString& user_regexp,
  const QString& title,
  const QString& suffix,
  const QString& host,
  const QString& id,
  QString& dst
);

QString
to_process_errmsg (QProcess::ProcessError);

bool
choose_from_feed (QWidget *, QString&);

} // End of namespace.

#endif

// vim: set ts=2 sw=2 tw=72 expandtab:
