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

#ifndef nsystray_h
#define nsystray_h

#include <QSystemTrayIcon>

class QAction;
class QMenu;

namespace nn
{

class NSysTray : public QSystemTrayIcon
{
  Q_OBJECT
public:
  NSysTray(QObject *parent=NULL, const QString& text="");
  NSysTray& operator<<(const QString&);
public:
  void addTrayMenuAction(const char *signal,
                         QObject *receiver,
                         const char *method,
                         const QString& text,
                         const bool checkable=false);
  void addTrayMenuSeparator();
  QAction *findTrayMenuAction(const QString&);
  void setTrayMenu();
private:
  QMenu *trayMenu;
};

} // namespace nn

#endif

/* vim: set ts=2 sw=2 tw=72 expandtab: */
