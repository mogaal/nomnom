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

#ifndef nomnom_mainwindow_h
#define nomnom_mainwindow_h

#include <QSystemTrayIcon>
#include <QHash>

#include "Media.h"

#include "ui_MainWindow.h"

class ProcessProgressDialog;
class DownloadDialog;
class QUrl;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT
public:
  MainWindow();
protected:
  void dragEnterEvent(QDragEnterEvent*);
  void closeEvent(QCloseEvent*);
  void dropEvent(QDropEvent*);
private:
  bool selectFormat(QStringList&, QString&, const bool);
  void handleURL(const QString&);
  bool queryFormats(QStringList&,
                    const QStringList&,
                    const QUrl&,
                    bool&);
  void createContextMenu();
  void changeProgramIcon();
  bool parseOK(QString&);
  void createTrayIcon();
  void downloadMedia();
  void streamMedia();
  void restore();
  void save();
private slots:
  // UI
  void activated(QSystemTrayIcon::ActivationReason);
  void onTerminate();
  void onSettings();
  void onAddress();
  void onRecent();
  void onAbout();
  void onFeed();
  // quvi.
  void onProcFinished(QString);
private:
  QHash<QString,QAction*> _actions;
  ProcessProgressDialog *_proc;
  DownloadDialog *_download;
  QString _json;
  Media _media;
};

#endif

// vim: set ts=2 sw=2 tw=72 expandtab:
