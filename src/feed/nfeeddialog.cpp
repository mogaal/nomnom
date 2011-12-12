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

#include <NFeedProgressDialog>
#include <NErrorWhileDialog>
#include <NFeedDialog>
#include <NSettings>
#include <NUtil>

extern nn::feed::NFeedList feedItems; // main.cpp

namespace nn
{

NFeedDialog::NFeedDialog(QWidget *parent, const QStringList& args)
  : QDialog(parent), _toolbox(NULL)
{

// Toolbox

  NFeedProperties *info = new NFeedProperties(args);
  connect(info, SIGNAL(parse(QStringList)), this, SLOT(parse(QStringList)));

  NFeedItems *items = new NFeedItems;
  connect(this, SIGNAL(parsed()), items, SLOT(update()));
  connect(items, SIGNAL(selected(QString)), this, SLOT(selected(QString)));

  _toolbox = new QToolBox;
  _toolbox->addItem(info, tr("&Properties"));
  _toolbox->addItem(items, tr("&Items"));

// Button box

  QDialogButtonBox *btnBox = new QDialogButtonBox(
    QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  connect(btnBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(btnBox, SIGNAL(rejected()), this, SLOT(reject()));

// Layout

  QVBoxLayout *box = new QVBoxLayout;
  box->addWidget(_toolbox);
  box->addWidget(btnBox);
  setLayout(box);

// Window

  setWindowTitle(tr("YouTube feed"));
  setMinimumSize(QSize(500,400));
  setSizeGripEnabled(true);
  foreachWidget();

  if (feedItems.count() > 0)
    _toolbox->setCurrentIndex(1);
}

void NFeedDialog::done(int n)
{
  if (n == QDialog::Accepted)
    {
      if (_selected.isEmpty())
        {
          if (feedItems.count() == 0)
            {
              _toolbox->setCurrentIndex(0);
              info(this, tr("Please read a feed"));
            }
          else
            {
              _toolbox->setCurrentIndex(1);
              info(this, tr("Please select an item from the list"));
            }
          return;
        }
    }
  QDialog::done(n);
  close();
}

bool NFeedDialog::foreachWidget()
{
  const int c = _toolbox->count();
  for (int i=0; i<c; ++i)
    {
      QWidget *w = _toolbox->widget(i);
      dynamic_cast<NFeedWidget*>(w)->init();
    }
  return true;
}

void NFeedDialog::parse(QStringList args)
{
  _selected.clear();

  NFeedProgressDialog *d = new NFeedProgressDialog(this);
  const bool r = d->open(args);

  if (d->cancelled())
    return;

  NErrorWhileDialog *ewd = NULL;

  if (r)
    {
      QString errmsg;
      if (!d->results(feedItems, errmsg))
        {
          ewd = new NErrorWhileDialog(args,
                                      errmsg,
                                      -1,
                                      this);
          ewd->exec();
        }
      else
        {
          _toolbox->setCurrentIndex(1);
          emit parsed();
        }
    }
  else
    {
      ewd = new NErrorWhileDialog(args,
                                  d->errmsg(),
                                  d->errcode(),
                                  this);
      ewd->exec();
    }
}

void NFeedDialog::selected(QString s)
{
  _selected = s;
}

QString NFeedDialog::selected() const
{
  return _selected;
}

NFeedWidget::NFeedWidget(QWidget *parent/*=NULL*/)
  : QWidget(parent)
{
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
