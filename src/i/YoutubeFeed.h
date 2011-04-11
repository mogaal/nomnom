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

#ifndef nomnom_youtubefeed_h
#define nomnom_youtubefeed_h

#include <QPointer>

#include "ProcProgDiag.h"

#include "ui_YoutubeFeed.h"

class YoutubeFeed : public QDialog, private Ui::YoutubeFeed
{
  Q_OBJECT
public:
  YoutubeFeed (QWidget *parent);
public:
  bool gotItems () const;
private slots:
  void onTypeChanged (int);
  // Umph.
  void onFinished (QString);
protected:
  void done (int);
  void closeEvent (QCloseEvent*);
private:
  bool fromJSON (const QString&, QString&);
private:
  QPointer<ProcessProgressDialog> _proc;
  bool _got_items;
};

#endif

// vim: set ts=2 sw=2 tw=72 expandtab:
