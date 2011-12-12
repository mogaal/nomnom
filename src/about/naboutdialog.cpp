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

#include <QDialogButtonBox>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QDebug>
#include <QFile>

#include <NAboutDialog>

namespace nn
{

NAboutDialog::NAboutDialog(const QString& version,
                           const QString& www,
                           QWidget *parent/*=NULL*/)
  : QDialog(parent)
{

// Details.

  QGridLayout *grid = new QGridLayout;

  grid->addWidget(new QLabel(tr("<b>Version</b>:")), 0, 0);
  grid->addWidget(new QLabel(version), 0, 1);

  grid->addWidget(new QLabel("<b>WWW</b>:"), 1, 0);
  const QString _www = QString("<a href=\"%1\">%1</a>").arg(www);
  grid->addWidget(new QLabel(_www), 1, 1);

// Button box.

  QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok);

  QPushButton *b = new QPushButton(tr("&License"));
  btnBox->addButton(b, QDialogButtonBox::ActionRole);

  connect(btnBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(b, SIGNAL(clicked()), this, SLOT(license()));

// Layout.

  QVBoxLayout *box = new QVBoxLayout;
  box->addLayout(grid);
  box->addWidget(new QLabel("Copyright (C) 2010-2011  Toni Gundogdu"));
  box->addWidget(btnBox);
  box->addStretch(0);
  setLayout(box);

  setWindowTitle(tr("About"));
}

void NAboutDialog::license()
{
  NLicenseDialog *d = new NLicenseDialog(this);
  d->exec();
}

NLicenseDialog::NLicenseDialog(QWidget *parent)
  : QDialog(parent)
{

// Text view.

  QTextEdit *e = new QTextEdit;

  QFile f(":/text/COPYING");
  if (f.open(QIODevice::ReadOnly | QIODevice::Text))
    e->setPlainText(f.readAll());
  e->setReadOnly(true);

// Button box.

  QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok);
  connect(btnBox, SIGNAL(accepted()), this, SLOT(accept()));

// Layout.

  QVBoxLayout *box = new QVBoxLayout;
  box->addWidget(e);
  box->addWidget(btnBox);
  setLayout(box);

  setMinimumSize(QSize(600,400));
  setWindowTitle(tr("License"));
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
