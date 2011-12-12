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

#include <QCoreApplication>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QToolBox>

#include <NSettingsMutator>
#include <NSettingsDialog>
#include <NUtil>

extern nn::NSettingsMutator settings; // main.cpp

namespace nn
{

NSettingsDialog::NSettingsDialog(QWidget *parent/*=NULL*/)
  : QDialog(parent)
{

// Toolbox

  toolbox = new QToolBox;
  toolbox->addItem(new NSettingsOptions, tr("Op&tions"));
  toolbox->addItem(new NSettingsCommands, tr("Co&mmands"));
  toolbox->addItem(new NSettingsDownload, tr("&Download"));
#ifdef _1
  toolbox->addItem(new NSettingsProxy, tr("&Proxy"));
#endif

// Button box

  QDialogButtonBox *btnBox = new QDialogButtonBox(
    QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  connect(btnBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(btnBox, SIGNAL(rejected()), this, SLOT(reject()));

// Layout

  QVBoxLayout *box = new QVBoxLayout;
  box->addWidget(toolbox);
  box->addWidget(btnBox);
  setLayout(box);

// Window

  setWindowTitle(tr("Settings"));
  setMinimumSize(QSize(540,450));
  setSizeGripEnabled(true);
  fromConfig();
}

void NSettingsDialog::done(int n)
{
  if (n == QDialog::Accepted)
    {
      if (!toConfig())
        return;
    }
  QDialog::done(n);
  close();
}

bool NSettingsDialog::foreachWidget(Mode mode)
{
  const int c = toolbox->count();
  for (int i=0; i<c; ++i)
    {
      QWidget *w = toolbox->widget(i);

      NSettingsWidget *s = dynamic_cast<NSettingsWidget*>(w);

      QString msg;
      if (mode == To && !s->verify(msg))
        {
          toolbox->setCurrentWidget(s);
          info(this, msg);
          return false;
        }

      if (mode == From)
        {
          s->read();
          s->init();
        }
      else
        {
          s->write();
        }
    }
  return true;
}

void NSettingsDialog::fromConfig()
{
  foreachWidget(From);
}

bool NSettingsDialog::toConfig()
{
  const bool r = foreachWidget(To);
  if (r)
    settings.write();
  return r;
}

NSettingsWidget::NSettingsWidget(QWidget *parent/*=NULL*/)
  : QWidget(parent)
{
}

NSettingsTabWidget::NSettingsTabWidget(QWidget *parent/*=NULL*/)
  : QWidget(parent)
{
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
