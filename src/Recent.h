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

#ifndef nomnom_recent_h
#define nomnom_recent_h

#include <QStringList>

class Recent
{
public:
  Recent ();
  virtual ~Recent();
public:
  void setMaxItems(const int);
public:
  void write () const;
  void read  ();
  void append(const QString&);
  void clear ();
public:
  QStringList toStringList () const;
public:
  Recent& operator<< (const QString&);
private:
  QStringList drops;
  int maxItems;
};

#endif

// vim: set ts=2 sw=2 tw=72 expandtab:
