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
#include <QDebug>

#include "util.h"
#include "Log.h"
#include "LogView.h"

#define QSETTINGS_GROUP "LogView"

// main.cpp
extern Log log;

// Ctor.

LogView::LogView (QWidget *parent)
  : QDialog (parent)
{
  setupUi (this);

  textBrowser->setText (log.data ());

  QSettings s;
  NomNom::restore_size (s, this, QSETTINGS_GROUP, QSize (620,375));
}

// Slot: clear.

void
LogView::onClear ()
{
  log.clear ();
  log << tr ("Log cleared.");
  textBrowser->setText (log.data ());
}

// Done. QDialog and closeEvent design glitch workaround.

void
LogView::done (int r)
{
  QSettings s;
  NomNom::save_size (s, this, QSETTINGS_GROUP);
  QDialog::done (r);
  close ();
}

// Close.

void
LogView::closeEvent (QCloseEvent *e)
{
  QDialog::closeEvent (e);
}

// vim: set ts=2 sw=2 tw=72 expandtab:
