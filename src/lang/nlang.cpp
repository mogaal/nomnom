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

#include <cstdlib>

#include <QCoreApplication>
#include <QFileInfoList>
#include <QApplication>
#include <QInputDialog>
#include <QTranslator>
#include <QStringList>
#include <QVariant>
#include <QLocale>
#include <QRegExp>
#include <QDir>

#ifdef ENABLE_VERBOSE
#include <QDebug>
#endif

#include <NLang>

namespace nn
{

namespace lang
{

/*
 * Function: scan_for_qm
 *
 * Scan a directory for .qm files matching the pattern.
 *
 * Parameters:
 *  scanDir  - Directory to scan
 *  fileMask - e.g. "example_??_??.qm"
 *
 * Returns:
 *  QFileInfoList
 */
static QFileInfoList scan_for_qm(const QString& scanDir,
                                 const QString& fileMask)
{
  const QDir dir(QDir::toNativeSeparators(scanDir));
  return dir.entryInfoList(QStringList(fileMask), QDir::Files);
}

/*
 * Function: filemask
 *
 * Returns a filemask used to match .qm files.
 */
static QString filemask()
{
  return QString("%1_??_??.qm").arg(qApp->applicationName());
}

/*
 * Function: find_all_qm
 *
 * Find all .qm files from the default paths. Supports N_QM_SHOW_SCANDIR
 * and N_QM_SHOW environment variables.
 *
 * Parameters:
 *  found - Found .qm files
 *
 * Returns:
 *  true if succeeded
 */
bool find_all_qm(QFileInfoList& found)
{
  QStringList dirs;

  dirs << QDir::currentPath() + "/tr"
       << QDir::homePath() + "/.config/nomnom/tr"
       << QDir::homePath() + "/.local/share/nomnom/tr"
#ifdef DATADIR
       << QString(DATADIR) + "/nomnom/tr";
#endif
  ;

  const char *show_scandir = getenv("N_QM_SHOW_SCANDIR");

  if (show_scandir)
    fprintf(stderr, "Scan directories for .qm files:\n");

  const QString fileMask = filemask();

  foreach (const QString dir, dirs)
  {
    if (show_scandir)
      fprintf(stderr, "  %s\n", dir.toAscii().data());

    found << scan_for_qm(dir, fileMask);
  }

  const char *show_qm = getenv("N_QM_SHOW");

  if (show_qm)
    {
      fprintf(stderr, "Found .qm files:\n");
      foreach (const QFileInfo fi, found)
      {
        fprintf(stderr, "  %s\n", fi.filePath().toAscii().data());
      }
    }
  return found.size() > 0;
}

/*
 * Function: qm_files
 *
 * Return found .qm files.
 */
NResult qm_files()
{
  QFileInfoList qmFiles;
  NResult result;

  if (find_all_qm(qmFiles))
    {
      QRegExp rx("_(\\w+_\\w+)\\.qm");
      foreach (const QFileInfo info, qmFiles)
      {
        if (rx.indexIn(info.fileName()) == -1)
          continue;

        QLocale l(rx.cap(1));
        QString lang = QLocale::languageToString(l.language());

        result[rx.cap(1)] = NResultPair(lang, info.filePath());
      }
    }
  return result;
}

/*
 * Function: load
 *
 * Use the specified .qm file.
 *
 * Parameters:
 *  qmFilePath - Path to a .qm file
 *
 * Returns:
 *  true if succeeded
 */
static bool load(const QString& qmFilePath)
{
  QTranslator *t = new QTranslator;
  if (t->load(qmFilePath))
    {
#ifdef ENABLE_VERBOSE
      qDebug() << __PRETTY_FUNCTION__ << __LINE__ << qmFilePath;
#endif
      qApp->installTranslator(t);
      return true;
    }
  return false;
}

/*
 * Function: choose_dialog
 *
 * Display a dialog with the available translations.
 * Loads the user selected translation .qm file.
 *
 * QCoreApplication::applicationName must be set before this
 * function is called.
 *
 * Parameters:
 *  parent - Parent widget
 *  lang   - Language string
 *
 * Returns:
 *  true if succeeded
 */
bool choose_dialog(QWidget *parent, QString& lang)
{
  QStringList languages = QStringList() << "English";
  NResult results = qm_files();

  foreach (const NResultPair p, results)
  {
    languages << p.first;
  }

  bool ok = false;
  const QString s = QInputDialog::getItem(
                      parent,
                      qApp->translate("nn::NLang", "Select language"),
                      qApp->translate("nn::NLang", "Language:"),
                      languages,
                      0,
                      false,
                      &ok
                    );
  if (!ok)
    return false; // User cancelled.

  foreach (const NResultPair p, results)
  {
    if (p.first == s)
      {
        lang = s;
        return load(p.second);
      }
  }
  return true; // Default language (empty "lang" string).
}

/*
 * Function: choose
 *
 * Choose and load a translation automatically. This function tries to
 * match the system locale to the found .qm files. If a matching one is
 * found, the function loads the matched translation.
 *
 * If "initial" is set (to a locale), the function tries to find the
 * matching .qm file. This overrides the default behaviour of this
 * function which is to use the system locale for matching.
 *
 * QCoreApplication::applicationName must be set before this
 * function is called.
 *
 * Parameters:
 *  initial  - Locale to match, can be empty (see notes above)
 *
 * Returns:
 *  true if succeeded
 */
bool choose(const QString initial/*=""*/)
{
  const QLocale l = QLocale::system();

#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__
           << l.language()
           << l.name()
           << l.country()
           << QLocale::countryToString(l.country())
           << QLocale::languageToString(l.language());
#endif

  foreach(const NResultPair p, qm_files())
  {
    if (initial.isEmpty())
      {
#ifdef ENABLE_VERBOSE
        qDebug() << __PRETTY_FUNCTION__ << __LINE__
                 << p.first
                 << p.second
                 << QLocale::languageToString(l.language());
#endif

        if (p.first == QLocale::languageToString(l.language()))
          return load(p.second);
      }
    else
      {
        if (initial == p.first)
          return load(p.second);
      }
  }
  return false;
}

} // namespace lang

} // namespace nn

/* vim: set ts=2 sw=2 tw=72 expandtab: */
