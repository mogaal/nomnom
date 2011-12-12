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
#include <QFileDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QLabel>

#include <NSettingsMutator>
#include <NSettingsDialog>
#include <NTripwireEdit>

extern nn::NSettingsMutator settings; // main.cpp

namespace nn
{

NSettingsDownload::NSettingsDownload(QWidget *parent/*=NULL*/)
  : NSettingsWidget(parent),
    fnfmtCombo(NULL),
    fnfmtEdit(NULL),
    regexpCombo(NULL),
    regexpEdit(NULL),
    exampleEdit(NULL),
    savedirEdit(NULL)
{

// Save as

  QGroupBox *fngBox = new QGroupBox(tr("Save downloaded media as"));

  QGridLayout *g = new QGridLayout;

  addGridRow(g, tr("&Filename format:"),
             &fnfmtCombo, &fnfmtEdit, SLOT(fnfmtChanged(int)));

  addGridRow(g, tr("&Regular expression:"),
             &regexpCombo, &regexpEdit, SLOT(regexpChanged(int)));

  addGridRow(g, tr("Example:"), NULL, &exampleEdit, NULL);

  fngBox->setLayout(g);

  fnfmtCombo->addItem(tr("Default"), "%t.%s");
  fnfmtCombo->addItem(tr("With media ID"), "%t_%i.%s");
  fnfmtCombo->addItem(tr("With ID, domain"), "%t_%i_%d.%s");

  regexpCombo->addItem(tr("Default"), "/(\\w|\\s)/g");
  regexpCombo->addItem(tr("Word characters"), "/(\\w)/g");
  regexpCombo->addItem(tr("Digit characters"), "/(\\d)/g");

  exampleEdit->setReadOnly(true);

// Save in

  savedirEdit = new NTripwireEdit;
  savedirEdit->setReadOnly(true);

  connect(savedirEdit, SIGNAL(entered()), this, SLOT(browseDir()));

  QHBoxLayout *dirBox = new QHBoxLayout;
  dirBox->addWidget(savedirEdit);
  dirBox->addStretch(0);

  QGroupBox *dirgBox = new QGroupBox(tr("&Save downloaded media in"));
  dirgBox->setLayout(dirBox);

// Layout

  QVBoxLayout *box = new QVBoxLayout;
  box->addWidget(fngBox);
  box->addWidget(dirgBox);
  box->addStretch(0);
  setLayout(box);
}

void NSettingsDownload::init()
{
  if (fnfmtEdit->text().isEmpty())
    fnfmtChanged(0);

  if (regexpEdit->text().isEmpty())
    regexpChanged(0);

  updateExample();

  if (savedirEdit->text().isEmpty())
    savedirEdit->setText(QDir::homePath());
}

static bool _verify(QLineEdit *edit, QString& msg)
{
  if (edit->text().isEmpty())
    {
      msg = qApp->translate("nn::NSettingsDownload",
                            "Please fill the required fields");
    }
  return msg.isEmpty();
}

bool NSettingsDownload::verify(QString& msg)
{
  bool r = _verify(fnfmtEdit, msg);
  if (r)
    r = _verify(regexpEdit, msg);
  return r;
}

static void _write(SettingKey k, QLineEdit *e)
{
  settings.setValue(k, e->text());
}

void NSettingsDownload::write()
{
// Filename
  _write(FilenameFormat, fnfmtEdit);
  _write(FilenameRegExp, regexpEdit);
// Directory
  _write(SaveMediaDirectory, savedirEdit);
}

static void _read(SettingKey key, QLineEdit *edit, QComboBox *combo)
{
  const QString text = settings.value(key).toString();

  if (combo)
    {
      const int n = combo->findData(text);
      if (n != -1)
        combo->setCurrentIndex(n);
    }

  edit->setText(text);
}

void NSettingsDownload::read()
{
// Filename
  _read(FilenameFormat, fnfmtEdit, fnfmtCombo);
  _read(FilenameRegExp, regexpEdit, regexpCombo);
// Directory
  _read(SaveMediaDirectory, savedirEdit, NULL);
}

void NSettingsDownload::addGridRow(
  QGridLayout *g,
  const QString& text,
  QComboBox **c,
  QLineEdit **e,
  const char *slot)
{
  QLabel *l = new QLabel(text);
  *e = new QLineEdit;
  l->setBuddy(*e);

  if (c)
    *c = new QComboBox;

  if (slot)
    {
      connect(*c, SIGNAL(currentIndexChanged(int)),
              this, slot);
    }

  connect(*e, SIGNAL(editingFinished()),
          this, SLOT(updateExample()));

  const int row = g->rowCount() + 1;

  g->addWidget(l,  row, 0);
  g->addWidget(*e, row, 1);
  if (c)
    g->addWidget(*c, row, 2);
}

void NSettingsDownload::fnfmtChanged(int n)
{
  fnfmtEdit->setText(fnfmtCombo->itemData(n).toString());
  updateExample();
}

void NSettingsDownload::regexpChanged(int n)
{
  regexpEdit->setText(regexpCombo->itemData(n).toString());
  updateExample();
}

static const QString e_title    = "Foo bar";
static const QString e_domain   = "baz";
static const QString e_media_id = "abcd1234";
static const QString e_suffix   = "flv";

void NSettingsDownload::updateExample()
{
  if (!fnfmtEdit || !regexpEdit || !exampleEdit)
    return;

  QString fname = fnfmtEdit->text();
  const bool r  = format_filename(regexpEdit->text(),
                                  e_title,
                                  e_media_id,
                                  e_domain,
                                  e_suffix,
                                  fname);
  if (!r)
    return;

  exampleEdit->setText(fname);
}

void NSettingsDownload::browseDir()
{
  const QString dir = QFileDialog::getExistingDirectory(
                        this,
                        tr("Open directory"),
                        QDir::homePath(),
                        QFileDialog::ShowDirsOnly
                        | QFileDialog::DontResolveSymlinks);
  if (!dir.isEmpty())
    savedirEdit->setText(dir);
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
