/* 
   Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007 Bastien Nocera
   Copyright (C) 2003, 2004 Colin Walters <walters@rhythmbox.org>

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

#include "idol-pl-parser.h"
#include "idolplparser-marshal.h"
#include "idol-disc.h"
#endif /* !IDOL_PL_PARSER_MINI */

#include "idol-pl-parser-mini.h"
#include "idol-pl-parser-misc.h"
#include "idol-pl-parser-private.h"

#ifndef IDOL_PL_PARSER_MINI
IdolPlParserResult
idol_pl_parser_add_gvp (IdolPlParser *parser,
			 GFile *file,
			 GFile *base_file,
			 IdolPlParseData *parse_data,
			 gpointer data)
{
	IdolPlParserResult retval = IDOL_PL_PARSER_RESULT_UNHANDLED;
	char *contents, **lines, *title, *link, *version;
	gsize size;

	if (g_file_load_contents (file, NULL, &contents, &size, NULL, NULL) == FALSE)
		return IDOL_PL_PARSER_RESULT_ERROR;

	if (g_str_has_prefix (contents, "#.download.the.free.Google.Video.Player") == FALSE && g_str_has_prefix (contents, "# download the free Google Video Player") == FALSE) {
		g_free (contents);
		return retval;
	}

	lines = g_strsplit (contents, "\n", 0);
	g_free (contents);

	/* We only handle GVP version 1.1 for now */
	version = idol_pl_parser_read_ini_line_string_with_sep (lines, "gvp_version", ":");
	if (version == NULL || strcmp (version, "1.1") != 0) {
		g_free (version);
		g_strfreev (lines);
		return retval;
	}
	g_free (version);

	link = idol_pl_parser_read_ini_line_string_with_sep (lines, "url", ":");
	if (link == NULL) {
		g_strfreev (lines);
		return retval;
	}

	retval = IDOL_PL_PARSER_RESULT_SUCCESS;

	title = idol_pl_parser_read_ini_line_string_with_sep (lines, "title", ":");

	idol_pl_parser_add_one_uri (parser, link, title);

	g_free (link);
	g_free (title);
	g_strfreev (lines);

	return retval;
}

IdolPlParserResult
idol_pl_parser_add_desktop (IdolPlParser *parser,
			     GFile *file,
			     GFile *base_file,
			     IdolPlParseData *parse_data,
			     gpointer data)
{
	char *contents, **lines;
	const char *path, *display_name, *type;
	GFile *target;
	gsize size;
	IdolPlParserResult res = IDOL_PL_PARSER_RESULT_ERROR;

	if (g_file_load_contents (file, NULL, &contents, &size, NULL, NULL) == FALSE)
		return res;

	lines = g_strsplit (contents, "\n", 0);
	g_free (contents);

	type = idol_pl_parser_read_ini_line_string (lines, "Type");
	if (type == NULL)
		goto bail;
	
	if (g_ascii_strcasecmp (type, "Link") != 0
	    && g_ascii_strcasecmp (type, "FSDevice") != 0) {
		goto bail;
	}

	path = idol_pl_parser_read_ini_line_string (lines, "URL");
	if (path == NULL)
		goto bail;
	target = g_file_new_for_uri (path);

	display_name = idol_pl_parser_read_ini_line_string (lines, "Name");

	if (idol_pl_parser_ignore (parser, path) == FALSE
	    && g_ascii_strcasecmp (type, "FSDevice") != 0) {
		idol_pl_parser_add_one_file (parser, target, display_name);
	} else {
		if (idol_pl_parser_parse_internal (parser, target, NULL, parse_data) != IDOL_PL_PARSER_RESULT_SUCCESS)
			idol_pl_parser_add_one_file (parser, target, display_name);
	}

	res = IDOL_PL_PARSER_RESULT_SUCCESS;

bail:
	g_strfreev (lines);

	return res;
}

#endif /* !IDOL_PL_PARSER_MINI */
