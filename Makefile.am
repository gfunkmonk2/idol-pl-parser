SUBDIRS = lib plparse po docs

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

EXTRA_DIST =			\
	COPYING.LIB		\
	autogen.sh		\
	intltool-extract.in	\
	intltool-merge.in	\
	intltool-update.in	\
	AUTHORS NEWS		\
	gtk-doc.make		\
	idol-plparser.pc.in	\
	idol-plparser-mini.pc	\
	idol-pl-parser.spec

CLEANFILES = idol-plparser.pc

DISTCLEANFILES = intltool-extract intltool-merge intltool-update \
		 mate-doc-utils.make gtk-doc.make

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = idol-plparser.pc idol-plparser-mini.pc

DISTCHECK_CONFIGURE_FLAGS = --disable-scrollkeeper --enable-gtk-doc --enable-introspection

# Build ChangeLog from GIT history
ChangeLog:
	@if test -f $(top_srcdir)/.git/HEAD; then \
		git log --stat --after="Mon Jul 20 23:47:57 2009" > $@; \
	fi

dist: ChangeLog

.PHONY: ChangeLog
