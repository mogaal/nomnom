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

#ifndef nomnom_procprogdiag_h
#define nomnom_procprogdiag_h

#include <QProgressDialog>
#include <QProcess>
#include <QHash>
#include <QRegExp>

class ProcessProgressDialog : public QProgressDialog
{
  Q_OBJECT
public:
  ProcessProgressDialog (QWidget *parent);
public:
  void start          (QStringList&);
  void setLabelRegExp (const QHash<QString,QRegExp>&);
  void setErrorRegExp (const QRegExp&);
private slots:
  void onProcStarted   ();
  void onProcError     (QProcess::ProcessError);
  void onProcReadyRead ();
  void onProcFinished  (int, QProcess::ExitStatus);
  void onCanceled      ();
signals:
  void finished (QString);
  void error    ();
private:
  QHash<QString,QRegExp> _rx_label;
  QRegExp _rx_error;
  QString _buffer;
  QProcess _proc;
  QString _error;
  bool _canceled;
};

#endif

// vim: set ts=2 sw=2 tw=72 expandtab:
