/* 
   2002, 2003, 2004, 2005, 2006, 2007 Bastien Nocera
   Copyright (C) 2003 Colin Walters <walters@verbum.org>

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

#ifndef IDOL_PL_PARSER_MEDIA_H
#define IDOL_PL_PARSER_MEDIA_H

G_BEGIN_DECLS

#ifndef IDOL_PL_PARSER_MINI
#include "idol-pl-parser.h"
#include "idol-pl-parser-private.h"
#include <gio/gio.h>
#endif /* !IDOL_PL_PARSER_MINI */

#ifndef IDOL_PL_PARSER_MINI
IdolPlParserResult idol_pl_parser_add_iso (IdolPlParser *parser,
					     GFile *file,
					     GFile *base_file,
					     IdolPlParseData *parse_data,
					     gpointer data);
IdolPlParserResult idol_pl_parser_add_cue (IdolPlParser *parser,
					     GFile *file,
					     GFile *base_file,
					     IdolPlParseData *parse_data,
					     gpointer data);
IdolPlParserResult idol_pl_parser_add_directory (IdolPlParser *parser,
						   GFile *file,
						   GFile *base_file,
						   IdolPlParseData *parse_data,
						   gpointer data);
IdolPlParserResult idol_pl_parser_add_block (IdolPlParser *parser,
					       GFile *file,
					       GFile *base_file,
					       IdolPlParseData *parse_data,
					       gpointer data);
#endif /* !IDOL_PL_PARSER_MINI */

G_END_DECLS

#endif /* IDOL_PL_PARSER_MEDIA_H */
