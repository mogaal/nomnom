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

#ifndef nrecentmutator_h
#define nrecentmutator_h

#include <QList>

class QTreeWidget;

namespace nn
{

class NRecentEntry;

class NRecentMutator
{
public:
  NRecentMutator();
  NRecentMutator& operator<<(const NRecentEntry&);
public:
  typedef void (*foreach_callback)(const NRecentEntry&);
public:
  void setMaxItems(const int);
  void append(const NRecentEntry&, const bool ignoreLimit=false);
  void update(const NRecentEntry&);
  void for_each(foreach_callback);
  void populate(QTreeWidget*);
  void write();
  void clear();
  void read();
private:
  QList<NRecentEntry*> _entries;
  int _maxItems;
};

} // namespace nn

#endif

/* vim: set ts=2 sw=2 tw=72 expandtab: */
