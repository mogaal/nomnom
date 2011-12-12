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

#include <QListWidgetItem>
#include <QListWidget>
#include <QVBoxLayout>

#include <NFeedDialog>
#include <NFeed>

extern nn::feed::NFeedList feedItems; // main.cpp

namespace nn
{

NFeedItems::NFeedItems(QWidget *parent/*=NULL*/)
  : NFeedWidget(parent), _list(NULL)
{

// Widgets

  _list = new QListWidget;
  connect(_list, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelected()));

// Layout

  QVBoxLayout *box = new QVBoxLayout;
  box->addWidget(_list);
  box->addStretch(0);
  setLayout(box);
}

static void to_list(QListWidget *w, const feed::NFeedItem& i)
{
  QListWidgetItem *item = new QListWidgetItem(i.first);
  item->setData(Qt::UserRole, i.second);
  w->addItem(item);
}

void NFeedItems::init()
{
  update(); // Restore any old results from last session.
}

void NFeedItems::update()
{
  _list->clear();
  foreach (const feed::NFeedItem i, feedItems)
  {
    to_list(_list, i);
  }
}

void NFeedItems::itemSelected()
{
  QListWidgetItem *c = _list->currentItem();
  if (c)
    emit selected(c->data(Qt::UserRole).toString());
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
