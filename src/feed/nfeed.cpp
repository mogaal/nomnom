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
#include <QFileInfo>
#include <QProcess>

#include <NFeed>

namespace nn
{

namespace feed
{

QStringList to_args(const QStringList& _args,
                    const QString& type,
                    const QString& ident,
                    const int startIndex,
                    const int maxResults,
                    const bool all)
{
  QStringList args = _args;
  args << "--json"
       << "-t"
       << type
       << ident;
  if (all)
    args << "-a";
  else
    {
      args << "-s" << QString::number(startIndex);
      args << "-m" << QString::number(maxResults);
    }
  return args;
}

} // namespace feed

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
