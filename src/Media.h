/*
 * NomNom
 * Copyright (C) 2010-2011  Toni Gundogdu <legatvs@gmail.com>
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

#ifndef nomnom_media_h
#define nomnom_media_h

#include <QVariantMap>

class Media
{
public:
  typedef enum
  {
    LengthBytes = 0x00,
    FileSuffix,
    PageTitle,
    StreamURL,
    PageURL,
    MediaID,
    Host
  } Detail;
public:
  Media& operator=(const Media&);
  Media(const Media&);
  Media();
public:
  bool fromJSON(const QString&, QString&);
  QVariant get(Detail) const;
private:
  QVariantMap _results;
};

class MediaException
{
public:
  MediaException(const QString&);
  const QString& what() const;
private:
  QString errmsg;
};

#endif

// vim: set ts=2 sw=2 tw=72 expandtab:
