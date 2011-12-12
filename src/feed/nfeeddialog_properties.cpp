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
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>

#include <NFeedDialog>
#include <NSettings>
#include <NFeed>
#include <NUtil>

extern bool have_umph_feature_all; // main.cpp

namespace nn
{

enum { StartIndexTooltip=0x00, MaxResultsTooltip };

static const char *tooltips[] =
{
  QT_TRANSLATE_NOOP("nn::NFeedProperties",
  "The start index parameter specifies the index of the first matching\n"
  "result that should be included in the result set. This parameter uses\n"
  "a one-based index, meaning the first result is 1, the second result\n"
  "is 2 and so forth.\n\n"
  "This parameter works in conjunction with the \"max. results\"\n"
  "parameter to determine which results to return. For example, to\n"
  "request the second set of 10 results, i.e. results 11-20, set the\n"
  "start index parameter to 11 and the max results parameter to 10.\n"
  "  -- YouTube Developer's Guide: Data API Protocol"
                   ),
  QT_TRANSLATE_NOOP("nn::NFeedProperties",
  "The max results parameter specifies the maximum number of\n"
  "results that should be included in the result set.\n\n"
  "This parameter works in conjunction with the start index parameter\n"
  "to determine which results to return. For example, to request the\n"
  "second set of 10 results, i.e. results 11-20, set the max results\n"
  "parameter to 10 and the start index parameter to 11.\n\n"
  "The default value of this parameter is 25, and the maximum value\n"
  "is 50. However, for displaying lists of videos, we recommend that\n"
  "you set the max results parameter to 10.\n"
  "  -- YouTube Developer's Guide: Data API Protocol"
                   )
};

typedef enum { Uploads=0x00, Favorites, Playlist } Type;
typedef enum { All=0x00, Select } Mode;

NFeedProperties::NFeedProperties(const QStringList& args,
                                 QWidget *parent/*=NULL*/)
  : NFeedWidget(parent),
    _rangeGroup(NULL),
    _typeCombo(NULL),
    _identEdit(NULL),
    _bgroup(NULL),
    _indexSpin(NULL),
    _identLabel(NULL),
    _maxSpin(NULL),
    _args(args)
{

// Widgets

  _typeCombo = new QComboBox;
  _typeCombo->addItem(tr("Uploads"), Uploads);
  _typeCombo->addItem(tr("Favorites"), Favorites);
  _typeCombo->addItem(tr("Playlist"), Playlist);

  connect(_typeCombo, SIGNAL(currentIndexChanged(int)),
          this, SLOT(typeChanged(int)));

  _identEdit = new QLineEdit;
  _identEdit->setToolTip(
    tr("YouTube username or playlist ID depending on your selection"));
  _identLabel = new QLabel;
  _identLabel->setBuddy(_identEdit);

  QHBoxLayout *identBox = new QHBoxLayout;
  identBox->addWidget(_typeCombo);
  identBox->addWidget(_identLabel);
  identBox->addWidget(_identEdit);

  QRadioButton *all = new QRadioButton(tr("&Get the entire feed"));
  QRadioButton *select =
    new QRadioButton(tr("G&et the specified range from the feed"));

  if (!have_umph_feature_all)
    {
      all->setToolTip(tr("Please install umph 0.2.0 or later to enable"));
      all->setEnabled(false);
      select->toggle();
    }
  else
    all->toggle();

  _bgroup = new QButtonGroup;
  _bgroup->addButton(all, All);
  _bgroup->addButton(select, Select);

  connect(_bgroup, SIGNAL(buttonClicked(int)), this, SLOT(modeChanged(int)));

// Select range

  QLabel *l = new QLabel(tr("St&art index:"));
  _indexSpin = new QSpinBox;
  _indexSpin->setToolTip(tooltips[StartIndexTooltip]);
  _indexSpin->setMinimum(1);
  l->setBuddy(_indexSpin);

  QGridLayout *rangeGrid = new QGridLayout;
  rangeGrid->addWidget(l, 0, 0);
  rangeGrid->addWidget(_indexSpin, 0, 1);

  l = new QLabel(tr("&Max. results:"));
  _maxSpin = new QSpinBox;
  _maxSpin->setToolTip(tooltips[MaxResultsTooltip]);
  _maxSpin->setMinimum(1);
  _maxSpin->setMaximum(50);
  l->setBuddy(_maxSpin);

  rangeGrid->addWidget(l, 1, 0);
  rangeGrid->addWidget(_maxSpin, 1, 1);

  _rangeGroup = new QGroupBox(tr("Select range"));
  _rangeGroup->setLayout(rangeGrid);

// Git it.

  QDialogButtonBox *btnBox = new QDialogButtonBox(
    QDialogButtonBox::Reset | QDialogButtonBox::Ok);

  connect(btnBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()),
          this, SLOT(reset()));

  connect(btnBox, SIGNAL(accepted()), this, SLOT(parse()));

// Layout

  QVBoxLayout *box = new QVBoxLayout;
  box->addLayout(identBox);
  box->addWidget(all);
  box->addWidget(select);
  box->addWidget(_rangeGroup);
  box->addWidget(btnBox);
  box->addStretch(0);
  setLayout(box);
}

void NFeedProperties::init()
{
  reset();
  typeChanged(_typeCombo->currentIndex());
  modeChanged(_bgroup->checkedId());
}

void NFeedProperties::enableRange(bool state/*=true*/)
{
  _rangeGroup->setEnabled(state);
}

void NFeedProperties::modeChanged(int n)
{
  enableRange(n == Select);
}

void NFeedProperties::typeChanged(int n)
{
  _identLabel->setText(n == 2 ? tr("I&D") : tr("for &user"));
}

void NFeedProperties::parse()
{
  if (_identEdit->text().isEmpty())
    {
      // Potential translation hack-up.
      QString s = _identLabel->text().remove("&").split(" ").takeLast();
      info(this, tr("Please enter %1").arg(s));
      _identEdit->setFocus();
      return;
    }

  QString type;
  switch (_typeCombo->itemData(_typeCombo->currentIndex()).toInt())
    {
    case Uploads:
    default:
      type = "u";
      break;
    case Favorites:
      type = "f";
      break;
    case Playlist:
      type = "p";
      break;
    }

  QStringList args = feed::to_args(_args,
                                   type,
                                   _identEdit->text(),
                                   _indexSpin->value(),
                                   _maxSpin->value(),
                                   _bgroup->checkedId() == All);
  emit parse(args);
}

void NFeedProperties::reset()
{
  _typeCombo->setCurrentIndex(0);
  _indexSpin->setValue(1);
  _maxSpin->setValue(25);
  _identEdit->clear();
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
