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
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QToolBox>

#include <NLogDialog>
#include <NSettings>
#include <NUtil>

namespace nn
{

NLogDialog::NLogDialog(QWidget *parent/*=NULL*/)
  : QDialog(parent), _toolbox(NULL)
{

// Toolbox

  _toolbox = new QToolBox;

  NLogRecent *recent = new NLogRecent;
  connect(recent, SIGNAL(selected(QString)), this, SLOT(selected(QString)));

  _toolbox->addItem(recent, tr("&Recent"));

// Button box

  QDialogButtonBox *bb =
    new QDialogButtonBox(QDialogButtonBox::Ok
                         | QDialogButtonBox::Cancel
                         | QDialogButtonBox::Reset);

  QPushButton *b = new QPushButton(tr("Co&py"));
  b->setToolTip(tr("Copy to clipboard"));

  bb->addButton(b, QDialogButtonBox::ActionRole);

  connect(bb, SIGNAL(accepted()), this, SLOT(accept()));
  connect(bb, SIGNAL(rejected()), this, SLOT(reject()));
  connect(b, SIGNAL(clicked()), this, SLOT(copy()));

  connect(bb->button(QDialogButtonBox::Reset), SIGNAL(clicked()),
          this, SLOT(reset()));

// Layout

  QVBoxLayout *box = new QVBoxLayout;
  box->addWidget(_toolbox);
  box->addWidget(bb);
  setLayout(box);

// Window

  setMinimumSize(QSize(500,400));
  setWindowTitle(tr("Log"));
  setSizeGripEnabled(true);

  foreachWidget();
}

void NLogDialog::foreachWidget()
{
  const int c = _toolbox->count();
  for (int i=0; i<c; ++i)
    {
      NLogWidget *l = dynamic_cast<NLogWidget*>(_toolbox->widget(i));
      l->init();
    }
}

void NLogDialog::done(int n)
{
  if (n == QDialog::Accepted)
    {
      if (_selected.isEmpty())
        {
          _toolbox->setCurrentIndex(0);
          info(this, tr("Please select an item from the list"));
          return;
        }
    }
  QDialog::done(n);
  close();
}

void NLogDialog::selected(QString s)
{
  _selected = s;
}

QString NLogDialog::selected() const
{
  return _selected;
}

void NLogDialog::reset()
{
  NLogWidget *l = dynamic_cast<NLogWidget*>(_toolbox->currentWidget());
  l->reset();
}

void NLogDialog::copy()
{
  NLogWidget *l = dynamic_cast<NLogWidget*>(_toolbox->currentWidget());
  l->copy();
}

// NLogWidget

NLogWidget::NLogWidget(QWidget *parent/*=NULL*/)
  : QWidget(parent)
{
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
