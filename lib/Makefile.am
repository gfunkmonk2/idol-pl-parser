noinst_LTLIBRARIES =					\
	libidol_glibc.la

common_defines = \
	-DMATELOCALEDIR=\""$(datadir)/locale"\"	\
	-DDATADIR=\""$(datadir)"\"			\
	$(DISABLE_DEPRECATED)

noinst_HEADERS = idol_internal.h

libidol_glibc_la_SOURCES = stub.c
libidol_glibc_la_LIBADD = @LTLIBOBJS@

EXTRA_DIST = asprintf.c
