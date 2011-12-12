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

#include <NRecentEntry>

namespace nn
{

NRecentEntry::NRecentEntry(QObject *parent/*=NULL*/)
  : QObject(parent), _added(QDateTime::currentDateTime())
{
}

NRecentEntry::NRecentEntry(const NRecentEntry& e, QObject *parent/*=NULL*/)
  : QObject(parent)
{
  _copy(e);
}

NRecentEntry& NRecentEntry::operator=(const NRecentEntry& e)
{
  if (this != &e) _copy(e);
  return *this;
}

void NRecentEntry::_copy(const NRecentEntry& e)
{
  _added = e.added();
  _title = e.title();
  _url   = e.url();
}

NRecentEntry::~NRecentEntry()
{
}

void NRecentEntry::setAdded(const QDateTime& d)
{
  _added = d;
}

QDateTime NRecentEntry::added() const
{
  return _added;
}

void NRecentEntry::setTitle(const QString& t)
{
  _title = t;
}

QString NRecentEntry::title() const
{
  return _title;
}

void NRecentEntry::setURL(const QString& u)
{
  _url = u;
}

QString NRecentEntry::url() const
{
  return _url;
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
