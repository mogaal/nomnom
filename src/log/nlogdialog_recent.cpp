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
#include <QApplication>
#include <QVBoxLayout>
#include <QTextStream>
#include <QTreeWidget>
#include <QMessageBox>
#include <QClipboard>

#include <NRecentMutator>
#include <NRecentEntry>
#include <NLogDialog>
#include <NSettings>
#include <NUtil>

extern nn::NRecentMutator recent;

namespace nn
{

NLogRecent::NLogRecent(QWidget *parent/*=NULL*/)
  : NLogWidget(parent), _treew(NULL)
{

// Widgets

  _treew = new QTreeWidget;
  _treew->setHeaderLabels(QStringList() << tr("Title") << tr("Added"));
  _treew->setColumnCount(2);

  connect(_treew, SIGNAL(itemSelectionChanged()), this, SLOT(selected()));

// Layout

  QVBoxLayout *box = new QVBoxLayout;
  box->addWidget(_treew);
  setLayout(box);
}

#ifdef _1
void NLogRecent::read()
{
  recent.read();
}
#endif

void NLogRecent::init()
{
  recent.populate(_treew);
  _treew->resizeColumnToContents(0);
  _treew->sortByColumn(1);
  _treew->setSortingEnabled(true);
}

void NLogRecent::selected()
{
  QTreeWidgetItem *i = _treew->currentItem();
  i = (i->childCount() > 0) ? i->child(0) : i;
  if (i)
    emit selected(i->text(0));
}

void NLogRecent::reset()
{
  if (_treew->topLevelItemCount() == 0)
    return;

  if (ask(this, tr("All records will be lost permanently. Really clear?"))
      != QMessageBox::Yes)
    {
      return;
    }

  _treew->clear();
  recent.clear();
}

void NLogRecent::copy()
{
  QTreeWidgetItem *i = _treew->currentItem();
  if (!i)
    {
      info(this, tr("Please select an item from the list"));
      return;
    }

  i = (i->childCount() > 0) ? i : i->parent();

  QApplication::clipboard()->setText(QString("%1\n  %2\n  %3\n")
                                     .arg(i->text(0))
                                     .arg(i->text(1))
                                     .arg((i->child(0)->text(0))));
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
