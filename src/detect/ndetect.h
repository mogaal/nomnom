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

#ifndef ndetect_h
#define ndetect_h

#include <QPair>
#include <QList>

class QTextStream;

namespace nn
{

/*
 * Enum: Detection types
 *
 * MediaParser  - Media stream URL parser
 * MediaPlayer  - Media player
 * FeedParser   - YouTube feed parser
 * Downloader   - Downloader
 */
typedef enum
{
  MediaParser = 0x00,
  MediaPlayer,
  FeedParser,
  Downloader
} DetectType;

namespace detect
{

/*
 * Typedef: Detection results
 *
 * NResultPair - Wraps QPair<QString,QString>
 * NResultList - Wraps QList<NResult>
 * NResult     - Wraps QPair<QString,<NResultPair>>
 */
typedef QPair<QString,QString> NResultPair;
typedef QPair<QString,NResultPair> NResult;
typedef QList<NResult> NResultList;

bool find(const DetectType type,
          NResultList& results,
          QTextStream* output=NULL,
          const bool findFirst=false);

bool find_first(const DetectType type,
                NResult& result,
                QTextStream* output=NULL);

} // namespace detect

/*
 * Typedef: Detection result aliases
 *
 * NDetectResultPair - Alias for detect::NResultPair
 * NDetectResult     - Alias for detect::NResult
 * NDetectResultList - Alias for detect::NResultList
 *
 * See Also:
 *  <Detection results>
 */
typedef detect::NResultPair          NDetectResultPair;
typedef detect::NResultList          NDetectResultList;
typedef detect::NResult              NDetectResult;

} // namespace nn

#endif

/* vim: set ts=2 sw=2 tw=72 expandtab: */
