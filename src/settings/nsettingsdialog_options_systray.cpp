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
#include <QCheckBox>
#include <QGroupBox>

#include <NSettingsMutator>
#include <NSettingsDialog>
#include <NSysTray>

// main.cpp
extern nn::NSettingsMutator settings;
extern nn::NSysTray *systray;

namespace nn
{

NSettingsSystray::NSettingsSystray(QWidget *parent/*=NULL*/)
  : NSettingsTabWidget(parent)
{

// Widgets

  terminateInsteadBox =
    new QCheckBox(tr("T&erminate instead of minimizing to tray icon"));

  startInTrayBox =
    new QCheckBox(tr("&Start in tray icon"));

// Group box layout

  QVBoxLayout *sysBox = new QVBoxLayout;
  sysBox->addWidget(terminateInsteadBox);
  sysBox->addWidget(startInTrayBox);
  sysBox->addStretch(0);
  
// Systray group box

  showInTrayGroup = new QGroupBox(tr("S&how in system tray icon"));
  showInTrayGroup->setCheckable(true);
  showInTrayGroup->setLayout(sysBox);

  connect(showInTrayGroup, SIGNAL(clicked(bool)),
    this, SLOT(showClicked(bool)));

// Layout

  QVBoxLayout *box = new QVBoxLayout;
  box->addWidget(showInTrayGroup);
  box->addStretch(0);
  setLayout(box);
}

void NSettingsSystray::init()
{
}

static void _write(SettingKey k, QCheckBox *c)
{
  settings.setValue(k, c->checkState());
}

void NSettingsSystray::write()
{
  settings.setValue(ShowInTrayIcon, showInTrayGroup->isChecked());
  _write(TerminateInstead, terminateInsteadBox);
  _write(StartInTrayIcon,  startInTrayBox);
}

static void _read(SettingKey k, QCheckBox *c)
{
  const int n = settings.value(k).toInt();
  const Qt::CheckState s = static_cast<Qt::CheckState>(n);
  c->setCheckState(s);
}

void NSettingsSystray::read()
{
  showInTrayGroup->setChecked(settings.value(ShowInTrayIcon).toBool());
  _read(TerminateInstead, terminateInsteadBox);
  _read(StartInTrayIcon,  startInTrayBox);
}

bool NSettingsSystray::verify(QString&)
{
  return true;
}

void NSettingsSystray::showClicked(bool checked)
{
  terminateInsteadBox->setEnabled(checked);
  startInTrayBox->setEnabled(checked);
  systray->setVisible(checked);
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
