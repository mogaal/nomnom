/*
* Copyright (C) 2010 Toni Gundogdu.
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

#ifndef nomnom_video_h
#define nomnom_video_h

#include <QPointer>
#include <QRegExp>
#include <QHash>

class QLabel;

class Video : public QObject
{
  Q_OBJECT
public:
  enum Detail
  { Link=0, Title, PageURL, ID, Format, Length, Suffix, ContentType, Host };
public:
  Video ();
  Video (const Video&);
  Video& operator=(const Video&);
public:
  bool     fromJSON   (const QString&, QString&);
  QVariant get        (Detail) const;
  void     set        (Detail, const QString&);
private:
  // Details.
  QString _id;
  QString _title;
  QString _pageURL;
  QString _link;
  QString _format;
  qint64  _length;
  QString _suffix;
  QString _ctype;
  QString _host;
};

class VideoException
{
public:
  VideoException (const QString&);
  const QString& what() const;
private:
  QString errmsg;
};

#endif

// vim: set ts=2 sw=2 tw=72 expandtab:
