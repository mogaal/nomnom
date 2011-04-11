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
#include <QShowEvent>
#include <QDebug>

#include "util.h"
#include "tips.h"
#include "Reminder.h"

#define QSETTINGS_GROUP                 "Reminder"

#define QSETTINGS_REMINDER_SHOWREMINDER "showReminder"
#define QSETTINGS_REMINDER_FIRSTRUN     "firstRun"

// Ctor.

Reminder::Reminder (QWidget *parent, const QString& group)
  : QDialog (parent), showReminder (true), currTip (0), firstRun (true)
{
  setupUi (this);

  QSettings s;
  NomNom::restore_size (s, this, QSETTINGS_GROUP, QSize (400,200));

  firstRunKey     = QString ("%1/%2")
                    .arg (group)
                    .arg (QSETTINGS_REMINDER_FIRSTRUN);

  showReminderKey = QString ("%1/%2")
                    .arg (group)
                    .arg (QSETTINGS_REMINDER_SHOWREMINDER);

  if (s.contains (firstRunKey))
    firstRun = s.value (firstRunKey).toBool ();

  if (s.contains (showReminderKey))
    showReminder = s.value (showReminderKey).toBool ();

  textBrowser->setHtml (NomNom::get_tip (currTip, firstRun));
}

// Slot: on Next.

void
Reminder::onNext ()
{
  textBrowser->setHtml (NomNom::next_tip (currTip));
}

// Conditional exec, or "until a more elegant way is found".

bool
Reminder::conditionalExec ()
{
  if (showReminder) exec ();
  return showReminder;
}

// Done. QDialog and closeEvent design glitch workaround.

void
Reminder::done (int r)
{
  QSettings s;
  NomNom::save_size (s, this, QSETTINGS_GROUP);

  showReminder = showagainBox->isChecked(); // Update.

  s.setValue (firstRunKey, false);
  s.setValue (showReminderKey, showReminder);

  QDialog::done (r);
  close ();
}

// Close.

void
Reminder::closeEvent (QCloseEvent *e)
{
  QDialog::closeEvent(e);
}

// vim: set ts=2 sw=2 tw=72 expandtab:
