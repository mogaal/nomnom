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

#ifndef ntripwireedit_h
#define ntripwireedit_h

#include <QLineEdit>

class QMouseEvent;
class QKeyEvent;

namespace nn
{

class NTripwireEdit : public QLineEdit
{
  Q_OBJECT
public:
  NTripwireEdit(QWidget *parent=NULL);
protected:
  virtual void mousePressEvent(QMouseEvent*);
signals:
  void entered();
};

} // namespace nn

#endif // ntripwireedit_h

/* vim: set ts=2 sw=2 tw=72 expandtab: */
