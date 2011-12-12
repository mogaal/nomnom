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

#ifndef nsettingsdialog_h
#define nsettingsdialog_h

#include <QDialog>

class QButtonGroup;
class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QTabWidget;
class QCheckBox;
class QComboBox;
class QLineEdit;
class QGroupBox;
class QToolBox;
class QSpinBox;

namespace nn
{

class NTripwireEdit;

typedef enum { From=0x00, To } Mode;

class NSettingsDialog : public QDialog
{
  Q_OBJECT
public:
  NSettingsDialog(QWidget *parent=NULL);
protected:
  void done(int);
private:
  bool foreachWidget(Mode);
  void fromConfig();
  bool toConfig();
private:
  QToolBox *toolbox;
};

class NSettingsWidget : public QWidget
{
  Q_OBJECT
public:
  NSettingsWidget(QWidget *parent);
protected:
  virtual bool verify(QString&) = 0;
  virtual void write() = 0;
  virtual void read()  = 0;
  virtual void init()  = 0;
// Friends
  friend class NSettingsDialog;
};

class NSettingsCommands : public NSettingsWidget
{
  Q_OBJECT
public:
  NSettingsCommands(QWidget *parent=NULL);
protected:
  virtual bool verify(QString&);
  virtual void write();
  virtual void read();
  virtual void init();
private slots:
  void downloadUsingChanged(QString);
  void parseUsingChanged(QString);
  void playUsingChanged(QString);
  void feedUsingChanged(QString);
private:
  QComboBox *downloadUsingCombo;
  QLineEdit *downloadUsingEdit;
  QComboBox *parseUsingCombo;
  QLineEdit *parseUsingEdit;
  QComboBox *playUsingCombo;
  QLineEdit *playUsingEdit;
  QComboBox *feedUsingCombo;
  QLineEdit *feedUsingEdit;
};

class NSettingsDownload : public NSettingsWidget
{
  Q_OBJECT
public:
  NSettingsDownload(QWidget *parent=NULL);
protected:
  virtual bool verify(QString&);
  virtual void write();
  virtual void read();
  virtual void init();
private:
  void addGridRow(QGridLayout*,
                  const QString&,
                  QComboBox**,
                  QLineEdit**,
                  const char*);
private slots:
  void regexpChanged(int);
  void fnfmtChanged(int);
  void updateExample();
  void browseDir();
private:
  QComboBox *fnfmtCombo;
  QLineEdit *fnfmtEdit;
  QComboBox *regexpCombo;
  QLineEdit *regexpEdit;
  QLineEdit *exampleEdit;
  QLineEdit *savedirEdit;
};

class NSettingsProxy : public NSettingsWidget
{
  Q_OBJECT
public:
  NSettingsProxy(QWidget *parent=NULL);
protected:
  virtual bool verify(QString&);
  virtual void write();
  virtual void read();
  virtual void init();
private slots:
  void enableAddress(bool state=true);
  void typeChanged(int);
private:
  QGroupBox *addressGroup;
  QLineEdit *addressEdit;
  QButtonGroup *bgroup;
  QSpinBox *portSpin;
};

class NSettingsOptions : public NSettingsWidget
{
  Q_OBJECT
public:
  NSettingsOptions(QWidget *parent=NULL);
protected:
  virtual bool verify(QString&);
  virtual void write();
  virtual void read();
  virtual void init();
private:
  bool foreachWidget(Mode mode);
  void fromConfig();
  bool toConfig();
private:
  QTabWidget *tabWidget;
};

class NSettingsTabWidget : public QWidget
{
  Q_OBJECT
public:
  NSettingsTabWidget(QWidget *parent=NULL);
protected:
  virtual bool verify(QString&) = 0;
  virtual void write() = 0;
  virtual void read() = 0;
  virtual void init() = 0;
// Friends
  friend class NSettingsOptions;
};

class NSettingsAppearance : public NSettingsTabWidget
{
  Q_OBJECT
public:
  NSettingsAppearance(QWidget *parent=NULL);
protected:
  virtual bool verify(QString&);
  virtual void write();
  virtual void read();
  virtual void init();
private:
  void addGridRow(QGridLayout*, const QString&, NTripwireEdit**, const char*);
#ifdef _1
private slots:
  void browseError();
  void browseBusy();
  void browseApp();
#endif
private:
  NTripwireEdit *errorEdit;
  NTripwireEdit *busyEdit;
  NTripwireEdit *appEdit;
  QComboBox *langCombo;
};

class NSettingsBehaviour : public NSettingsTabWidget
{
  Q_OBJECT
public:
  NSettingsBehaviour(QWidget *parent=NULL);
protected:
  virtual bool verify(QString&);
  virtual void write();
  virtual void read();
  virtual void init();
private:
  void createWidgets();
private:
  QCheckBox *replaceExistingMediaBox;
  QCheckBox *clearURLRecordAtExitBox;
  QCheckBox *askWhereSaveMediaBox;
  QCheckBox *keepAppWinTopBox;
  QCheckBox *getBestFormatBox;
  QCheckBox *playWhenDoneBox;
};

class NSettingsSystray : public NSettingsTabWidget
{
  Q_OBJECT
public:
  NSettingsSystray(QWidget *parent=NULL);
protected:
  virtual bool verify(QString&);
  virtual void write();
  virtual void read();
  virtual void init();
private slots:
  void showClicked(bool);
private:
  QCheckBox *terminateInsteadBox;
  QGroupBox *showInTrayGroup;
  QCheckBox *startInTrayBox;
};

} // namespace nn

#endif

/* vim: set ts=2 sw=2 tw=72 expandtab: */
