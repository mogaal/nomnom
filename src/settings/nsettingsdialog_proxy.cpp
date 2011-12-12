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

#include <QButtonGroup>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>

#include <NSettingsMutator>
#include <NSettingsDialog>

extern nn::NSettingsMutator settings; // main.cpp

namespace nn
{

NSettingsProxy::NSettingsProxy(QWidget *parent/*=NULL*/)
  : NSettingsWidget(parent)
{

// Widgets

  QRadioButton *autodetect = new QRadioButton(
    tr("&Autodetect (environment, e.g. http_proxy)"));

  QRadioButton *no_proxy = new QRadioButton(tr("&No proxy"));
  QRadioButton *manual = new QRadioButton(tr("Man&ual"));

// Button group

  bgroup = new QButtonGroup;
  bgroup->addButton(autodetect, AutodetectProxy);
  bgroup->addButton(no_proxy, NoProxy);
  bgroup->addButton(manual, ManualProxy);

  connect(bgroup, SIGNAL(buttonClicked(int)), this, SLOT(typeChanged(int)));

// Address

  QLabel *l = new QLabel(tr("&Host:"));
  addressEdit = new QLineEdit;
  l->setBuddy(addressEdit);

  QGridLayout *addrGrid = new QGridLayout;
  addrGrid->addWidget(l, 0, 0);
  addrGrid->addWidget(addressEdit, 0, 1);

  l = new QLabel(tr("Po&rt:"));
  portSpin = new QSpinBox;
  portSpin->setMaximum(99999);
  l->setBuddy(portSpin);

  addrGrid->addWidget(l, 1, 0);
  addrGrid->addWidget(portSpin, 1, 1);

  addressGroup = new QGroupBox(tr("Proxy address"));
  addressGroup->setLayout(addrGrid);

// Layout

  QVBoxLayout *box = new QVBoxLayout;
  box->addWidget(autodetect);
  box->addWidget(no_proxy);
  box->addWidget(manual);
  box->addWidget(addressGroup);
  box->addStretch(0);
  setLayout(box);
}

void NSettingsProxy::init()
{
  typeChanged(bgroup->checkedId());
}

bool NSettingsProxy::verify(QString& msg)
{
  if (bgroup->checkedId() == ManualProxy)
    {
      if (addressEdit->text().isEmpty())
        msg = tr("Please fill the required field");
    }
  return msg.isEmpty();
}

void NSettingsProxy::write()
{
  settings.setValue(ProxyType, bgroup->checkedId());
  settings.setValue(ProxyHost, addressEdit->text());
  settings.setValue(ProxyPort, portSpin->value());
}

void NSettingsProxy::read()
{
  const int n = settings.value(ProxyType).toInt();
  dynamic_cast<QRadioButton*>(bgroup->button(n))->setChecked(true);
  addressEdit->setText(settings.value(ProxyHost).toString());
  portSpin->setValue(settings.value(ProxyPort).toInt());
}

void NSettingsProxy::enableAddress(bool state/*=true*/)
{
  addressGroup->setEnabled(state);
}

void NSettingsProxy::typeChanged(int n)
{
  enableAddress(n == ManualProxy);
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
