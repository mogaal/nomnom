/* NomNom
 * Copyright (C) 2011  Toni Gundogdu <legatvs@gmail.com>
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

#include <QCoreApplication>
#include <QMessageBox>
#include <QProcess>

#ifdef ENABLE_VERBOSE
#include <QDebug>
#endif

#include <NSettingsMutator>
#include <NUtil>

// main.cpp
extern nn::NSettingsMutator settings;

namespace nn
{

/*
 * Function: ask
 *
 * Prompt for yes or no confirmation from user.
 *
 * Parameters:
 *  p - Parent widget
 *  t - Text to display
 *
 * Returns:
 *  Either QMessageBox::Yes or QMessageBox::No
 */
int ask(QWidget *p, const QString& t)
{
  const QMessageBox::StandardButtons b = QMessageBox::Yes|QMessageBox::No;
  return QMessageBox::question(p, qApp->applicationName(), t, b);
}

/*
 * Function: info
 *
 * Display info messagebox to the user.
 *
 * Parameters:
 *  p - Parent widget
 *  t - Text to display
 */
void info(QWidget *p, const QString& t)
{
  QMessageBox::information(p, qApp->applicationName(), t);
}

/*
 * Function: check_for_cmd_feature
 *
 * Check whether a command supports a feature.
 *
 * Parameters:
 *  a               - "Using" setting key, e.g. ParseUsing
 *  b               - "Other" setting key, e.g. ParseUsingOther
 *  expect_exitcode - Expected OK exit code to compare against
 *
 * Returns:
 *  true or false
 */
bool check_for_cmd_feature(SettingKey a,
                           SettingKey b,
                           const QString& feature,
                           const int expect_exitcode)
{
  const QString q = settings.eitherValue(a,b).toString().simplified();

  if (q.isEmpty())
    return false;

  QStringList args = QStringList()
                     << to_cmd_args(q).takeFirst()
                     << feature;

#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "args=" << args;
#endif

  QProcess *p = new QProcess;
  p->start(args.takeFirst(), args);

  if (!p->waitForFinished())
    return false;

#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__
           << "exit_status=" << p->exitStatus()
           << "exit_code="<< p->exitCode();
#endif

// 0x1=invalid option, 0x3=no input

  return (p->exitStatus() == 0x0 && p->exitCode() == expect_exitcode);
}

/*
 * Function: to_cmd_args
 *
 * Converts NSettingsMutator command string to a list of command args
 * that can be passed to QProcess.
 *
 * Parameters:
 *  s - String to convert (e.g. "/usr/bin/quvi:--category-http -r %u")
 *
 * Returns:
 *  List of args, the first is the path to the command
 */
QStringList to_cmd_args(const QString& s)
{
#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "parse=" << s;
#endif

// $cmd_path:$cmd_args

  static const QString::SplitBehavior sb = QString::SkipEmptyParts;
  static const QString sep = NSETTINGS_CMDPATH_SEPARATOR;

  if (s.simplified().contains(sep))
    {
      QStringList r, a = s.simplified().split(sep);
      r << a.takeFirst();
      r << a.takeFirst().split(" ", sb);
      return r;
    }

// "Other" value.

  return s.simplified().split(" ", sb);
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */

