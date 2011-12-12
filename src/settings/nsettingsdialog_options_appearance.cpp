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
#include <QImageReader>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>

#include <NSettingsMutator>
#include <NSettingsDialog>
#include <NTripwireEdit>
#include <NLang>

extern nn::NSettingsMutator settings; // main.cpp

namespace nn
{

NSettingsAppearance::NSettingsAppearance(QWidget *parent/*=NULL*/)
  : NSettingsTabWidget(parent)
{

#ifdef _1
// Icons

  QGridLayout *iconsBox = new QGridLayout;
  addGridRow(iconsBox, tr("Application:"), &appEdit,   SLOT(browseApp()));
  addGridRow(iconsBox, tr("Busy:"),        &busyEdit,  SLOT(browseBusy()));
  addGridRow(iconsBox, tr("Error:"),       &errorEdit, SLOT(browseError()));

  QGroupBox *gIcons = new QGroupBox(tr("Icons"));
  gIcons->setLayout(iconsBox);
#endif // _1

// Language

  langCombo = new QComboBox;
  langCombo->addItem("English");

  foreach (const lang::NResultPair p, lang::qm_files())
  {
    langCombo->addItem(p.first);
  }

  QHBoxLayout *langBox = new QHBoxLayout;
  langBox->addWidget(langCombo);
  langBox->addStretch(0);

  QGroupBox *gLang = new QGroupBox(tr("Language"));
  gLang->setLayout(langBox);

// Layout

  QVBoxLayout *box = new QVBoxLayout;
#ifdef _1
  box->addWidget(gIcons);
#endif
  box->addWidget(gLang);
  box->addStretch(0);
  setLayout(box);
}

void NSettingsAppearance::addGridRow(QGridLayout *g,
                                     const QString& text,
                                     NTripwireEdit **e,
                                     const char *slot)
{
  QLabel *l = new QLabel(text);
  *e = new NTripwireEdit;
  l->setBuddy(*e);

  (*e)->setReadOnly(true);
  (*e)->setToolTip(tr("Leave empty for application default"));

  connect(*e, SIGNAL(entered()), this, slot);

  QPushButton *b = new QPushButton;
  b->setIcon(QIcon(":/images/edit-clear.png"));
  b->setToolTip(tr("Clear"));

  connect(b, SIGNAL(clicked()), *e, SLOT(clear()));

  const int row = g->rowCount() + 1;

  g->addWidget(l,  row, 0);
  g->addWidget(*e, row, 1);
  g->addWidget(b,  row, 2);
}

void NSettingsAppearance::init()
{
}

void NSettingsAppearance::read()
{
#ifdef _1
  appEdit->setText(settings.value(CustomApplicationIcon).toString());
  busyEdit->setText(settings.value(CustomBusyIcon).toString());
  errorEdit->setText(settings.value(CustomErrorIcon).toString());
#endif
  const int n = langCombo->findText(settings.value(Language).toString());
  if (n != -1)
    langCombo->setCurrentIndex(n);
}

void NSettingsAppearance::write()
{
#ifdef _1
  settings.setValue(CustomApplicationIcon, appEdit->text());
  settings.setValue(CustomBusyIcon, busyEdit->text());
  settings.setValue(CustomErrorIcon, errorEdit->text());
#endif
  settings.setValue(Language, langCombo->currentText());
}

bool NSettingsAppearance::verify(QString&)
{
  return true;
}

static QString construct_filter()
{
  QString filter =
    qApp->translate("nn::NSettingsAppearance","Images") + " (";

  foreach(QByteArray b, QImageReader::supportedImageFormats())
  {
    filter += "*." +b+ " ";
  }

  filter = filter.simplified();
  filter += ")";

  return filter;
}

static bool browse(QWidget *parent, QString& fname)
{
  fname = QFileDialog::getOpenFileName(
            parent,
            qApp->translate("nn::NSettingsAppearance", "Open image"),
            QDir::homePath(),
            construct_filter());
  return ! fname.isEmpty();
}

#ifdef _1
void NSettingsAppearance::browseApp()
{
  QString fname;
  if (browse(this, fname))
    appEdit->setText(fname);
}

void NSettingsAppearance::browseBusy()
{
  QString fname;
  if (browse(this, fname))
    busyEdit->setText(fname);
}

void NSettingsAppearance::browseError()
{
  QString fname;
  if (browse(this, fname))
    errorEdit->setText(fname);
}
#endif // _1

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
