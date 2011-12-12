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

#ifndef nerrorwhiledialog_h
#define nerrorwhiledialog_h

#include <QStringList>
#include <QDialog>

namespace nn
{

class NErrorWhileDialog : public QDialog
{
  Q_OBJECT
public:
  NErrorWhileDialog(const QStringList&,
                    const QString&,
                    const int,
                    QWidget *parent=NULL);
private slots:
  void copy();
private:
  QStringList _args;
  QString _errmsg;
  int _errcode;
};

} // namespace nn

#endif

/* vim: set ts=2 sw=2 tw=72 expandtab: */
