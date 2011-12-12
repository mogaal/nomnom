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

#ifndef nrecententry_h
#define nrecententry_h

#include <QDateTime>

namespace nn
{

class NRecentEntry : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QDateTime added READ added WRITE setAdded)
  Q_PROPERTY(QString title READ title WRITE setTitle)
  Q_PROPERTY(QString url READ url WRITE setURL)
public:
  NRecentEntry(QObject *parent=NULL);
  NRecentEntry(const NRecentEntry&, QObject *parent=NULL);
  NRecentEntry& operator=(const NRecentEntry&);
  virtual ~NRecentEntry();
public:
  void setAdded(const QDateTime&);
  void setTitle(const QString&);
  void setURL(const QString&);
  QDateTime added() const;
  QString title() const;
  QString url() const;
private:
  void _copy(const NRecentEntry&);
private:
  QDateTime _added;
  QString _title;
  QString _url;
};

} // namespace nn

#endif

/* vim: set ts=2 sw=2 tw=72 expandtab: */
