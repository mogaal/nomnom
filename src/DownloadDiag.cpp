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

#include <QCoreApplication>

#ifdef ENABLE_VERBOSE
#include <QDebug>
#endif

#include "DownloadDiag.h"

DownloadDialog::DownloadDialog(QWidget *parent/*=NULL*/)
  : QProgressDialog(parent), _canceled(false), _errcode(-1)
{
  _proc.setProcessChannelMode(QProcess::MergedChannels);

#define _wrap(s,sl) \
    do { connect(&_proc, SIGNAL(s), this, SLOT(sl)); } while (0)
  _wrap(started(), onCurlStarted());
  _wrap(error(QProcess::ProcessError), onCurlError(QProcess::ProcessError));
  _wrap(readyRead(), onCurlReadyRead());
  _wrap(finished(int, QProcess::ExitStatus),
        onCurlFinished(int, QProcess::ExitStatus));
#undef _wrap

  connect(this, SIGNAL(canceled()), this, SLOT(onCanceled()));

  setWindowModality(Qt::WindowModal);
  setAutoClose(false);
}

bool DownloadDialog::canceled() const
{
  return _canceled;
}

bool DownloadDialog::failed() const
{
  return !_errmsg.isEmpty();
}

QString DownloadDialog::errmsg() const
{
  return _errmsg;
}

int DownloadDialog::errcode() const
{
  return _errcode;
}

void DownloadDialog::start(QStringList& args)
{
#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "args=" << args;
#endif
  _canceled = false;

  _errmsg.clear();
  _args = args;

  show();
#ifdef _1
  _proc.setProcessEnvironment(NomNom::proc_env());
#endif
  _proc.start(args.takeFirst(), args);
  exec();
}

void DownloadDialog::onCurlStarted()
{
  setLabelText(tr("Starting download..."));
}

void DownloadDialog::onCurlError(QProcess::ProcessError n)
{
#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "code=" << n;
#endif
  if (!_canceled)
    {
      hide();
      _errmsg = _proc.errorString();
      _errcode = n;
      emit error();
    }
  cancel();
}

static const QRegExp rx_rate("(\\D+)");   // rate unit

static void update_label(QProgressDialog *d, const QString& ln)
{
  QStringList lst = ln.split(" ");
  if (lst.count() < 12)
    return; // Full line updates only.

#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "lst=" << lst;
#endif

  enum {PERCENT=0, ETA=10, RATE=11};

  QString rate = lst[RATE];
  if (rx_rate.indexIn(rate) == -1)
    rate = QString("%1k").arg(rate.toLongLong()/1024.0,2,'f',1);

  const QString s = qApp->translate("DownloadDialog", "Copying %1/s, %2")
                    .arg(rate).arg(lst[ETA]);

#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "s=" << s;
#endif

  d->setValue(lst[PERCENT].toInt());
  d->setLabelText(s);
}

static const QRegExp rx_err("curl:\\s+(.*)$");

void DownloadDialog::onCurlReadyRead()
{
  static char data[1024];
  while (_proc.readLine(data, sizeof(data)))
    {
      const QString ln = QString::fromLocal8Bit(data).simplified();

#ifdef ENABLE_VERBOSE
      qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "ln=" << ln;
#endif

      if (rx_err.indexIn(ln) != -1)
        {
          _errmsg = rx_err.cap(1);
          _errcode = -1;
          break;
        }
      update_label(this, ln);
    }
}

void DownloadDialog::onCurlFinished(int ec , QProcess::ExitStatus es)
{
  if (es == QProcess::NormalExit && ec == 0)
    emit finished();
  else
    {
      if(!_canceled)
        emit error();
    }
  cancel();
}

void DownloadDialog::onCanceled()
{
  _canceled = true;
  if (_proc.state() == QProcess::Running)
    _proc.kill();
}

// vim: set ts=2 sw=2 tw=72 expandtab:
