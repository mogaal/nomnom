ACLOCAL_AMFLAGS=-I m4

EXTRA_DIST=\
 nomnom.desktop.in \
 README.packagers \
 nomnom.xpm \
 gen-ver.sh \
 gen-tr.sh \
 VERSION

desktopdir=	$(datarootdir)/applications
desktop_DATA=	nomnom.desktop

pixmapdir=	$(datarootdir)/pixmaps
pixmap_DATA=	nomnom.xpm

SUBDIRS=	src
if HAVE_LRELEASE
SUBDIRS+=	tr
endif
if WITH_MANUAL
SUBDIRS+=	doc
endif

.PHONY: ChangeLog VERSION

VERSION:
	@if test -d "$(top_srcdir)/.git" -o -f "$(top_srcdir)/.git"; then \
		"$(top_srcdir)/gen-ver.sh" "$(top_srcdir)" > $@; \
	fi

ChangeLog:
	@if test -d "$(top_srcdir)/.git" -o -f "$(top_srcdir)/.git"; then \
		git log --stat --name-only --date=short --abbrev-commit > $@; \
	fi

dist-hook: ChangeLog VERSION
