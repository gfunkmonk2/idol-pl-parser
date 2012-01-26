/* 
   2002, 2003, 2004, 2005, 2006 Bastien Nocera
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

#ifndef IDOL_PL_PARSER_H
#define IDOL_PL_PARSER_H

#include <glib.h>
#include <gio/gio.h>

#include "idol-pl-parser-features.h"
#include "idol-pl-parser-builtins.h"
#include "idol-pl-playlist.h"

G_BEGIN_DECLS

#define IDOL_TYPE_PL_PARSER            (idol_pl_parser_get_type ())
#define IDOL_PL_PARSER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), IDOL_TYPE_PL_PARSER, IdolPlParser))
#define IDOL_PL_PARSER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), IDOL_TYPE_PL_PARSER, IdolPlParserClass))
#define IDOL_IS_PL_PARSER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IDOL_TYPE_PL_PARSER))
#define IDOL_IS_PL_PARSER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), IDOL_TYPE_PL_PARSER))

/**
 * IdolPlParserResult:
 * @IDOL_PL_PARSER_RESULT_UNHANDLED: The playlist could not be handled.
 * @IDOL_PL_PARSER_RESULT_ERROR: There was an error parsing the playlist.
 * @IDOL_PL_PARSER_RESULT_SUCCESS: The playlist was parsed successfully.
 * @IDOL_PL_PARSER_RESULT_IGNORED: The playlist was ignored due to its scheme or MIME type (see idol_pl_parser_add_ignored_scheme()
 * and idol_pl_parser_add_ignored_mimetype()).
 * @IDOL_PL_PARSER_RESULT_CANCELLED: Parsing of the playlist was cancelled part-way through.
 *
 * Gives the result of parsing a playlist.
 **/
typedef enum {
	IDOL_PL_PARSER_RESULT_UNHANDLED,
	IDOL_PL_PARSER_RESULT_ERROR,
	IDOL_PL_PARSER_RESULT_SUCCESS,
	IDOL_PL_PARSER_RESULT_IGNORED,
	IDOL_PL_PARSER_RESULT_CANCELLED
} IdolPlParserResult;

/**
 * IdolPlParser:
 *
 * All the fields in the #IdolPlParser structure are private and should never be accessed directly.
 **/
typedef struct _IdolPlParser	       IdolPlParser;
typedef struct IdolPlParserClass      IdolPlParserClass;
typedef struct IdolPlParserPrivate    IdolPlParserPrivate;

struct _IdolPlParser {
	GObject parent;
	IdolPlParserPrivate *priv;
};

/* Known metadata fields */

/**
 * IDOL_PL_PARSER_FIELD_URI:
 *
 * Metadata field for an entry's URI.
 *
 * Since: 2.26
 **/
#define IDOL_PL_PARSER_FIELD_URI		"url"
/**
 * IDOL_PL_PARSER_FIELD_GENRE:
 *
 * Metadata field for an entry's genre.
 **/
#define IDOL_PL_PARSER_FIELD_GENRE		"genre"
/**
 * IDOL_PL_PARSER_FIELD_TITLE:
 *
 * Metadata field for an entry's displayable title.
 **/
#define IDOL_PL_PARSER_FIELD_TITLE		"title"
/**
 * IDOL_PL_PARSER_FIELD_AUTHOR:
 *
 * Metadata field for an entry's author/composer/director.
 **/
#define IDOL_PL_PARSER_FIELD_AUTHOR		"author"
/**
 * IDOL_PL_PARSER_FIELD_ALBUM:
 *
 * Metadata field for an entry's album.
 **/
#define IDOL_PL_PARSER_FIELD_ALBUM		"album"
/**
 * IDOL_PL_PARSER_FIELD_BASE:
 *
 * Metadata field for an entry's base path.
 **/
#define IDOL_PL_PARSER_FIELD_BASE		"base"
/**
 * IDOL_PL_PARSER_FIELD_SUBTITLE_URI:
 *
 * The URI of the entry's subtitle file.
 **/
#define IDOL_PL_PARSER_FIELD_SUBTITLE_URI	"subtitle-uri"
/**
 * IDOL_PL_PARSER_FIELD_VOLUME:
 *
 * Metadata field for an entry's playback volume.
 **/
#define IDOL_PL_PARSER_FIELD_VOLUME		"volume"
/**
 * IDOL_PL_PARSER_FIELD_AUTOPLAY:
 *
 * Metadata field for an entry's "autoplay" flag, which is %TRUE if the entry should play automatically.
 **/
#define IDOL_PL_PARSER_FIELD_AUTOPLAY		"autoplay"
/**
 * IDOL_PL_PARSER_FIELD_DURATION:
 *
 * Metadata field for an entry's playback duration, which should be parsed using idol_pl_parser_parse_duration().
 **/
#define IDOL_PL_PARSER_FIELD_DURATION		"duration"
/**
 * IDOL_PL_PARSER_FIELD_DURATION_MS:
 *
 * Metadata field for an entry's playback duration, in milliseconds. It's only used when an entry's
 * duration is available in that format, so one would get either the %IDOL_PL_PARSER_FIELD_DURATION
 * or %IDOL_PL_PARSER_FIELD_DURATION_MS as metadata.
 **/
#define IDOL_PL_PARSER_FIELD_DURATION_MS	"duration-ms"
/**
 * IDOL_PL_PARSER_FIELD_STARTTIME:
 *
 * Metadata field for an entry's playback start time, which should be parsed using idol_pl_parser_parse_duration().
 **/
#define IDOL_PL_PARSER_FIELD_STARTTIME		"starttime"
/**
 * IDOL_PL_PARSER_FIELD_ENDTIME:
 *
 * Metadata field for an entry's playback end time.
 **/
#define IDOL_PL_PARSER_FIELD_ENDTIME		"endtime"
/**
 * IDOL_PL_PARSER_FIELD_COPYRIGHT:
 *
 * Metadata field for an entry's copyright line.
 **/
#define IDOL_PL_PARSER_FIELD_COPYRIGHT		"copyright"
/**
 * IDOL_PL_PARSER_FIELD_ABSTRACT:
 *
 * Metadata field for an entry's abstract text.
 **/
#define IDOL_PL_PARSER_FIELD_ABSTRACT		"abstract"
/**
 * IDOL_PL_PARSER_FIELD_DESCRIPTION:
 *
 * Metadata field for an entry's description.
 **/
#define IDOL_PL_PARSER_FIELD_DESCRIPTION	"description"
/**
 * IDOL_PL_PARSER_FIELD_SUMMARY:
 *
 * Metadata field for an entry's summary. (In practice, identical to %IDOL_PL_PARSER_FIELD_DESCRIPTION.)
 **/
#define IDOL_PL_PARSER_FIELD_SUMMARY		IDOL_PL_PARSER_FIELD_DESCRIPTION
/**
 * IDOL_PL_PARSER_FIELD_MOREINFO:
 *
 * Metadata field for an entry's "more info" URI.
 **/
#define IDOL_PL_PARSER_FIELD_MOREINFO		"moreinfo"
/**
 * IDOL_PL_PARSER_FIELD_SCREENSIZE:
 *
 * Metadata field for an entry's preferred screen size.
 **/
#define IDOL_PL_PARSER_FIELD_SCREENSIZE	"screensize"
/**
 * IDOL_PL_PARSER_FIELD_UI_MODE:
 *
 * Metadata field for an entry's preferred UI mode.
 **/
#define IDOL_PL_PARSER_FIELD_UI_MODE		"ui-mode"
/**
 * IDOL_PL_PARSER_FIELD_PUB_DATE:
 *
 * Metadata field for an entry's publication date, which should be parsed using idol_pl_parser_parse_date().
 **/
#define IDOL_PL_PARSER_FIELD_PUB_DATE		"publication-date"
/**
 * IDOL_PL_PARSER_FIELD_FILESIZE:
 *
 * Metadata field for an entry's filesize in bytes. This is only advisory, and can sometimes not match the actual filesize of the stream.
 **/
#define IDOL_PL_PARSER_FIELD_FILESIZE		"filesize"
/**
 * IDOL_PL_PARSER_FIELD_LANGUAGE:
 *
 * Metadata field for an entry's audio language.
 **/
#define IDOL_PL_PARSER_FIELD_LANGUAGE		"language"
/**
 * IDOL_PL_PARSER_FIELD_CONTACT:
 *
 * Metadata field for an entry's contact details for the webmaster.
 **/
#define IDOL_PL_PARSER_FIELD_CONTACT		"contact"
/**
 * IDOL_PL_PARSER_FIELD_IMAGE_URI:
 *
 * Metadata field for an entry's thumbnail image URI.
 *
 * Since: 2.26
 **/
#define IDOL_PL_PARSER_FIELD_IMAGE_URI		"image-url"
/**
 * IDOL_PL_PARSER_FIELD_DOWNLOAD_URI:
 *
 * Metadata field for an entry's download URI. Only used if an alternate download
 * location is available for the entry.
 *
 * Since: 2.26
 **/
#define IDOL_PL_PARSER_FIELD_DOWNLOAD_URI	"download-url"
/**
 * IDOL_PL_PARSER_FIELD_ID:
 *
 * Metadata field for an entry's identifier. Its use is dependent on the format
 * of the playlist parsed, and its origin.
 **/
#define IDOL_PL_PARSER_FIELD_ID		"id"
/**
 * IDOL_PL_PARSER_FIELD_IS_PLAYLIST:
 *
 * Metadata field used to tell the calling code that the parsing of a playlist
 * started. It is only %TRUE for the metadata passed to #IdolPlParser::playlist-started or
 * #IdolPlParser::playlist-ended signal handlers.
 **/
#define IDOL_PL_PARSER_FIELD_IS_PLAYLIST	"is-playlist"
/**
 * IDOL_PL_PARSER_FIELD_CONTENT_TYPE:
 *
 * Metadata field for an entry's content-type (usually a mime-type coming
 * from a web server).
 **/
#define IDOL_PL_PARSER_FIELD_CONTENT_TYPE	"content-type"

/**
 * IdolPlParserClass:
 * @parent_class: the parent class
 * @entry_parsed: the generic signal handler for the #IdolPlParser::entry-parsed signal,
 * which can be overridden by inheriting classes
 * @playlist_started: the generic signal handler for the #IdolPlParser::playlist-started signal,
 * which can be overridden by inheriting classes
 * @playlist_ended: the generic signal handler for the #IdolPlParser::playlist-ended signal,
 * which can be overridden by inheriting classes
 *
 * The class structure for the #IdolPlParser type.
 **/
struct IdolPlParserClass {
	GObjectClass parent_class;

	/* signals */
	void (*entry_parsed) (IdolPlParser *parser,
			      const char *uri,
			      GHashTable *metadata);
	void (*playlist_started) (IdolPlParser *parser,
				  const char *uri,
				  GHashTable *metadata);
	void (*playlist_ended) (IdolPlParser *parser,
				const char *uri);
};

/**
 * IdolPlParserType:
 * @IDOL_PL_PARSER_PLS: PLS parser
 * @IDOL_PL_PARSER_M3U: M3U parser
 * @IDOL_PL_PARSER_M3U_DOS: M3U (DOS linebreaks) parser
 * @IDOL_PL_PARSER_XSPF: XSPF parser
 * @IDOL_PL_PARSER_IRIVER_PLA: iRiver PLA parser
 *
 * The type of playlist a #IdolPlParser will parse.
 **/
typedef enum {
	IDOL_PL_PARSER_PLS,
	IDOL_PL_PARSER_M3U,
	IDOL_PL_PARSER_M3U_DOS,
	IDOL_PL_PARSER_XSPF,
	IDOL_PL_PARSER_IRIVER_PLA,
} IdolPlParserType;

/**
 * IdolPlParserError:
 * @IDOL_PL_PARSER_ERROR_NO_DISC: Error attempting to open a disc device when no disc is present
 * @IDOL_PL_PARSER_ERROR_MOUNT_FAILED: An attempted mount operation failed
 * @IDOL_PL_PARSER_ERROR_EMPTY_PLAYLIST: Playlist to be saved is empty
 *
 * Allows you to differentiate between different
 * errors occurring during file operations in a #IdolPlParser.
 **/
typedef enum {
	IDOL_PL_PARSER_ERROR_NO_DISC,
	IDOL_PL_PARSER_ERROR_MOUNT_FAILED,
	IDOL_PL_PARSER_ERROR_EMPTY_PLAYLIST
} IdolPlParserError;

#define IDOL_PL_PARSER_ERROR (idol_pl_parser_error_quark ())

GQuark idol_pl_parser_error_quark (void);

GType    idol_pl_parser_get_type (void);

gint64  idol_pl_parser_parse_duration (const char *duration, gboolean debug);
guint64 idol_pl_parser_parse_date     (const char *date_str, gboolean debug);

gboolean idol_pl_parser_save (IdolPlParser      *parser,
			       IdolPlPlaylist    *playlist,
			       GFile              *dest,
			       const gchar        *title,
			       IdolPlParserType   type,
			       GError            **error);

void	   idol_pl_parser_add_ignored_scheme (IdolPlParser *parser,
					       const char *scheme);
void       idol_pl_parser_add_ignored_mimetype (IdolPlParser *parser,
						 const char *mimetype);

IdolPlParserResult idol_pl_parser_parse (IdolPlParser *parser,
					   const char *uri, gboolean fallback);
void idol_pl_parser_parse_async (IdolPlParser *parser, const char *uri,
				  gboolean fallback, GCancellable *cancellable,
				  GAsyncReadyCallback callback,
                                  gpointer user_data);
IdolPlParserResult idol_pl_parser_parse_finish (IdolPlParser *parser,
						  GAsyncResult *async_result,
						  GError **error);

IdolPlParserResult idol_pl_parser_parse_with_base (IdolPlParser *parser,
						     const char *uri,
						     const char *base,
						     gboolean fallback);
void idol_pl_parser_parse_with_base_async (IdolPlParser *parser,
					    const char *uri, const char *base,
					    gboolean fallback,
					    GCancellable *cancellable,
					    GAsyncReadyCallback callback,
                    			    gpointer user_data);

IdolPlParser *idol_pl_parser_new (void);

GType idol_pl_parser_metadata_get_type (void) G_GNUC_CONST;
#define IDOL_TYPE_PL_PARSER_METADATA (idol_pl_parser_metadata_get_type())

G_END_DECLS

#endif /* IDOL_PL_PARSER_H */
