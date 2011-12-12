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

#include <NSettingsMutator>
#include <NSettingsDialog>

extern nn::NSettingsMutator settings; // main.cpp

namespace nn
{

NSettingsBehaviour::NSettingsBehaviour(QWidget *parent/*=NULL*/)
  : NSettingsTabWidget(parent)
{

// Widgets

  createWidgets();

// Layout

  QVBoxLayout *box = new QVBoxLayout;
  box->addWidget(clearURLRecordAtExitBox);
  box->addWidget(getBestFormatBox);
  box->addWidget(askWhereSaveMediaBox);
  box->addWidget(replaceExistingMediaBox);
  box->addWidget(keepAppWinTopBox);
  box->addWidget(playWhenDoneBox);
  box->addStretch(0);
  setLayout(box);
}

void NSettingsBehaviour::createWidgets()
{
  keepAppWinTopBox =
    new QCheckBox(tr("&Keep application window on top"));

  playWhenDoneBox =
    new QCheckBox(tr("Pla&y media when done downloading"));

  askWhereSaveMediaBox =
    new QCheckBox(tr("A&sk where to save media"));

  clearURLRecordAtExitBox =
    new QCheckBox(tr("C&lear record of visited URLs at exit"));

  replaceExistingMediaBox =
    new QCheckBox(tr("Al&ways replace existing media"));

  getBestFormatBox =
    new QCheckBox(tr("Always &get the best media format"));
}

void NSettingsBehaviour::init()
{
}

static void _write(SettingKey k, QCheckBox *c)
{
  settings.setValue(k, c->checkState());
}

void NSettingsBehaviour::write()
{
  _write(ReplaceExistingMedia,       replaceExistingMediaBox);
  _write(ClearURLRecordAtExit,       clearURLRecordAtExitBox);
  _write(AskWhereToSaveMediaFile,    askWhereSaveMediaBox);
  _write(KeepApplicationWindowOnTop, keepAppWinTopBox);
  _write(GetBestFormat,              getBestFormatBox);
  _write(PlayWhenDoneDownloading,    playWhenDoneBox);
}

static void _read(SettingKey k, QCheckBox *c)
{
  const int n = settings.value(k).toInt();
  const Qt::CheckState s = static_cast<Qt::CheckState>(n);
  c->setCheckState(s);
}

void NSettingsBehaviour::read()
{
  _read(ReplaceExistingMedia,       replaceExistingMediaBox);
  _read(ClearURLRecordAtExit,       clearURLRecordAtExitBox);
  _read(AskWhereToSaveMediaFile,    askWhereSaveMediaBox);
  _read(GetBestFormat,              getBestFormatBox);
  _read(KeepApplicationWindowOnTop, keepAppWinTopBox);
  _read(PlayWhenDoneDownloading,    playWhenDoneBox);
}

bool NSettingsBehaviour::verify(QString&)
{
  return true;
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
