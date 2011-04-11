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

#include <QString>
#include <QObject>
#include <QApplication>
#include <QDebug>

#include "tips.h"

namespace NomNom
{

static const char* tips[] =
{
  // 0: reserved.
  QT_TRANSLATE_NOOP("Tips",
  "<p>Looks like you are running NomNom for the first time.</p>"
  "<p>Please make sure you have installed quvi(1) and "
  "curl(1) commands to your system. You will also need "
  "a streaming capable media player, e.g. vlc(1).</p>"
  "<p>Open the Preferences (Ctrl+E) and check that the "
  "paths to these commands have been specified correctly. "
  "NomNom will need these commands to work properly.</p>"
                   ),
  QT_TRANSLATE_NOOP("Tips",
  "Right-click opens the context-menu."
                   ),
  QT_TRANSLATE_NOOP("Tips",
  "You can drag and drop video page URLs onto the window to start "
  "the download (or streaming)."
                   ),
  QT_TRANSLATE_NOOP("Tips",
  "You can make NomNom \"hover\" over the other windows. "
  "Check the \"Stay on top\" in the Preferences."
                   ),
  QT_TRANSLATE_NOOP("Tips",
  "You can set NomNom to either \"Download\" or \"Stream\" the media. "
  "When you choose \"Stream\", open the preferences (Ctrl+E) and make "
  "sure that you have specified the path to a player command (that is "
  "capable of streaming media)."
                   ),
  QT_TRANSLATE_NOOP("Tips",
  "NomNom resumes file transfers by default (determined by the curl "
  "command in the Preferences). Check \"Overwrite\" in the context "
  "menu if you want to overwrite the existing files."
                   ),
  QT_TRANSLATE_NOOP("Tips",
  "To clear the list of recently visited URLs, open the Preferences "
  "(Ctrl+E), select \"Other\" and click \"Clear recent\"."
                   ),
  QT_TRANSLATE_NOOP("Tips",
  "To access Youtube feeds -- including Youtube users playlists, "
  "favorites and uploads -- choose \"Feed\" (Ctrl+F) from the "
  "context menu. "
                   ),
  QT_TRANSLATE_NOOP("Tips",
  "Check \"Do not prompt for filename\" in the Preferences (Ctrl+E) "
  "if you do not want to choose the video output file everytime you "
  "start a download."
                   ),
  QT_TRANSLATE_NOOP("Tips",
  "NomNom can minimize automatically when download starts; check "
  "\"Minimize when download starts\" in the Preferences (Ctrl+E)."
                   ),
  QT_TRANSLATE_NOOP("Tips",
  "You can use a custom program icon instead of the default one. "
  "To do that, open the Preferences (Ctrl+E) and select Appearance."
                   ),
  NULL // End.
};

static size_t
max_tips ()
{
  size_t i;
  for (i=0; tips[i] != NULL; ++i);
  return i;
}

QString
next_tip (size_t &n)
{
  if (n == 0)
    n = 1;

  if (++n >= max_tips ())
    n = 1;

  return QCoreApplication::translate ("Tips", tips[n]);
}

QString
get_tip (size_t& n, const bool first/*=false*/)
{
  n = 0;

  if (first) return tips[0];

  n = qrand () % max_tips ();

  if (n == 0) ++n;

  return QCoreApplication::translate ("Tips", tips[n]);
}

} // End namespace.

// vim: set ts=2 sw=2 tw=72 expandtab:
