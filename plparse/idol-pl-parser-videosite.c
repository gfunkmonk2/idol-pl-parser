/*
   Copyright (C) 2010 Bastien Nocera <hadess@hadess.net>

   The Mate Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Mate Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Mate Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301  USA.

   Author: Bastien Nocera <hadess@hadess.net>
 */

#include "config.h"

#include <string.h>
#include <glib.h>

#ifndef IDOL_PL_PARSER_MINI
#include "idol-disc.h"
#endif /* !IDOL_PL_PARSER_MINI */

#ifdef HAVE_QUVI
#include <quvi/quvi.h>
#endif /* HAVE_QUVI */

#include "idol-pl-parser-mini.h"
#include "idol-pl-parser-videosite.h"
#include "idol-pl-parser-private.h"

gboolean
idol_pl_parser_is_videosite (const char *uri, gboolean debug)
{
#ifdef HAVE_QUVI
	quvi_t handle;
	QUVIcode rc;

	if (quvi_init (&handle) != QUVI_OK)
		return FALSE;

	rc = quvi_supported(handle, (char *) uri);
	quvi_close (&handle);

	if (debug)
		g_print ("Checking videosite for URI '%s' returned %d (%s)",
			 uri, rc, (rc == QUVI_OK) ? "true" : "false");

	return (rc == QUVI_OK);
#else
	return FALSE;
#endif /* HAVE_QUVI */
}

#ifndef IDOL_PL_PARSER_MINI

#define getprop(prop, p)					\
	if (quvi_getprop (v, prop, &p) != QUVI_OK)		\
		p = NULL;

IdolPlParserResult
idol_pl_parser_add_videosite (IdolPlParser *parser,
			       GFile *file,
			       GFile *base_file,
			       IdolPlParseData *parse_data,
			       gpointer data)
{
#ifdef HAVE_QUVI
	QUVIcode rc;
	quvi_t handle;
	quvi_video_t v;
	char *uri;
	/* properties */
	const char *video_uri;
	double length;
	char *length_str;
	const char *title;
	const char *id;
	const char *page_uri;
	const char *starttime;
	const char *content_type;

	if (quvi_init (&handle) != QUVI_OK)
		return IDOL_PL_PARSER_RESULT_ERROR;

	uri = g_file_get_uri (file);
	rc = quvi_parse(handle, uri, &v);
	if (rc != QUVI_OK) {
		if (idol_pl_parser_is_debugging_enabled (parser)) {
			g_print ("quvi_parse for '%s' returned %d", uri, rc);
		}
		g_free (uri);
		quvi_close (&handle);
		return IDOL_PL_PARSER_RESULT_ERROR;
	}

	getprop (QUVIPROP_VIDEOURL, video_uri);
	if (quvi_getprop (v, QUVIPROP_VIDEOFILELENGTH, &length) == QUVI_OK)
		length_str = g_strdup_printf ("%f", length);
	else
		length_str = NULL;
	getprop (QUVIPROP_PAGETITLE, title);
	getprop (QUVIPROP_VIDEOID, id);
	getprop (QUVIPROP_PAGEURL, page_uri);
	getprop (QUVIPROP_STARTTIME, starttime);
	getprop (QUVIPROP_VIDEOFILECONTENTTYPE, content_type);

	length_str = g_strdup_printf ("%d", (int) length);
	if (video_uri != NULL)
		idol_pl_parser_add_uri (parser,
					 IDOL_PL_PARSER_FIELD_TITLE, title,
					 IDOL_PL_PARSER_FIELD_ID, id,
					 IDOL_PL_PARSER_FIELD_MOREINFO, page_uri,
					 IDOL_PL_PARSER_FIELD_URI, video_uri,
					 IDOL_PL_PARSER_FIELD_FILESIZE, length_str,
					 IDOL_PL_PARSER_FIELD_STARTTIME, starttime,
					 IDOL_PL_PARSER_FIELD_CONTENT_TYPE, content_type,
					 NULL);
	g_free (uri);
	g_free (length_str);

	quvi_parse_close (&v);
	quvi_close (&handle);

	return IDOL_PL_PARSER_RESULT_SUCCESS;
#else
	return IDOL_PL_PARSER_RESULT_UNHANDLED;
#endif /* !HAVE_QUVI */
}

#endif /* !IDOL_PL_PARSER_MINI */
