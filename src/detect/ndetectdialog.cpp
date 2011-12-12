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

#include <QDialogButtonBox>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QTextStream>
#include <QComboBox>

#include <NDetectDialog>

namespace nn
{

/*
 * Constructor: NDetectDialog
 *
 * The detection dialog allows users to select "Normal" which will pick
 * the first command that belongs to each command group, or "Everything"
 * which will return all detected commands. Defaults to "Normal" mode.
 */
NDetectDialog::NDetectDialog(QWidget *parent/*=NULL*/)
  : QDialog(parent)
{
  _textBrowser = new QTextBrowser(this);
  _modeCombo   = new QComboBox(this);

  _modeCombo->addItem(tr("Detect first"));
  _modeCombo->addItem(tr("Detect all"));

  connect(_modeCombo, SIGNAL(currentIndexChanged(int)),
          this, SLOT(modeChanged(int)));

  QDialogButtonBox *b = new QDialogButtonBox(QDialogButtonBox::Ok);
  connect(b, SIGNAL(accepted()), this, SLOT(accept()));

  QVBoxLayout *box = new QVBoxLayout;
  box->addWidget(_textBrowser);
  box->addWidget(_modeCombo);
  box->addWidget(b);
  setLayout(box);

  setWindowTitle(tr("Detection"));
  setMinimumSize(QSize(400,200));
  modeChanged(0);
  show();
}

/*
 * Function: modeChanged
 *
 * Handles detection mode change in the combobox.
 *
 * Parameters:
 *  n - New index
 */
void NDetectDialog::modeChanged(int n)
{
  QString output;
  QTextStream stream(&output);

  _textBrowser->clear();

  static const char w[] =
    QT_TR_NOOP("\t<b>Warning</b>: required command not found<br>");

  if (n == 0) // Find the first of each category.
    {
      if (!detect::find_first(MediaParser, _mediaParser, &stream))
        stream << w;
      if (!detect::find_first(MediaPlayer, _mediaPlayer, &stream))
        stream << w;
      if (!detect::find_first(FeedParser, _feedParser, &stream))
        stream << w;
      if (!detect::find_first(Downloader, _downloader, &stream))
        stream << w;
      _textBrowser->insertHtml(output);
    }
  else // Find all of each category.
    {
      detect::find(MediaParser, _mediaParsers, &stream);
      detect::find(MediaPlayer, _mediaPlayers, &stream);
      detect::find(FeedParser, _feedParsers, &stream);
      detect::find(Downloader, _downloaders, &stream);
      _textBrowser->insertHtml(output);
    }
}

/*
 * Function: showModeComboBox
 *
 * Show (or hide) the "mode" combobox in the dialog.
 *
 * Parameters:
 *  visible - New state
 */
void NDetectDialog::showModeComboBox(const bool visible/*=true*/)
{
  _modeCombo->setVisible(visible);
}

/*
 * Function: mediaParsers
 *
 * Returns:
 *  List of detected media parsers
 */
const detect::NResultList& NDetectDialog::mediaParsers() const
{
  return _mediaParsers;
}

/*
 * Function: mediaPlayers
 *
 * Returns:
 *  List of detected media players
 */
const detect::NResultList& NDetectDialog::mediaPlayers() const
{
  return _mediaPlayers;
}

/*
 * Function: feedParsers
 *
 * Returns:
 *  List of detected feed parsers
 */
const detect::NResultList& NDetectDialog::feedParsers() const
{
  return _feedParsers;
}

/*
 * Function: downloaders
 *
 * Returns:
 *  List of detected downloaders
 */
const detect::NResultList& NDetectDialog::downloaders() const
{
  return _downloaders;
}

/*
 * Function: mediaParser
 *
 * Returns:
 *  First detected media parser
 */
const detect::NResult& NDetectDialog::mediaParser() const
{
  return _mediaParser;
}

/*
 * Function: mediaPlayer
 *
 * Returns:
 *  First detected media player
 */
const detect::NResult& NDetectDialog::mediaPlayer() const
{
  return _mediaPlayer;
}

/*
 * Function: feedParser
 *
 * Returns:
 *  First detected feed parser
 */
const detect::NResult& NDetectDialog::feedParser() const
{
  return _feedParser;
}

/*
 * Function: downloader
 *
 * Returns:
 *  First detected downloader
 */
const detect::NResult& NDetectDialog::downloader() const
{
  return _downloader;
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
