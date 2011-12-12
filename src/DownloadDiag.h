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

#ifndef nomnom_downloaddiag_h
#define nomnom_downloaddiag_h

#include <QProgressDialog>
#include <QProcess>

#include "Media.h"

class DownloadDialog : public QProgressDialog
{
  Q_OBJECT
public:
  DownloadDialog(QWidget *parent=NULL);
public:
  void start(QStringList&);
  QString errmsg() const;
  bool canceled() const;
  int errcode() const;
  bool failed() const;
signals:
  void finished();
  void error();
private slots:
  void onCurlFinished(int, QProcess::ExitStatus);
  void onCurlError(QProcess::ProcessError);
  void onCurlReadyRead();
  void onCurlStarted();
  void onCanceled();
private:
  QStringList _args;
  QString _errmsg;
  QProcess _proc;
  bool _canceled;
  int _errcode;
};

#endif

// vim: set ts=2 sw=2 tw=72 expandtab:
