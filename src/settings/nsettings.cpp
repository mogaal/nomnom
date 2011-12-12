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

#include <QStringList>

#include <NSettings>

namespace nn
{

// Edit "SettingKey" enum in nsettings.h also.
const char *key_strings[] =
{
// Commands
  "ParseUsing",
  "ParseUsingOther",
  "DownloadUsing",
  "DownloadUsingOther",
  "PlayUsing",
  "PlayUsingOther",
  "FeedUsing",
  "FeedUsingOther",
// Download
  "FilenameFormat",
  "FilenameRegExp",
  "SaveMediaDirectory",
// Proxy
  "ProxyType",
  "ProxyHost",
  "ProxyPort",
// Options: Appearance
  "CustomApplicationIcon",
  "CustomBusyIcon",
  "CustomErrorIcon",
  "Language",
// Options: Behaviour
  "KeepApplicationWindowOnTop",
  "PlayWhenDoneDownloading",
  "AskWhereToSaveMediaFile",
  "ClearURLRecordAtExit",
  "ReplaceExistingMedia",
  "GetBestFormat",
// Options: Systray
  "ShowInTrayIcon",
  "TerminateInstead",
  "StartInTrayIcon",
// Must end with 0.
  0
};

static bool check_regexp(const QString& regexp, QStringList& cap)
{
  QRegExp rx("^\\/(.*)\\/(.*)$");

  if (rx.indexIn(regexp) == -1)
    return false;

  cap = rx.capturedTexts();

  return true;
}

static bool apply_regexp(const QString& regexp,
                         const QString& title,
                         QString& dst)
{
  QStringList cap;
  if (!check_regexp(regexp, cap))
    return false;

  const QString p = cap.at(1);
  const bool g = cap.at(2).contains("g");
#ifdef _1
  const bool i = cap.at(2).contains("i");
#endif

  QRegExp rx(p);
#ifdef _1
  rx.setCaseSensitivity(i ? Qt::CaseInsensitive : Qt::CaseSensitive);
#endif

  int pos = 0;
  while ((pos = rx.indexIn(title, pos)) != -1)
    {
      pos += rx.matchedLength();
      dst += rx.cap(1);
      if (!g) break;
    }

  dst = dst.simplified();

  return true;
}

bool format_filename(const QString& regexp,
                     const QString& title,
                     const QString& media_id,
                     const QString& domain,
                     const QString& suffix,
                     QString& result)
{
  QString regexp_title;
  if (!apply_regexp(regexp, title, regexp_title))
    return false;

  result.replace("%t", regexp_title);
  result.replace("%i", media_id);
  result.replace("%d", domain);
  result.replace("%s", suffix);

  result = result.simplified();

  return true;
}


} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
