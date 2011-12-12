/*
 * NomNom
 * Copyright (C) 2010-2011  Toni Gundogdu <legatvs@gmail.com>
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

#include <QInputDialog>
#include <QMainWindow>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QRegExp>

#ifdef ENABLE_VERBOSE
#include <QDebug>
#endif

#include <NErrorWhileDialog>
#include <NSettingsMutator>
#include <NSettingsDialog>
#include <NRecentMutator>
#include <NRecentEntry>
#include <NAboutDialog>
#include <NFeedDialog>
#include <NLogDialog>
#include <NSettings>
#include <NSysTray>
#include <NLang>
#include <NUtil>

#include "DownloadDiag.h"
#include "ProcProgDiag.h"
#include "MainWindow.h"

#define QSETTINGS_GROUP             "MainWindow"

// main.cpp

extern bool have_quvi_feature_query_formats;
extern nn::NSettingsMutator settings;
extern bool have_umph_feature_all;
extern nn::NRecentMutator recent;
extern nn::NSysTray *systray;

enum { StreamMedia=0, DownloadMedia };

MainWindow::MainWindow()
{
  setupUi(this);
  restore();

// Create Download dialog.

  _download = new DownloadDialog(this);

// Create Process Progress dialog for quvi.

  _proc = new ProcessProgressDialog(this);

  connect(_proc, SIGNAL(finished(QString)),
          this, SLOT(onProcFinished(QString)));

// Recent URLs.

  recent.setMaxItems(25);
  recent.read();

// Custom program icon.

#ifdef _1
  if (shPrefs.get(SharedPreferences::CustomProgramIcon).toBool())
    changeProgramIcon();
#endif
}

void MainWindow::createContextMenu()
{
#define creat_a(t,f,c) \
    do { \
        QAction *a = new QAction(t, textBrowser); \
        if (c) { \
            a->setCheckable(c); \
            /*connect(a, SIGNAL(toggled(bool)), SLOT(f(bool)));*/ \
        } \
        else \
            connect(a, SIGNAL(triggered()), SLOT(f())); \
        textBrowser->addAction(a); \
        _actions[t] = a; \
    } while (0)

#define add_s \
    do { \
        QAction *a = new QAction(textBrowser); \
        a->setSeparator(true); \
        textBrowser->addAction(a); \
    } while (0)

  creat_a(tr("Address..."),  onAddress,   false);
  creat_a(tr("Feed..."),     onFeed,      false);
  creat_a(tr("Recent..."),   onRecent,    false);
  add_s;
  creat_a(tr("Settings..."), onSettings,  false);
  add_s;
  creat_a(tr("About..."),    onAbout,     false);
  creat_a(tr("Quit"),        onTerminate, false);

#undef add_s
#undef creat_a

// Add key shortcuts.

#define _wrap(s,k) \
    do { _actions[s]->setShortcut(QKeySequence(k)); } while (0)

  _wrap(tr("Address..."),    "Ctrl+A");
  _wrap(tr("Feed..."),       "Ctrl+F");
  _wrap(tr("Recent..."),     "Ctrl+R");
  // --
  _wrap(tr("Settings..."),   "Ctrl+E");
  // --
  _wrap(tr("Quit"),          "Ctrl+Q");
#undef _wrap

// Add the context menu.

  textBrowser->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createTrayIcon()
{
  systray = new nn::NSysTray(this, QString("<b>NomNom</b> %1").arg(VN));

  connect(systray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this, SLOT(activated(QSystemTrayIcon::ActivationReason)));

  systray->addTrayMenuAction(SIGNAL(toggled(bool)),
                             this,
                             SLOT(setVisible(bool)),
                             tr("Show"),
                             true);

#ifdef _1
  // FIXME: If mainwindow is hidden, closing the NSettingsDialog causes
  // the program to exit.
  systray->addTrayMenuAction(SIGNAL(triggered()),
                             this,
                             SLOT(onSettings()),
                             tr("Settings..."));
#endif

  systray->addTrayMenuSeparator();

  systray->addTrayMenuAction(SIGNAL(triggered()),
                             this,
                             SLOT(onTerminate()),
                             tr("Quit"));

  systray->setIcon(QIcon(":img/nomnom.png"));
  systray->setTrayMenu();

  const bool startInTray = settings.value(nn::StartInTrayIcon).toBool();
  const bool showInTray = settings.value(nn::ShowInTrayIcon).toBool();

  systray->setVisible(showInTray);

  if (showInTray)
    startInTray ? hide() : show();
  else
    show();
}

#ifdef ENABLE_VERBOSE
static void print_url(const nn::NRecentEntry& r)
{
  qDebug() << __PRETTY_FUNCTION__ << __LINE__ << r.url();
}
#endif

void MainWindow::handleURL(const QString& u)
{
  const QUrl url = QUrl::fromUserInput(u);

  if (!url.isValid())
  {
    // QUrl::errorString() is less than ideal.
    nn::info(this, tr("Invalid URL: %1").arg(u));
    return;
  }

  QString s = settings.eitherValue(nn::ParseUsing,
                                   nn::ParseUsingOther)
              .toString()
              .simplified();

  QStringList q_args = nn::to_cmd_args(s);
  const QString q = q_args.first();

#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "quvi_path=" << q;
#endif

  if (q.isEmpty())
    {
      nn::info(this, tr("Please configure the path to the quvi. "
                        "See under the \"commands\" in the settings."));
      onSettings();
      return;
    }

  s = settings.eitherValue(nn::PlayUsing,
                           nn::PlayUsingOther)
      .toString()
      .simplified();

  QStringList p_args = nn::to_cmd_args(s);
  const QString p = p_args.first();

  if (p.isEmpty())
    {
      nn::info(this, tr("Please configure the path to a media player. "
                        "See under the \"commands\" in the settings."));
      onSettings();
      return;
    }

// Recent.

  nn::NRecentEntry e;
  e.setURL(url.toEncoded());

  recent << e;

#ifdef ENABLE_VERBOSE
  recent.for_each(print_url);
#endif

// 0x1=invalid input, 0x3=no input

  have_quvi_feature_query_formats =
    nn::check_for_cmd_feature(nn::ParseUsing,
                              nn::ParseUsingOther,
                              "-F",
                              0x3);

// Query formats to an URL.

#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__
           << "have_quvi_feature_query_formats="
           << have_quvi_feature_query_formats;
#endif

  const bool get_best = settings.value(nn::GetBestFormat).toBool();

  QStringList formats;
  if (have_quvi_feature_query_formats && !get_best)
    {
      bool failed = false;

      if (!queryFormats(formats, q_args, url, failed))
        formats.clear();

      if (failed)
        {
          nn::NErrorWhileDialog *d =
            new nn::NErrorWhileDialog(q_args,
                                      _proc->errmsg(),
                                      _proc->errcode(),
                                      this);
          d->exec();
          return;
        }

      if (_proc->canceled())
        return;
    }

  _json.clear();

// Choose format.

  QString fmt;
  if (!selectFormat(formats, fmt, get_best))
    return;

// Query media stream data.

  q_args.replaceInStrings("%u", url.toEncoded());
  q_args << "-f" << fmt;

#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "q_args=" << q_args;
#endif

  _proc->setLabelText(tr("Checking..."));
  _proc->setMaximum(0);
  _proc->start(q_args);

  if (_proc->canceled())
    return;

// Check for quvi errors.

  QString errmsg;
  if (parseOK(errmsg))
    {
      // Update recent entry. Media URL is set already. Update title
      // only.

      e.setTitle(_media.get(Media::PageTitle).toString().simplified());
      recent.update(e);

      // Download media or pass media stream URL.

      if (modeCBox->currentIndex() == StreamMedia)
        streamMedia();
      else
        downloadMedia();
    }
  else
    {
      nn::NErrorWhileDialog *d =
        new nn::NErrorWhileDialog(q_args,
                                  errmsg,
                                  _proc->errcode(),
                                  this);
      d->exec();
    }
}

bool MainWindow::queryFormats(QStringList& formats,
                              const QStringList& q_args,
                              const QUrl& url,
                              bool& failed)
{
  QStringList args = q_args;
  args.replaceInStrings("%u", url.toEncoded());
  args << "-F";

#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "args=" << args;
#endif

  _json.clear();

  _proc->setLabelText(tr("Checking..."));
  _proc->setMaximum(0);
  _proc->start(args);

  failed = _proc->failed();
  if (failed)
    return false;

#ifdef ENABLE_VERBOSE
  qDebug() <<  __PRETTY_FUNCTION__ << __LINE__ << "failed=" << failed;
#endif

  QStringList lns = _json.split("\n");
  lns.removeDuplicates();

  const QRegExp rx("^(.*)\\s+:\\s+");
  foreach (const QString s, lns)
  {
    if (rx.indexIn(s) != -1)
      {
        formats = QStringList()
                  << "default"
                  << "best"
                  << rx.cap(1).simplified().split("|");

#ifdef ENABLE_VERBOSE
        qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "formats=" << formats;
#endif

        return true;
      }
  }
  return false;
}

bool MainWindow::selectFormat(QStringList& formats,
                              QString& fmt,
                              const bool get_best)
{
  if (get_best)
    {
      fmt = "best";
      return true;
    }

  if (formats.size() < 2) // Skip prompt unless >1 formats available
    {
      fmt = "default";
      return true;
    }

  bool ok = false;
  fmt = QInputDialog::getItem(this,
                              tr("Choose format"),
                              tr("Format:"),
                              formats << tr("Enter your own"),
                              0,
                              false,
                              &ok);
  if (!ok)
    return false; // Cancel

  if (fmt == tr("Enter your own"))
    {
      fmt = QInputDialog::getText(this,
                                  tr("Enter format"),
                                  tr("Format:"),
                                  QLineEdit::Normal,
                                  "default",
                                  &ok);
    }
  return ok && !fmt.isEmpty();
}

void MainWindow::streamMedia()
{
  const QString p = settings.eitherValue(nn::PlayUsing,
                                         nn::PlayUsingOther)
                    .toString()
                    .simplified();

  QStringList args = nn::to_cmd_args(p);
  args.replaceInStrings("%m", _media.get(Media::StreamURL).toString());

  const QString cmd = args.takeFirst();

  if (!QProcess::startDetached(cmd, args))
    {
      nn::NErrorWhileDialog *d =
        new nn::NErrorWhileDialog(QStringList() << cmd << args,
                                  tr("Unknown error while attempting to "
                                     "start a detached process"),
                                  -1,
                                  this);
      d->exec();
    }
}

void MainWindow::downloadMedia()
{
  QString fname = settings.value(nn::FilenameFormat).toString();

  const QString suffix =
    _media.get(Media::FileSuffix).toString().simplified();

  bool ok = nn::format_filename(
              settings.value(nn::FilenameRegExp).toString(),
              _media.get(Media::PageTitle).toString().simplified(),
              _media.get(Media::MediaID).toString().simplified(),
              _media.get(Media::Host).toString().simplified(),
              suffix,
              fname
            );

  if (!ok)
    return;

  QString fpath = settings.value(nn::SaveMediaDirectory).toString();

  if (fpath.isEmpty())
    fpath = QDir::homePath();

  fpath = QDir::toNativeSeparators(fpath +"/"+ fname);

  const bool ask_where_to_save =
    settings.value(nn::AskWhereToSaveMediaFile).toBool();

  if (ask_where_to_save)
    {
      const QFileDialog::Options opts =
        QFileDialog::DontConfirmOverwrite;

      fpath = QFileDialog::getSaveFileName(this,
                                           tr("Save media as"),
                                           fpath,
                                           suffix, // Filter.
                                           0,      // Selected filter.
                                           opts);
      if (fpath.isEmpty())
        return;
    }

  if (settings.value(nn::ReplaceExistingMedia).toBool())
    QDir().remove(fpath);

  const qint64 expected_bytes =
    _media.get(Media::LengthBytes).toLongLong();

  if (QFileInfo(fpath).size() < expected_bytes)
    {
      const QString p = settings.eitherValue(nn::DownloadUsing,
                                             nn::DownloadUsingOther)
                        .toString()
                        .simplified();

      QStringList args = nn::to_cmd_args(p);
      args.replaceInStrings("%u", _media.get(Media::StreamURL).toString());
      args.replaceInStrings("%f", fpath);

      _download->start(args);

      if (_download->canceled())
        return;

      if (_download->failed())
        {
          nn::NErrorWhileDialog *d =
            new nn::NErrorWhileDialog(args,
                                      _download->errmsg(),
                                      _download->errcode(),
                                      this);
          d->exec();
          return;
        }
    }

  const bool completeFile =
    QFileInfo(fpath).size() >= expected_bytes;

  const bool playWhenDone =
    settings.value(nn::PlayWhenDoneDownloading).toBool();

#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "file="
           << QFileInfo(fpath).size()
           << expected_bytes
           << completeFile
           << playWhenDone;
#endif

  if (completeFile && playWhenDone)
    {
      const QString p = settings.eitherValue(nn::PlayUsing,
                                             nn::PlayUsingOther)
                        .toString()
                        .simplified();

      QStringList args = nn::to_cmd_args(p);
      args.replaceInStrings("%m", fpath);

#ifdef ENABLE_VERBOSE
      qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "args=" << args;
#endif

      const QString cmd = args.takeFirst();

      if (!QProcess::startDetached(cmd, args))
        {
          nn::NErrorWhileDialog *d =
            new nn::NErrorWhileDialog(QStringList() << cmd << args,
                                      tr("Unknown error while attempting to "
                                         "start a detached process"),
                                      -1,
                                      this);
          d->exec();
        }
    }
}

void MainWindow::changeProgramIcon()
{
#ifdef _1
  const bool customProgramIcon =
    shPrefs.get(SharedPreferences::CustomProgramIcon).toBool();

  QString html = textBrowser->toHtml();

  const QString iconPath =
    customProgramIcon
    ? shPrefs.get(SharedPreferences::ProgramIconPath).toString()
    : ":img/nomnom.png";

  html.replace(QRegExp("img src=\".*\""), "img src=\"" +iconPath+ "\"");

  textBrowser->setHtml(html);

  QIcon icon = QIcon(iconPath);
  trayIcon->setIcon(icon);
  setWindowIcon(icon);
#endif // _1
}

bool MainWindow::parseOK(QString& errmsg)
{
  if (_proc->failed())
    {
      errmsg = _proc->errmsg();
      return false;
    }
  const int n = _json.indexOf("{");
  if (n == -1)
    {
      errmsg = tr("quvi returned unexpected data");
      return false;
    }
  return _media.fromJSON(_json.mid(n), errmsg);
}

static void check_window_flags(QWidget *w)
{
  Qt::WindowFlags flags = w->windowFlags();
  if (settings.value(nn::KeepApplicationWindowOnTop).toBool())
    {
      if (!(flags & Qt::WindowStaysOnTopHint))
        flags |= Qt::WindowStaysOnTopHint;
    }
  else
    {
      if (flags & Qt::WindowStaysOnTopHint)
        flags &= ~Qt::WindowStaysOnTopHint;
    }

  if (flags != w->windowFlags())
    {
      w->setWindowFlags(flags);
      if (w->isHidden())
        w->show();
    }
}

static bool check_language(QWidget *w, const QString& lang)
{
#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "lang="
           << lang
           << settings.value(nn::Language).toString();
#endif

  if (lang != settings.value(nn::Language).toString())
    {
      if (nn::ask(w, qApp->translate("MainWindow",
                                     "Language will be changed after "
                                     "you restart the application. "
                                     "Restart now?"))
          == QMessageBox::No)
        {
          return false;
        }
      return true;
    }
  return false;
}

void MainWindow::onSettings()
{
  QString lang = settings.value(nn::Language).toString();

  if (lang.isEmpty()) // NSettingsDialog uses "English" instead of ""
    lang = "English";

  nn::NSettingsDialog *d = new nn::NSettingsDialog(this);
  if (d->exec() == QDialog::Accepted)
    {
      if (check_language(this, lang))
        {
          onTerminate();
          QProcess::startDetached(QCoreApplication::applicationFilePath());
          return;
        }
      check_window_flags(this);
    }
}

#define WWW "http://nomnom.sourceforge.net/"

void MainWindow::onAbout()
{
  nn::NAboutDialog *d = new nn::NAboutDialog(VN, WWW, this);
  d->exec();
}

#undef WWW

void MainWindow::onRecent()
{
  nn::NLogDialog *d = new nn::NLogDialog(this);
  if (d->exec() == QDialog::Accepted)
    handleURL(d->selected());
}

void MainWindow::onAddress()
{
  const QString url =
    QInputDialog::getText(this,
                          tr("Address"),
                          tr("Media page URL:"))
    .simplified();

  if (url.isEmpty())
    return;

  handleURL(url);
}

void MainWindow::onFeed()
{
  const QString p = settings.eitherValue(nn::FeedUsing,
                                         nn::FeedUsingOther)
                    .toString()
                    .simplified();

  QStringList args = nn::to_cmd_args(p);
#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "args=" << args;
#endif
  const QString r = args.first();

  if (r.isEmpty())
    {
      nn::info(this, tr("Please configure the path to a feed reader. "
                        "See under the \"commands\" in the settings."));
      onSettings();
      return;
    }

  have_umph_feature_all =
    nn::check_for_cmd_feature(nn::FeedUsing,
                              nn::FeedUsingOther,
                              "-a",
                              0x0);

  nn::NFeedDialog *d = new nn::NFeedDialog(this, args);
  if (d->exec() == QDialog::Accepted)
    handleURL(d->selected());
}

void MainWindow::onProcFinished(QString output)
{
  _json = output;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
  QUrl url(e->mimeData()->text());
  if (url.isValid() && url.scheme().toLower() == "http")
    e->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *e)
{
  handleURL(e->mimeData()->text());
  e->acceptProposedAction();
}

static void update_show_state(QWidget *w)
{
  const bool v = !w->isVisible();

  QAction *a = systray->findTrayMenuAction(
                 qApp->translate("MainWindow", "Show"));

  if (a)
    a->setChecked(v);

  w->setVisible(v);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__;
#endif

  if (settings.value(nn::TerminateInstead).toBool()
      || !systray->isVisible())
    {
      if (settings.value(nn::ClearURLRecordAtExit).toBool())
        recent.clear();
      save();
      e->accept();
    }
  else
    {
      update_show_state(this);
      e->ignore();
    }
}

void MainWindow::activated(QSystemTrayIcon::ActivationReason r)
{
  if (r == QSystemTrayIcon::Trigger)
    update_show_state(this);
}

void MainWindow::onTerminate()
{
  // When systray icon is visible: the default behaviour is to ignore
  // calls to 'close' mainwindow unless "terminate instead" is true.
#ifdef ENABLE_VERBOSE
  qDebug() << __PRETTY_FUNCTION__ << __LINE__;
#endif
  // Although the line below uses "settings" value "TerminateInstead",
  // it is not written to config.
  settings.setValue(nn::TerminateInstead, true);
  close();
}

static void tweak_window_flags(QWidget *w)
{
  Qt::WindowFlags flags = w->windowFlags();

// Remove buttons.

  flags &= ~Qt::WindowMinimizeButtonHint;
  flags &= ~Qt::WindowMaximizeButtonHint;

// Stay on top?

  if (settings.value(nn::KeepApplicationWindowOnTop).toBool())
    flags |= Qt::WindowStaysOnTopHint;

  w->setWindowFlags(flags);
}

void MainWindow::restore()
{
  QSettings s;
  s.beginGroup(QSETTINGS_GROUP);
  modeCBox->setCurrentIndex(s.value("modeCBox").toInt());
  restoreGeometry(s.value("geometry").toByteArray());
  restoreState(s.value("state").toByteArray());
  s.endGroup();

  tweak_window_flags(this);
  createContextMenu();
  createTrayIcon();
}

void MainWindow::save()
{
  QSettings s;
  s.beginGroup(QSETTINGS_GROUP);
  s.setValue("modeCBox", modeCBox->currentIndex());
  s.setValue("geometry", saveGeometry());
  s.setValue("state", saveState());
  s.endGroup();
  recent.write();
}

// vim: set ts=2 sw=2 tw=72 expandtab:
