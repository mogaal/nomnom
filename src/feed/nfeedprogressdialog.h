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

#ifndef nfeedprogressdialog_h
#define nfeedprogressdialog_h

#include <QProgressDialog>
#include <QProcess>

#include <NFeed>

class QStringList;

namespace nn
{

class NFeedProgressDialog : public QProgressDialog
{
  Q_OBJECT
public:
  NFeedProgressDialog(QWidget *parent=NULL);
public:
  bool results(feed::NFeedList&, QString&);
  bool open(QStringList);
  QString errmsg() const;
  bool cancelled() const;
  int errcode() const;
private slots:
  void finished(int, QProcess::ExitStatus);
  void error(QProcess::ProcessError);
  void cleanup();
  void read();
private:
  QStringList _args;
  QString _buffer;
  QString _errmsg;
  QProcess *_proc;
  bool _cancelled;
  int _errcode;
};

} // namespace nn

#endif

/* vim: set ts=2 sw=2 tw=72 expandtab: */
