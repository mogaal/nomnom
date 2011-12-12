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

#ifndef nsettings_h
#define nsettings_h

class QString;

namespace nn
{

// Edit "key_strings" in nsettings.cpp also.
typedef enum
{
// Commands
  ParseUsing = 0x00,
  ParseUsingOther,
  DownloadUsing,
  DownloadUsingOther,
  PlayUsing,
  PlayUsingOther,
  FeedUsing,
  FeedUsingOther,
// Download
  FilenameFormat,
  FilenameRegExp,
  SaveMediaDirectory,
// Proxy
  ProxyType,
  ProxyHost,
  ProxyPort,
// Options: Appearance
  CustomApplicationIcon,
  CustomBusyIcon,
  CustomErrorIcon,
  Language,
// Options: Behaviour
  KeepApplicationWindowOnTop,
  PlayWhenDoneDownloading,
  AskWhereToSaveMediaFile,
  ClearURLRecordAtExit,
  ReplaceExistingMedia,
  GetBestFormat,
// Options: Systray
  ShowInTrayIcon,
  TerminateInstead,
  StartInTrayIcon,
// Counter, must be last.
  _MAX_KEYS
} SettingKey;

typedef enum { AutodetectProxy=0x00, NoProxy, ManualProxy } ProxyMode;

bool format_filename(const QString& regexp,
                     const QString& title,
                     const QString& media_id,
                     const QString& domain,
                     const QString& suffix,
                     QString& result);

} // namespace nn

#endif

/* vim: set ts=2 sw=2 tw=72 expandtab: */
