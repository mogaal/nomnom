
BUILT_SOURCES=\
 ui_MainWindow.h
 
noinst_HEADERS=\
 rc/MainWindow.ui \
 rc/img/nomnom.png \
 rc/img/README \
 rc/text/COPYING \
 rc/nomnom.qrc

nodist_nomnom_SOURCES=\
 moc_nfeedprogressdialog.cpp \
 moc_nerrorwhiledialog.cpp \
 moc_nsettingsdialog.cpp \
 moc_ndetectdialog.cpp \
 moc_ntripwireedit.cpp \
 moc_nrecententry.cpp \
 moc_naboutdialog.cpp \
 moc_nfeeddialog.cpp \
 moc_nlogdialog.cpp \
 moc_nsystray.cpp \
 moc_MainWindow.cpp \
 moc_DownloadDiag.cpp \
 moc_ProcProgDiag.cpp \
 qrc_nomnom.cpp

CLEANFILES=	$(nodist_nomnom_SOURCES) $(BUILT_SOURCES)

ui_MainWindow.h:
	$(UIC) -o $@ $(top_srcdir)/src/rc/MainWindow.ui

moc_MainWindow.cpp:
	$(MOC) -o $@ $(top_srcdir)/src/i/MainWindow.h

moc_DownloadDiag.cpp:
	$(MOC) -o $@ $(top_srcdir)/src/DownloadDiag.h

moc_ProcProgDiag.cpp:
	$(MOC) -o $@ $(top_srcdir)/src/ProcProgDiag.h

moc_nrecententry.cpp:
	$(MOC) -o $@ $(top_srcdir)/src/log/nrecententry.h

moc_nlogdialog.cpp:
	$(MOC) -o $@ $(top_srcdir)/src/log/nlogdialog.h

moc_nerrorwhiledialog.cpp:
	$(MOC) -o $@ $(top_srcdir)/src/errorwhile/nerrorwhiledialog.h

moc_nfeedprogressdialog.cpp:
	$(MOC) -o $@ $(top_srcdir)/src/feed/nfeedprogressdialog.h

moc_nfeeddialog.cpp:
	$(MOC) -o $@ $(top_srcdir)/src/feed/nfeeddialog.h

moc_nsettingsdialog.cpp:
	$(MOC) -o $@ $(top_srcdir)/src/settings/nsettingsdialog.h

moc_ndetectdialog.cpp:
	$(MOC) -o $@ $(top_srcdir)/src/detect/ndetectdialog.h

moc_ntripwireedit.cpp:
	$(MOC) -o $@ $(top_srcdir)/src/widgets/ntripwireedit.h

moc_naboutdialog.cpp:
	$(MOC) -o $@ $(top_srcdir)/src/about/naboutdialog.h

moc_nsystray.cpp:
	$(MOC) -o $@ $(top_srcdir)/src/systray/nsystray.h

qrc_nomnom.cpp:
	$(RCC) -o $@ $(top_srcdir)/src/rc/nomnom.qrc

src=\
 about/naboutdialog.cpp \
 detect/ndetectdialog.cpp \
 detect/ndetect.cpp \
 errorwhile/nerrorwhiledialog.cpp \
 feed/nfeeddialog_properties.cpp \
 feed/nfeedprogressdialog.cpp \
 feed/nfeeddialog_items.cpp \
 feed/nfeeddialog.cpp \
 feed/nfeed.cpp \
 lang/nlang.cpp \
 log/nlogdialog_recent.cpp \
 log/nrecentmutator.cpp \
 log/nrecententry.cpp \
 log/nlogdialog.cpp \
 settings/nsettingsdialog_options_appearance.cpp \
 settings/nsettingsdialog_options_behaviour.cpp \
 settings/nsettingsdialog_options_systray.cpp \
 settings/nsettingsdialog_commands.cpp \
 settings/nsettingsdialog_download.cpp \
 settings/nsettingsdialog_options.cpp \
 settings/nsettingsdialog_proxy.cpp \
 settings/nsettingsmutator.cpp \
 settings/nsettingsdialog.cpp \
 settings/nsettings.cpp \
 systray/nsystray.cpp \
 util/nutil.cpp \
 widgets/ntripwireedit.cpp \
 i/MainWindow.cpp \
 ProcProgDiag.cpp \
 DownloadDiag.cpp \
 main.cpp \
 Media.cpp

hdr=\
 NAboutDialog \
  about/naboutdialog.h \
 NDetectDialog \
 NDetect \
  detect/ndetectdialog.h \
  detect/ndetect.h \
 NErrorWhileDialog \
  errorwhile/nerrorwhiledialog.h \
 NFeedProgressDialog \
 NFeedDialog \
 NFeed \
  feed/nfeedprogressdialog.h \
  feed/nfeeddialog.h \
  feed/nfeed.h \
 NLang \
  lang/nlang.h \
 NRecentMutator \
 NRecentEntry \
 NLogDialog \
  log/nrecentmutator.h \
  log/nrecententry.h \
  log/nlogdialog.h \
 NTripwireEdit \
  widgets/ntripwireedit.h \
 NSettingsMutator \
 NSettingsDialog \
 NSettings \
  settings/nsettingsmutator.h \
  settings/nsettingsdialog.h \
  settings/nsettings.h \
 NSysTray \
  systray/nsystray.h \
 NUtil \
  util/nutil.h \
 i/MainWindow.h \
 ProcProgDiag.h \
 DownloadDiag.h \
 Media.h

bin_PROGRAMS=	nomnom
nomnom_SOURCES=	$(src) $(hdr)

AM_CPPFLAGS=	-I$(top_srcdir)/src
AM_CPPFLAGS+=	-I$(top_srcdir)/src/i
AM_CPPFLAGS+=	-DDATADIR='"$(datadir)"'
AM_CPPFLAGS+=	$(QtCore_CFLAGS)
AM_CPPFLAGS+=	$(QtGui_CFLAGS)
AM_CPPFLAGS+=	$(QJson_CFLAGS)
#if ENABLE_LIBPROXY
#AM_CPPFLAGS+=	$(libproxy_CFLAGS)
#endif

nomnom_LDADD=	$(QtCore_LIBS)
nomnom_LDADD+=	$(QtGui_LIBS)
nomnom_LDADD+=	$(QJson_LIBS)
#if ENABLE_LIBPROXY
#nomnom_LDADD+=	$(libproxy_LIBS)
#endif
