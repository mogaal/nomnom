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

#include "config.h"

#include <QMenu>

#ifdef ENABLE_VERBOSE
#include <QDebug>
#endif

#include <NSysTray>

namespace nn
{

/*
 * Constructor: NSystray
 *
 * Initializes the object.
 */
NSysTray::NSysTray(QObject *parent/*=NULL*/, const QString& text/*=""*/)
  : QSystemTrayIcon(parent), trayMenu(NULL)
{
  trayMenu = new QMenu;
  if (!text.isEmpty())
    *this << text;
}

/*
 * Operator: <<
 *
 * Shorthand for setting the tooltip message.
 *
 * Parameters:
 *  text - Text to be used a a new tooltip message
 *
 * Returns:
 *  Instance reference
 */
NSysTray& NSysTray::operator<<(const QString& text)
{
  setToolTip(text);
  return *this;
}

/*
 * Function: addTrayMenuAction
 *
 * Adds a new action to the tray menu.
 *
 * Parameters:
 *  signal   - Signal to connect to
 *  receiver - Receiver
 *  method   - Method that handles the signal
 *  text     - Action text
 *  checkable - Whether the action should be checkable
 */
void NSysTray::addTrayMenuAction(const char *signal,
                                 QObject *receiver,
                                 const char *method,
                                 const QString& text,
                                 const bool checkable/*=false*/)
{
  QAction *action = new QAction(text, this);
  action->setCheckable(checkable);
  connect(action, signal, receiver, method);
  trayMenu->addAction(action);
}

/*
 * Function: addTrayMenuSeparator
 *
 * Adds a separator to the tray menu.
 */
void NSysTray::addTrayMenuSeparator()
{
  QAction *a = new QAction(this);
  a->setSeparator(true);
  trayMenu->addAction(a);
}

/*
 * Function: findTrayMenuAction
 *
 * Returns a matching tray menu action.
 *
 * Parameters:
 *  text - Action text to match
 *
 * Returns:
 *  Pointer to an action or NULL if nothing was matched
 */
QAction *NSysTray::findTrayMenuAction(const QString& text)
{
  foreach (QAction *action, trayMenu->actions())
  {
    if (action->text() == text)
      return action;
  }
#ifdef ENABLE_VERBOSE
  qWarning() << __PRETTY_FUNCTION__ << __LINE__
             << "Action \""
             << text
             << "\" not found in tray menu";
#endif
  return NULL;
}

/*
 * Function: setTrayMenu()
 *
 * Makes the created tray menu the context menu for the tray icon.
 */
void NSysTray::setTrayMenu()
{
  setContextMenu(trayMenu);
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
