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

#include <QVBoxLayout>
#include <QTabWidget>

#include <NSettingsMutator>
#include <NSettingsDialog>
#include <NUtil>

extern nn::NSettingsMutator settings; // main.cpp

namespace nn
{

NSettingsOptions::NSettingsOptions(QWidget *parent/*=NULL*/)
  : NSettingsWidget(parent)
{

// Tab widget

  tabWidget = new QTabWidget;
  tabWidget->addTab(new NSettingsAppearance, tr("&Appearance"));
  tabWidget->addTab(new NSettingsBehaviour, tr("&Behaviour"));
  tabWidget->addTab(new NSettingsSystray, tr("Sys&tray"));

// Layout

  QVBoxLayout *box = new QVBoxLayout;
  box->addWidget(tabWidget);
  box->addStretch(0);
  setLayout(box);
}

void NSettingsOptions::init()
{
}

bool NSettingsOptions::verify(QString&)
{
  return true;
}

void NSettingsOptions::write()
{
  toConfig();
}

void NSettingsOptions::read()
{
  fromConfig();
}

bool NSettingsOptions::foreachWidget(Mode mode)
{
  const int c = tabWidget->count();
  for (int i=0; i<c; ++i)
    {
      QWidget *w = tabWidget->widget(i);

      NSettingsTabWidget *o = dynamic_cast<NSettingsTabWidget*>(w);

      QString msg;
      if (!o->verify(msg))
        {
          tabWidget->setCurrentWidget(o);
          info(this, msg);
          return false;
        }

      if (mode == From)
        {
          o->read();
          o->init();
        }
      else
        {
          o->write();
        }
    }
  return true;
}

void NSettingsOptions::fromConfig()
{
  foreachWidget(From);
}

bool NSettingsOptions::toConfig()
{
  const bool r = foreachWidget(To);
  if (r)
    settings.write();
  return r;
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
