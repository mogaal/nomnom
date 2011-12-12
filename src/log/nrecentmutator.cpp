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
#include <QTreeWidget>
#include <QSettings>

#ifdef ENABLE_VERBOSE
#include <QDebug>
#endif

#include <qjson/qobjecthelper.h>
#include <qjson/serializer.h>
#include <qjson/parser.h>

#include <NRecentMutator>
#include <NRecentEntry>

namespace nn
{

enum { MAX_ITEMS=10 };

NRecentMutator::NRecentMutator()
  : _maxItems(MAX_ITEMS)
{
}

NRecentMutator& NRecentMutator::operator<<(const NRecentEntry& e)
{
  append(e);
  return *this;
}

void NRecentMutator::setMaxItems(const int n)
{
  _maxItems = n;
}

void NRecentMutator::append(const NRecentEntry& e,
                            const bool ignoreLimit/*=false*/)
{
  foreach (const NRecentEntry *r, _entries)
  {
    if (r->url() == e.url())
      return; // Found duplicate.
  }

  NRecentEntry *r = new NRecentEntry(e);
  _entries.prepend(r);

  if (!ignoreLimit && _entries.size() > _maxItems)
    _entries.takeLast();

#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "size=" << _entries.size();
#endif
}

void NRecentMutator::update(const NRecentEntry& e)
{
  foreach (NRecentEntry *r, _entries)
  {
    if (r->url() == e.url())
      {
        *r = e; // Update existing entry.
        return;
      }
  }
}

void NRecentMutator::clear()
{
  _entries.clear();
  write();
}

void NRecentMutator::write()
{
  QVariantList l;
  foreach (const NRecentEntry *r, _entries)
  {
    l << QJson::QObjectHelper::qobject2qvariant(r);
  }
  QSettings s;
  s.setValue("Recent/data", QJson::Serializer().serialize(l));
}

void NRecentMutator::read()
{
  _entries.clear();

  QSettings s;
  if (!s.contains("Recent/data"))
    return;

  QJson::Parser p;
  bool ok;

  QVariant d = p.parse(s.value("Recent/data").toByteArray(), &ok);
  if (ok)
    {
      foreach (const QVariant v, d.toList())
      {
        NRecentEntry e;
        QJson::QObjectHelper::qvariant2qobject(v.toMap(), &e);
        append(e, true);
      }
    }
}

void NRecentMutator::for_each(foreach_callback fp)
{
  foreach (const NRecentEntry *r, _entries)
  {
    (fp)(*r);
  }
}

void NRecentMutator::populate(QTreeWidget *w)
{
  w->clear();
  foreach (const NRecentEntry *r, _entries)
  {
    const QString t = r->title();

    QTreeWidgetItem *i = new QTreeWidgetItem(w);
    i->setText(0, t.isEmpty()
               ? qApp->translate("nn::NRecentMutator", "(no title)")
               : t);
    i->setText(1, r->added().toString(Qt::ISODate));

    QTreeWidgetItem *j = new QTreeWidgetItem(i);
    j->setText(0, r->url());
  }
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
