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

#include <QDebug>

#include "util.h"
#include "Log.h"
#include "ProcProgDiag.h"

// main.cpp
extern Log log;

ProcessProgressDialog::ProcessProgressDialog (QWidget *parent/*=NULL*/)
  : QProgressDialog (parent),
    _rx_error (QRegExp ("error:\\s+(.*)$")),
    _canceled (false)
{
#define _conn(s,sl) \
    do { connect (&_proc, SIGNAL(s), this, SLOT(sl)); } while (0)

  _conn (started (), onProcStarted ());

  _conn (error (QProcess::ProcessError),
         onProcError (QProcess::ProcessError));

  _conn (readyRead (), onProcReadyRead ());

  _conn (finished (int, QProcess::ExitStatus),
         onProcFinished (int, QProcess::ExitStatus));

#undef _conn

  connect (this, SIGNAL(canceled ()), this, SLOT (onCanceled ()));

  setWindowModality (Qt::WindowModal);
  setAutoClose (false);

  _proc.setProcessChannelMode (QProcess::MergedChannels);

}

void
ProcessProgressDialog::setLabelRegExp (const QHash<QString,QRegExp>& h)
{
  _rx_label = h;
}

void
ProcessProgressDialog::setErrorRegExp (const QRegExp& rx)
{
  _rx_error = rx;
}

void
ProcessProgressDialog::start (QStringList& args)
{
  _buffer.clear ();
  _error.clear  ();

  _canceled = false;

  log << args.join (" ") + "\n";

  show ();
  _proc.start (args.takeFirst (), args);
  exec ();

}

void
ProcessProgressDialog::onProcStarted () { }

void
ProcessProgressDialog::onProcError (QProcess::ProcessError n)
{
  if (!_canceled)
    {
      hide ();
      NomNom::crit (parentWidget (), NomNom::to_process_errmsg (n));
      emit error ();
    }
  cancel ();
}

void
ProcessProgressDialog::onProcReadyRead ()
{
  static char data[1024];

  while (_proc.readLine (data, sizeof (data)))
    {

      QString ln = QString::fromLocal8Bit (data);

      _buffer += ln;
      log     << ln;

      QHashIterator<QString, QRegExp> i (_rx_label);

      while (i.hasNext ())
        {

          i.next ();

          if (i.value ().indexIn (ln) != -1)
            setLabelText (i.key ());

        }

    }

}

void
ProcessProgressDialog::onProcFinished (
  int exitCode,
  QProcess::ExitStatus exitStatus)
{
  if (exitStatus == QProcess::NormalExit && exitCode == 0)
    emit finished (_buffer);

  else
    {

      if (!_canceled)
        {

          QString err = _buffer;

          if (_rx_error.indexIn (_buffer) != -1)
            err = "error: " + _rx_error.cap (1).simplified ();

          hide ();
          NomNom::crit (parentWidget (), err);

          emit error ();
        }

    }

  cancel ();
}

void
ProcessProgressDialog::onCanceled ()
{
  _canceled = true;
  if (_proc.state () == QProcess::Running)
    _proc.kill ();
}

// vim: set ts=2 sw=2 tw=72 expandtab:
