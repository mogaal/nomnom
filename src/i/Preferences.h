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

#ifndef nomnom_preferences_h
#define nomnom_preferences_h

#include "ui_Preferences.h"

class Preferences : public QDialog, private Ui::Preferences
{
  Q_OBJECT
public:
  Preferences(QWidget *parent);
public:
  bool restartAfter () const;
protected:
  void done(int);
  void closeEvent(QCloseEvent*);
private slots:
  void onBrowsePlayer         ();
  void onBrowseQuvi           ();
  void onBrowseCurl           ();
  void onBrowseUmph           ();
  void onBrowseSaveDir        ();
  void onFilenameFormatChanged(int);
  void onRegexpChanged        (int);
  void onFilenameFormatEditingFinished ();
  void onRegexpEditingFinished();
  void onProgramIconChanged   (int);
  void onBrowseProgramIcon    ();
  void onRefresh              ();
  void onClearRecent          ();
  void onLanguage             ();
private:
  bool _restartAfter;
};

class SharedPreferences
{
public:
  enum Option
  {
    QuviPath = 0,
    CurlPath,
    PlayerPath,
    UmphPath,
    GrakePath,
    SaveDir,
    FilenameFormat,
    Regexp,
    CustomProgramIcon,
    ProgramIconPath,
    MinWhenStarts,
    MinToTray,
    StayOnTop,
    PlayWhenDone,
    ShowReminder,
    DontPromptFilename
  };
public:
  void     write   () const;
  void     read    ();
  void     set     (Option, const QVariant&);
  QVariant get     (Option) const;
private:
  QString quviPath;
  QString curlPath;
  QString playerPath;
  QString umphPath;
  QString saveDir;
  QString filenameFormat;
  QString regexp;
  bool    customProgramIcon;
  QString programIconPath;
  bool    minWhenStarts;
  bool    minToTray;
  bool    stayOnTop;
  bool    playWhenDone;
  bool    showReminder;
  bool    dontPromptFilename;
};

#endif

// vim: set ts=2 sw=2 tw=72 expandtab:
