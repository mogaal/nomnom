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
#include <QTextStream>
#include <QStringList>
#include <QProcess>

#include <NDetect>

namespace nn
{

namespace detect
{

static const QStringList _regexps =
  QStringList()
  << "(\\d+)\\.(\\d+)\\.(\\d+)\\-(\\w+)\\-(\\w+)"
  << "(\\d+)\\.(\\d+)\\.(\\d+)\\-(\\w+)"
  << "(\\d+)\\.(\\d+)\\.(\\d+)"
  << "(\\d+)\\.(\\d+)";

/*
 * Function: _find
 *
 * Run a command with "--version" to check if it is executable.
 *
 * Parameters:
 *  cmd - Command to run
 *  result - Result
 *
 * Returns:
 *  true if command was found and --version output was parsed OK
 */
static bool _find(const QString& cmd, NResultPair& result)
{
  QProcess p;
  p.start(cmd, QStringList() << "--version", QIODevice::ReadOnly);

  if (!p.waitForStarted())
    return false;

  if (!p.waitForFinished())
    return false;

  const QString r =
    QString::fromLocal8Bit(p.readAll()).simplified();

  foreach (const QString& s, _regexps)
  {
    QRegExp rx(s);
    if (rx.indexIn(r) != -1)
      {
        result.first  = cmd;
        result.second = rx.capturedTexts()[0];
        return true;
      }
  }
  return false;
}

struct _lookup_s
{
  const char *cmd;
  const char *args;
};

static const struct _lookup_s lookup_media_parsers[] =
{
  {"quvi", "--category-http %u"},
  {NULL, NULL}
};

static const struct _lookup_s lookup_media_players[] =
{
  {"xdg-open", "%m"},
  {"mplayer", "%m"},
  {"totem", "%m"},
  {"vlc", "%m"},
  {NULL, NULL}
};

static const struct _lookup_s lookup_feed_parsers[] =
{
  {"umph", ""},
  {NULL, NULL}
};

static const struct _lookup_s lookup_downloaders[] =
{
  {"curl", "-L -C - -o %f %u --user-agent Mozilla/5.0"},
  {NULL, NULL}
};

/*
 * Function: find
 *
 * Find a command that belongs to the group.
 *
 * Parameters:
 *  type        - Type of command to detect
 *  results     - List of results
 *  log         - If not NULL, append messages to the stream
 *  findFirst   - Return first found result if true
 *
 * Returns:
 *  true if the command was found
 */
bool find(const DetectType type,
          NResultList& results,
          QTextStream* log /*=NULL*/,
          const bool findFirst/*=false*/)
{
  const struct _lookup_s *l = NULL;

  switch (type)
    {
    case MediaParser:
    default:
      l = static_cast<const struct _lookup_s*>(lookup_media_parsers);
      if (log)
        {
          *log << qApp->translate("nn::NDetect",
                                  "<b>Find media parser</b> ...");
        }
      break;
    case MediaPlayer:
      l = static_cast<const struct _lookup_s*>(lookup_media_players);
      if (log)
        {
          *log << qApp->translate("nn::NDetect",
                                  "<b>Find media player</b> ...");
        }
      break;
    case FeedParser:
      l = static_cast<const struct _lookup_s*>(lookup_feed_parsers);
      if (log)
        {
          *log << qApp->translate("nn::NDetect",
                                  "<b>Find YouTube feed parser</b> ...");
        }
      break;
    case Downloader:
      l = static_cast<const struct _lookup_s*>(lookup_downloaders);
      if (log)
        {
          *log << qApp->translate("nn::NDetect",
                                  "<b>Find downloader</b> ...");
        }
      break;
    }

  if (log)
    *log << "<br>";

  results.clear();

  int i = 0;

  while (l[i].cmd)
    {
      if (log)
        {
          *log << qApp->translate("nn::NDetect",
                                  "Check for %1 ...").arg(l[i].cmd);
        }

      NResultPair found;

      if (_find(l[i].cmd, found))
        {
          if (log)
            *log << QString("<b>%1</b><br>").arg(found.second);

          NResult r;

          r.first  = l[i].cmd;
          r.second = NResultPair(l[i].args, found.second);

          results << r;

          if (findFirst)
            return true;
        }
      else
        {
          if (log)
            {
              *log << qApp->translate("nn::NDetect","no") << "<br>";
            }
        }
      ++i;
    }
  return results.count() > 0;
}

/*
 * Function: find_first
 *
 * Find the first command that belongs to the group.
 *
 * Parameters:
 *  type    - Type of command to detect
 *  result  - Result
 *  log     - If not NULL, append messages to the stream
 *
 * Returns:
 *  true if a command was found
 */
bool find_first(const DetectType type,
                NResult& result,
                QTextStream* log/*=NULL*/)
{
  NResultList results;
  if (find(type, results, log, true /* Find first only */))
    {
      result = results[0];
      return true;
    }
  return false;
}

} // namespace detect

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
