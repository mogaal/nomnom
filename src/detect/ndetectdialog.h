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

#ifndef ndetectdialog_h
#define ndetectdialog_h

#include <QDialog>

class QTextBrowser;
class QComboBox;

#include <NDetect>

namespace nn {

class NDetectDialog : public QDialog
{
  Q_OBJECT
public:
  NDetectDialog(QWidget *parent=NULL);
public:
  void showModeComboBox(bool visible=true);
  const detect::NResultList& mediaParsers() const;
  const detect::NResultList& mediaPlayers() const;
  const detect::NResultList& feedParsers() const;
  const detect::NResultList& downloaders() const;
  const detect::NResult& mediaParser() const;
  const detect::NResult& mediaPlayer() const;
  const detect::NResult& feedParser() const;
  const detect::NResult& downloader() const;
private slots:
  void modeChanged(int);
private:
  detect::NResultList _mediaParsers;
  detect::NResultList _mediaPlayers;
  detect::NResultList _feedParsers;
  detect::NResultList _downloaders;
  detect::NResult _mediaParser;
  detect::NResult _mediaPlayer;
  detect::NResult _feedParser;
  detect::NResult _downloader;
  QTextBrowser *_textBrowser;
  QComboBox *_modeCombo;
};

} // namespace nn

#endif

/* vim: set ts=2 sw=2 tw=72 expandtab: */
