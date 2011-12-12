/*
 * NomNom
 * Copyright (C) 2010-2011  Toni Gundogdu <legatvs@gmail.com>
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

#ifdef ENABLE_VERBOSE
#include <QDebug>
#endif

#include "ProcProgDiag.h"

static QHash<QString,QRegExp> rx_labels;

ProcessProgressDialog::ProcessProgressDialog(QWidget *parent/*=NULL*/)
  : QProgressDialog(parent), _canceled(false), _errcode(-1)
{
  _proc.setProcessChannelMode(QProcess::MergedChannels);

#define _conn(s,sl) \
    do { connect(&_proc, SIGNAL(s), this, SLOT(sl)); } while (0)
  _conn(started(), onProcStarted());
  _conn(error(QProcess::ProcessError), onProcError(QProcess::ProcessError));
  _conn(readyRead(), onProcReadyRead());
  _conn(finished(int, QProcess::ExitStatus),
        onProcFinished(int, QProcess::ExitStatus));
#undef _conn

  connect(this, SIGNAL(canceled()), this, SLOT(onCanceled()));

  setWindowModality(Qt::WindowModal);
  setAutoClose(false);

#define _wrap(s,r) do { rx_labels[s] = QRegExp(r); } while (0)
  _wrap(tr("Checking..."),  "^:: Check");
  _wrap(tr("Fetching..."),  "^:: Fetch");
  _wrap(tr("Verifying..."), "^:: Verify");
#undef _wrap
}

bool ProcessProgressDialog::canceled() const
{
  return _canceled;
}

bool ProcessProgressDialog::failed() const
{
  return !_errmsg.isEmpty();
}

QString ProcessProgressDialog::errmsg() const
{
  return _errmsg;
}

int ProcessProgressDialog::errcode() const
{
  return _errcode;
}

void ProcessProgressDialog::start(QStringList& args)
{
#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "args=" << args;
#endif
  _canceled = false;

  _buffer.clear();
  _errmsg.clear();
  _args = args;

  show();
#ifdef _1
  _proc.setProcessEnvironment(NomNom::proc_env());
#endif
  _proc.start(args.takeFirst(), args);
  exec();
}

void ProcessProgressDialog::onProcStarted()
{
#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__;
#endif
}

void ProcessProgressDialog::onProcError(QProcess::ProcessError n)
{
#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "code=" << n;
#endif
  if (!_canceled)
    {
      hide();
      _errcode = static_cast<int>(n);
      _errmsg  = _proc.errorString();
      emit error();
    }
  cancel();
}

static void update_label(QProgressDialog *d,
                         const QString& ln)
{
  QHashIterator<QString, QRegExp> i(rx_labels);
  while (i.hasNext())
    {
      i.next();
      if (i.value().indexIn(ln) != -1)
        d->setLabelText(i.key());
    }
}

void ProcessProgressDialog::onProcReadyRead()
{
  static char data[1024];
  while (_proc.readLine(data, sizeof(data)))
    {
      const QString ln = QString::fromLocal8Bit(data);
      update_label(this, ln);
      _buffer += ln;
#ifdef ENABLE_VERBOSE
      qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "ln=" << ln;
#endif
    }
}

static QRegExp rx_error("error:\\s+(.*)$");

void ProcessProgressDialog::onProcFinished(int ec, QProcess::ExitStatus es)
{
  if (es == QProcess::NormalExit && ec == 0)
    emit finished(_buffer);
  else
    {
      if (!_canceled)
        {
          hide();
          _errmsg = _buffer;
          if (rx_error.indexIn(_buffer) != -1)
            {
              _errmsg  = rx_error.cap(1).simplified();
              _errcode = static_cast<int>(ec);
            }
#ifdef ENABLE_VERBOSE
          qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "errmsg=" << _errmsg;
#endif
          emit error();
        }
    }
  cancel();
}

void ProcessProgressDialog::onCanceled()
{
  _canceled = true;
  if (_proc.state() == QProcess::Running)
    _proc.kill();
}

// vim: set ts=2 sw=2 tw=72 expandtab:
