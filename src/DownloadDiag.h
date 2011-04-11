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

#ifndef nomnom_downloaddiag_h
#define nomnom_downloaddiag_h

#include <QProgressDialog>
#include <QProcess>

#include "Video.h"

class DownloadDialog : public QProgressDialog
{
  Q_OBJECT
public:
  DownloadDialog (QWidget *parent=NULL);
public:
  void start (const QString&, const QString&, Video*);
signals:
  void error ();
private slots:
  void onCurlStarted   ();
  void onCurlError     (QProcess::ProcessError);
  void onCurlReadyRead ();
  void onCurlFinished  (int, QProcess::ExitStatus);
  void onCanceled      ();
private:
  QString _lastError;
  QProcess _proc;
  bool _canceled;
};

#endif

// vim: set ts=2 sw=2 tw=72 expandtab:
