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

#ifndef nutil_h
#define nutil_h

namespace nn
{

bool check_for_cmd_feature(SettingKey, SettingKey, const QString&, const int);
QStringList to_cmd_args(const QString&);
void info(QWidget*, const QString&);
int ask(QWidget*, const QString&);

} // namespace nn

#endif

/* vim: set ts=2 sw=2 tw=72 expandtab: */
