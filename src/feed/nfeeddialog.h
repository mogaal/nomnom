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

#ifndef nfeeddialog_h
#define nfeeddialog_h

#include <QDialog>

class QButtonGroup;
class QListWidget;
class QComboBox;
class QGroupBox;
class QLineEdit;
class QToolBox;
class QSpinBox;
class QLabel;

namespace nn
{

class NFeedDialog : public QDialog
{
  Q_OBJECT
public:
  NFeedDialog(QWidget *parent, const QStringList&);
public:
  QString selected() const;
  QString errmsg() const;
protected:
  void done(int);
private:
  bool foreachWidget();
private slots:
  void parse(QStringList);
  void selected(QString);
signals:
  void parsed();
private:
  QToolBox *_toolbox;
  QString _selected;
};

class NFeedWidget : public QWidget
{
  Q_OBJECT
public:
  NFeedWidget(QWidget *parent);
protected:
  virtual void init() = 0;
// Friends
  friend class NFeedDialog;
};

class NFeedProperties : public NFeedWidget
{
  Q_OBJECT
public:
  NFeedProperties(const QStringList&, QWidget *parent=NULL);
protected:
  virtual void init();
private slots:
  void enableRange(bool state=true);
  void typeChanged(int); // Feed type
  void modeChanged(int); // "Get all" or "specified range"
  void parse();
  void reset();
signals:
  void parse(QStringList);
private:
  QGroupBox *_rangeGroup;
  QComboBox *_typeCombo;
  QLineEdit *_identEdit;
  QButtonGroup *_bgroup;
  QSpinBox *_indexSpin;
  QLabel *_identLabel;
  QSpinBox *_maxSpin;
  QStringList _args;
};

class NFeedItems : public NFeedWidget
{
  Q_OBJECT
public:
  NFeedItems(QWidget *parent=NULL);
protected:
  virtual void init();
public slots:
  void itemSelected();
  void update();
signals:
  void selected(QString);
private:
  QListWidget *_list;
};

} // namespace nn

#endif

/* vim: set ts=2 sw=2 tw=72 expandtab: */
