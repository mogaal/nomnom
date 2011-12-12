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

#ifndef naboutdialog_h
#define naboutdialog_h

#include <QDialog>

namespace nn
{

class NAboutDialog : public QDialog
{
  Q_OBJECT
public:
  NAboutDialog(const QString&, const QString&, QWidget *parent=NULL);
private slots:
  void license();
};

class NLicenseDialog : public QDialog
{
  Q_OBJECT
public:
  NLicenseDialog(QWidget *parent);
};

} // namespace nn

#endif

/* vim: set ts=2 sw=2 tw=72 expandtab: */
