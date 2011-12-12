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

#ifndef nomnom_procprogdiag_h
#define nomnom_procprogdiag_h

#include <QProgressDialog>
#include <QProcess>
#include <QHash>

class ProcessProgressDialog : public QProgressDialog
{
  Q_OBJECT
public:
  ProcessProgressDialog(QWidget *parent);
public:
  void start(QStringList&);
  QString errmsg() const;
  bool canceled() const;
  int errcode() const;
  bool failed() const;
private slots:
  void onProcFinished(int, QProcess::ExitStatus);
  void onProcError(QProcess::ProcessError);
  void onProcReadyRead();
  void onProcStarted();
  void onCanceled();
signals:
  void finished(QString);
  void error();
private:
  QStringList _args;
  QString _buffer;
  QString _errmsg;
  QProcess _proc;
  bool _canceled;
  int _errcode;
};

#endif

// vim: set ts=2 sw=2 tw=72 expandtab:
