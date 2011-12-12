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

#include "config.h"

#include <QCoreApplication>
#include <QVariant>

#ifdef ENABLE_VERBOSE
#include <QDebug>
#endif

#include <qjson/parser.h>

#include "Media.h"

Media::Media()
{
}

Media::Media(const Media& v)
{
  _results = v._results;
}

Media& Media::operator=(const Media& m)
{
  if (this != &m)
    _results = m._results;
  return *this;
}

bool Media::fromJSON(const QString& s, QString& errmsg)
{
  QJson::Parser p;
  bool ok;

  QVariantMap r = p.parse(s.toLocal8Bit(), &ok).toMap();
  if (!ok)
    {
      errmsg = qApp->translate("Media",
                               "An error occurred while parsing JSON");
      return false;
    }

  _results["page_title"] = r["page_title"];
  _results["page_url"]   = r["page_url"];
  _results["host"]       = r["host"];
  _results["media_id"]   = r["id"];

  QVariantMap l = r["link"].toList().first().toMap();

  _results["length_bytes"] = l["length_bytes"];
  _results["file_suffix"]  = l["file_suffix"];
  _results["stream_url"]   = l["url"];

#ifdef ENABLE_VERBOSE
  qDebug () << __PRETTY_FUNCTION__ << __LINE__ << "media=" << _results;
#endif

  return true;
}

QVariant Media::get(Detail d) const
{
  switch (d)
    {
    case LengthBytes:
      return _results["length_bytes"];
    case FileSuffix:
      return _results["file_suffix"];
    case PageTitle:
      return _results["page_title"];
    case StreamURL:
      return _results["stream_url"];
    case PageURL:
      return _results["page_url"];
    case MediaID:
      return _results["media_id"];
    case Host:
      return _results["host"];
    }
  return QVariant();
}

// MediaException

MediaException::MediaException(const QString& errmsg)
  : errmsg(errmsg)
{
}

const QString& MediaException::what() const
{
  return errmsg;
}

// vim: set ts=2 sw=2 tw=72 expandtab:
