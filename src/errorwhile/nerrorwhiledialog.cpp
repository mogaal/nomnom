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

#include <QDialogButtonBox>
#include <QPlainTextEdit>
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QClipboard>
#include <QLabel>

#include <NErrorWhileDialog>

namespace nn
{

typedef enum
{
  ErrorWhileRunning = 0x00,
  ErrorMessageFollows,
  ErrorCode,
} LabelType;

static QString label(const LabelType t, const QString& arg1="")
{
  static const char *str[] =
  {
    QT_TRANSLATE_NOOP("nn::NErrorWhileDialog",
    "Error while running command:"),

    QT_TRANSLATE_NOOP("nn::NErrorWhileDialog",
    "Error message follows:"),

    QT_TRANSLATE_NOOP("nn::NErrorWhileDialog",
    "[code #%1]")
  };

  QString s = NErrorWhileDialog::tr(str[t]);

  if (!arg1.isEmpty())
    s = NErrorWhileDialog::tr(str[t]).arg(arg1);

  return s;
}

NErrorWhileDialog::NErrorWhileDialog(const QStringList& args,
                                     const QString& errmsg,
                                     const int errcode,
                                     QWidget *parent)
  : QDialog(parent)
{
  QVBoxLayout *box = new QVBoxLayout;

// Label

  QLabel *l = new QLabel(label(ErrorWhileRunning));
  box->addWidget(l);

// Command args

  QPlainTextEdit *t = new QPlainTextEdit(this);
  t->setPlainText(args.join(" "));
  t->setReadOnly(true);
  box->addWidget(t);

// Label

  l = new QLabel(label(ErrorMessageFollows));
  box->addWidget(l);

// Error message

  t = new QPlainTextEdit(this);
  const QString s = label(ErrorCode, QString("%1").arg(errcode));
  t->setPlainText(QString("%1\n%2").arg(errmsg).arg(s));
  t->setReadOnly(true);
  box->addWidget(t);

// Button box

  QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Ok);
  QPushButton *b = new QPushButton(tr("Co&py"));
  b->setToolTip(tr("Copy to clipboard"));

  bb->addButton(b, QDialogButtonBox::ActionRole);

  connect(bb, SIGNAL(accepted()), this, SLOT(accept()));
  connect(b, SIGNAL(clicked()), this, SLOT(copy()));
  box->addWidget(bb);

// Window

  setFixedSize(QSize(400,340));
  setWindowTitle(tr("Error"));
  setLayout(box);
  show();

// Save

  _errcode = errcode;
  _errmsg = errmsg;
  _args = args;
}

void NErrorWhileDialog::copy()
{
  QClipboard *c = QApplication::clipboard();
  c->setText(QString("%1\n%2\n\n%3\n%4\n%5\n")
             .arg(label(ErrorWhileRunning))
             .arg(_args.join(" "))
             .arg(label(ErrorMessageFollows))
             .arg(_errmsg)
             .arg(label(ErrorCode, QString("%1").arg(_errcode))));
}

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
