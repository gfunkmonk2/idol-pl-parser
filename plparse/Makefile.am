SUBDIRS = . tests
DIST_SUBDIRS = tests

lib_LTLIBRARIES = libidol-plparser.la libidol-plparser-mini.la

MARSHALFILES = idolplparser-marshal.c idolplparser-marshal.h
BUILT_SOURCES = $(MARSHALFILES)

idolplparser-marshal.c: idolplparser-marshal.list
	 $(AM_V_GEN) $(GLIB_GENMARSHAL) --prefix=idolplparser_marshal --header --body --internal $< > $@

idolplparser-marshal.h: idolplparser-marshal.list
	$(AM_V_GEN) $(GLIB_GENMARSHAL) --prefix=idolplparser_marshal --header --internal $< > $@

plparserincludedir = $(pkgincludedir)/1/plparser
plparserinclude_HEADERS =			\
	idol-pl-parser-builtins.h		\
	idol-pl-parser-features.h		\
	idol-pl-parser.h			\
	idol-pl-playlist.h			\
	idol-disc.h				\
	idol-pl-parser-mini.h

plparser_sources =				\
	idol-pl-parser-builtins.c		\
	idol-disc.c				\
	idol-pl-parser.c			\
	idol-pl-parser-lines.c			\
	idol-pl-parser-media.c			\
	idol-pl-parser-misc.c			\
	idol-pl-parser-pla.c			\
	idol-pl-parser-pls.c			\
	idol-pl-parser-podcast.c		\
	idol-pl-parser-qt.c			\
	idol-pl-parser-smil.c			\
	idol-pl-parser-videosite.c		\
	idol-pl-parser-wm.c			\
	idol-pl-parser-xspf.c

nodist_libidol_plparser_la_SOURCES = $(MARSHALFILES)
dist_libidol_plparser_la_SOURCES =		\
	$(plparser_sources)			\
	idol-pl-parser-builtins.h		\
	idol-disc.h				\
	idol-pl-parser-features.h		\
	idol-pl-parser.h			\
	idol-pl-parser-lines.h			\
	idol-pl-parser-media.h			\
	idol-pl-parser-misc.h			\
	idol-pl-parser-pla.h			\
	idol-pl-parser-pls.h			\
	idol-pl-parser-podcast.h		\
	idol-pl-parser-private.h		\
	idol-pl-parser-qt.h			\
	idol-pl-parser-smil.h			\
	idol-pl-parser-videosite.h		\
	idol-pl-parser-wm.h			\
	idol-pl-parser-xspf.h			\
	idol-pl-playlist.c			\
	idol-pl-playlist.h			\
	xmlparser.h xmlparser.c			\
	xmllexer.h xmllexer.c			\
	bswap.h

libidol_plparser_la_CPPFLAGS = \
	-I$(top_srcdir)				\
	-I$(top_srcdir)/lib			\
	-I$(top_builddir)/plparse		\
	-DMATELOCALEDIR=\""$(datadir)/locale"\"\
	$(DISABLE_DEPRECATED)			\
	$(AM_CPPFLAGS)

libidol_plparser_la_CFLAGS = \
	$(IDOL_PLPARSER_CFLAGS)	\
	$(DBUS_CFLAGS)			\
	$(WARN_CFLAGS)			\
	$(AM_CFLAGS)			\
	-D_FILE_OFFSET_BITS=64		\
	-D_LARGEFILE64_SOURCE		\
	-I$(top_srcdir)/lib		\
	-D_GNU_SOURCE

libidol_plparser_la_LIBADD = \
	$(IDOL_PLPARSER_LIBS)			\
	$(top_builddir)/lib/libidol_glibc.la

libidol_plparser_la_LDFLAGS =			\
	-version-info $(PLPARSER_LT_VERSION)	\
	-export-symbols $(srcdir)/plparser.symbols \
	-no-undefined				\
	$(AM_LDFLAGS)

libidol_plparser_mini_la_SOURCES =		\
	idol-pl-parser-mini.h			\
	idol-pl-parser.c			\
	idol-pl-parser-lines.c			\
	idol-pl-parser-lines.h			\
	idol-pl-parser-misc.c			\
	idol-pl-parser-misc.h			\
	idol-pl-parser-pls.c			\
	idol-pl-parser-pls.h			\
	idol-pl-parser-podcast.c		\
	idol-pl-parser-podcast.h		\
	idol-pl-parser-private.h		\
	idol-pl-parser-qt.c			\
	idol-pl-parser-qt.h			\
	idol-pl-parser-smil.c			\
	idol-pl-parser-smil.h			\
	idol-pl-parser-videosite.c		\
	idol-pl-parser-videosite.h		\
	idol-pl-parser-wm.c			\
	idol-pl-parser-wm.h			\
	idol-pl-parser-xspf.c			\
	idol-pl-parser-xspf.h			\
	idol-pl-playlist.c			\
	idol-pl-playlist.h

libidol_plparser_mini_la_CPPFLAGS = \
	-I$(top_srcdir)			\
	-I$(top_srcdir)/lib		\
	-I$(top_builddir)/plparse	\
	-DIDOL_PL_PARSER_MINI		\
	$(DISABLE_DEPRECATED)		\
	$(AM_CPPFLAGS)

libidol_plparser_mini_la_CFLAGS = \
	$(IDOL_PLPARSER_MINI_CFLAGS)	\
	$(WARN_CFLAGS)			\
	$(AM_CFLAGS)

libidol_plparser_mini_la_LIBADD =		\
	$(IDOL_PLPARSER_MINI_LIBS)		\
	$(top_builddir)/lib/libidol_glibc.la

libidol_plparser_mini_la_LDFLAGS =			\
	-version-info $(PLPARSER_LT_VERSION)		\
	-export-symbols $(srcdir)/plparser-mini.symbols	\
	-no-undefined					\
	$(AM_LDFLAGS)

idol-pl-parser-builtins.h: stamp-idol-pl-parser-builtins.h
	@true

stamp-idol-pl-parser-builtins.h: idol-pl-parser.h idol-disc.h Makefile
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) \
			--fhead "#ifndef __IDOL_PL_PARSER_BUILTINS_H__\n#define __IDOL_PL_PARSER_BUILTINS_H__\n\n#include <glib-object.h>\n\nG_BEGIN_DECLS\n" \
			--fprod "/* enumerations from \"@filename@\" */\n" \
			--vhead "GType @enum_name@_get_type (void) G_GNUC_CONST;\n#define IDOL_TYPE_@ENUMSHORT@ (@enum_name@_get_type())\n" \
			--ftail "G_END_DECLS\n\n#endif /* __IDOL_PL_PARSER_BUILTINS_H__ */" idol-pl-parser.h idol-disc.h) >> xgen-gtbh \
	&& (cmp -s xgen-gtbh idol-pl-parser-builtins.h || cp xgen-gtbh idol-pl-parser-builtins.h ) \
	&& rm -f xgen-gtbh \
	&& echo timestamp > $(@F)

idol-pl-parser-builtins.c: idol-pl-parser.h idol-disc.h Makefile idol-pl-parser-builtins.h
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) \
			--fhead "#include \"idol-pl-parser.h\"\n#include \"idol-disc.h\"\n#include \"idol-pl-parser-builtins.h\"" \
			--fprod "\n/* enumerations from \"@filename@\" */" \
			--vhead "GType\n@enum_name@_get_type (void)\n{\n  static GType etype = 0;\n  if (etype == 0) {\n    static const G@Type@Value values[] = {" \
			--vprod "      { @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
			--vtail "      { 0, NULL, NULL }\n    };\n    etype = g_@type@_register_static (\"@EnumName@\", values);\n  }\n  return etype;\n}\n" \
		idol-pl-parser.h idol-disc.h) > xgen-gtbc \
	&& cp xgen-gtbc idol-pl-parser-builtins.c  \
	&& rm -f xgen-gtbc

CLEANFILES = \
	idol-pl-parser-builtins.h \
	idol-pl-parser-builtins.c \
	stamp-idol-pl-parser-builtins.h \
	$(BUILT_SOURCES)

DISTCLEANFILES = $(CLEANFILES)

EXTRA_DIST =				\
	idolplparser-marshal.list	\
	idol-pl-parser-features.h.in	\
	plparser.symbols		\
	plparser-mini.symbols

# Introspection
-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir) --add-include-path=$(builddir) --warn-all
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir)

if HAVE_INTROSPECTION
introspection_sources = \
	$(plparser_sources)		\
	$(plparserinclude_HEADERS)	\
	idolplparser-marshal.c

IdolPlParser-1.0.gir: libidol-plparser.la Makefile idolplparser-marshal.c idolplparser-marshal.h
IdolPlParser_1_0_gir_SCANNERFLAGS = --namespace=IdolPlParser --identifier-prefix=IdolPl --symbol-prefix=idol_pl --identifier-prefix=IdolDisc --symbol-prefix=idol_disc --identifier-prefix=IdolCd --symbol-prefix=idol_cd --nsversion=1.0
IdolPlParser_1_0_gir_INCLUDES = GObject-2.0 Gio-2.0
IdolPlParser_1_0_gir_CFLAGS = \
	$(IDOL_PLPARSER_CFLAGS)	\
	$(DBUS_CFLAGS)			\
	$(AM_CFLAGS)			\
	-D_FILE_OFFSET_BITS=64		\
	-D_LARGEFILE64_SOURCE		\
	-I$(top_srcdir)			\
	-I$(top_srcdir)/lib		\
	-I$(top_builddir)/plparse	\
	-D_GNU_SOURCE
IdolPlParser_1_0_gir_LIBS = libidol-plparser.la
IdolPlParser_1_0_gir_FILES = $(introspection_sources)
INTROSPECTION_GIRS += IdolPlParser-1.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif
