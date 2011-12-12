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
#include <QSettings>

#ifdef ENABLE_VERBOSE
#include <QDebug>
#endif

#include <NSettingsMutator>

namespace nn
{

void NSettingsMutator::setValue(SettingKey t, const QVariant& v)
{
  values[t] = v;
}

QVariant NSettingsMutator::value(SettingKey t) const
{
  return values[t];
}

/*
 * Function: eitherValue
 *
 * Return value of "a" or "b" if the former is set to "Other". This
 * function is otherwise identical to function "value".
 *
 * Parameters:
 *  a - Setting key
 *  b - Setting key, use this instead if "a" is set to "Other:"
 *
 * Returns:
 *  Value of either "a" or "b"
 */
QVariant NSettingsMutator::eitherValue(SettingKey a, SettingKey b) const
{
  static const QString other = QString("other")+NSETTINGS_CMDPATH_SEPARATOR;
  return values[values[a] == other ? b:a];
}

#define SETTINGS_GROUP "Settings"

void NSettingsMutator::write()
{
  QSettings s;
  s.beginGroup(SETTINGS_GROUP);

  QHashIterator<SettingKey,QVariant> iter(values);
  while (iter.hasNext())
    {
      iter.next();
      s.setValue(toString(iter.key()), iter.value());
    }
  s.endGroup();
}

void NSettingsMutator::read()
{
  QSettings s;
  s.beginGroup(SETTINGS_GROUP);

  foreach (QString key, s.childKeys())
  {
    SettingKey k = toKey(key);
    if (k != (int)-1)
      values[k] = s.value(key);
#ifdef ENABLE_VERBOSE
    else
      qWarning() << __PRETTY_FUNCTION__ << __LINE__ << "ignored" << key;
#endif
  }
  s.endGroup();
}

#undef SETTINGS_GROUP

extern const char *key_strings[]; // Settings.cpp

QString NSettingsMutator::toString(SettingKey k)
{
  QString s;
  if (k < _MAX_KEYS)
    s = QString(key_strings[k]);
  return s;
}

SettingKey NSettingsMutator::toKey(const QString& s)
{
  for (int i=0; i<_MAX_KEYS && key_strings[i]; ++i)
    {
      if (key_strings[i] == s)
        return static_cast<SettingKey>(i);
    }
  return static_cast<SettingKey>(-1);
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
