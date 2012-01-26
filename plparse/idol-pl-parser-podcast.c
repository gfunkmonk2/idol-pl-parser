/* 
   Copyright (C) 2007 Bastien Nocera <hadess@hadess.net>

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
#include <libsoup/soup-gnome.h>
#include "xmlparser.h"
#include "idol-pl-parser.h"
#include "idolplparser-marshal.h"
#include "idol-disc.h"
#endif /* !IDOL_PL_PARSER_MINI */

#include "idol-pl-parser-mini.h"
#include "idol-pl-parser-podcast.h"
#include "idol-pl-parser-private.h"

#define RSS_NEEDLE "<rss "
#define RSS_NEEDLE2 "<rss\n"
#define ATOM_NEEDLE "<feed "
#define OPML_NEEDLE "<opml "

const char *
idol_pl_parser_is_rss (const char *data, gsize len)
{
	if (len == 0)
		return FALSE;
	if (len > MIME_READ_CHUNK_SIZE)
		len = MIME_READ_CHUNK_SIZE;

	if (g_strstr_len (data, len, RSS_NEEDLE) != NULL)
		return RSS_MIME_TYPE;
	if (g_strstr_len (data, len, RSS_NEEDLE2) != NULL)
		return RSS_MIME_TYPE;

	return NULL;
}

const char *
idol_pl_parser_is_atom (const char *data, gsize len)
{
	if (len == 0)
		return FALSE;
	if (len > MIME_READ_CHUNK_SIZE)
		len = MIME_READ_CHUNK_SIZE;

	if (g_strstr_len (data, len, ATOM_NEEDLE) != NULL)
		return ATOM_MIME_TYPE;

	return NULL;
}

const char *
idol_pl_parser_is_opml (const char *data, gsize len)
{
	if (len == 0)
		return FALSE;
	if (len > MIME_READ_CHUNK_SIZE)
		len = MIME_READ_CHUNK_SIZE;

	if (g_strstr_len (data, len, OPML_NEEDLE) != NULL)
		return OPML_MIME_TYPE;

	return NULL;
}

const char *
idol_pl_parser_is_xml_feed (const char *data, gsize len)
{
	if (idol_pl_parser_is_rss (data, len) != NULL)
		return RSS_MIME_TYPE;
	if (idol_pl_parser_is_atom (data, len) != NULL)
		return ATOM_MIME_TYPE;
	if (idol_pl_parser_is_opml (data, len) != NULL)
		return OPML_MIME_TYPE;
	return NULL;
}

#ifndef IDOL_PL_PARSER_MINI

static IdolPlParserResult
parse_rss_item (IdolPlParser *parser, xml_node_t *parent)
{
	const char *title, *uri, *description, *author;
	const char *pub_date, *duration, *filesize, *content_type;
	xml_node_t *node;

	title = uri = description = author = content_type = NULL;
	pub_date = duration = filesize = NULL;

	for (node = parent->child; node != NULL; node = node->next) {
		if (node->name == NULL)
			continue;

		if (g_ascii_strcasecmp (node->name, "title") == 0) {
			title = node->data;
		} else if (g_ascii_strcasecmp (node->name, "url") == 0) {
			uri = node->data;
		} else if (g_ascii_strcasecmp (node->name, "pubDate") == 0) {
			pub_date = node->data;
		} else if (g_ascii_strcasecmp (node->name, "description") == 0
			   || g_ascii_strcasecmp (node->name, "itunes:summary") == 0) {
			description = node->data;
		} else if (g_ascii_strcasecmp (node->name, "author") == 0
			   || g_ascii_strcasecmp (node->name, "itunes:author") == 0) {
			author = node->data;
		} else if (g_ascii_strcasecmp (node->name, "itunes:duration") == 0) {
			duration = node->data;
		} else if (g_ascii_strcasecmp (node->name, "length") == 0) {
			filesize = node->data;
		} else if (g_ascii_strcasecmp (node->name, "media:content") == 0) {
			const char *tmp;

			tmp = xml_parser_get_property (node, "type");
			if (tmp != NULL &&
			    g_str_has_prefix (tmp, "audio/") == FALSE)
				continue;
			content_type = tmp;

			tmp = xml_parser_get_property (node, "url");
			if (tmp != NULL)
				uri = tmp;
			else
				continue;

			tmp = xml_parser_get_property (node, "fileSize");
			if (tmp != NULL)
				filesize = tmp;

			tmp = xml_parser_get_property (node, "duration");
			if (tmp != NULL)
				duration = tmp;
		} else if (g_ascii_strcasecmp (node->name, "enclosure") == 0) {
			const char *tmp;

			tmp = xml_parser_get_property (node, "url");
			if (tmp != NULL)
				uri = tmp;
			else
				continue;
			tmp = xml_parser_get_property (node, "length");
			if (tmp != NULL)
				filesize = tmp;
		}
	}

	if (uri != NULL) {
		idol_pl_parser_add_uri (parser,
					 IDOL_PL_PARSER_FIELD_URI, uri,
					 IDOL_PL_PARSER_FIELD_TITLE, title,
					 IDOL_PL_PARSER_FIELD_PUB_DATE, pub_date,
					 IDOL_PL_PARSER_FIELD_DESCRIPTION, description,
					 IDOL_PL_PARSER_FIELD_AUTHOR, author,
					 IDOL_PL_PARSER_FIELD_DURATION, duration,
					 IDOL_PL_PARSER_FIELD_FILESIZE, filesize,
					 IDOL_PL_PARSER_FIELD_CONTENT_TYPE, content_type,
					 NULL);
	}

	return IDOL_PL_PARSER_RESULT_SUCCESS;
}

static IdolPlParserResult
parse_rss_items (IdolPlParser *parser, const char *uri, xml_node_t *parent)
{
	const char *title, *language, *description, *author;
	const char *contact, *img, *pub_date, *copyright;
	xml_node_t *node;

	title = language = description = author = NULL;
	contact = img = pub_date = copyright = NULL;

	/* We need to parse for the feed metadata first, then for the items */
	for (node = parent->child; node != NULL; node = node->next) {
		if (node->name == NULL)
			continue;

		if (g_ascii_strcasecmp (node->name, "title") == 0) {
			title = node->data;
		} else if (g_ascii_strcasecmp (node->name, "language") == 0) {
			language = node->data;
		} else if (g_ascii_strcasecmp (node->name, "description") == 0
			 || g_ascii_strcasecmp (node->name, "itunes:subtitle") == 0) {
		    	description = node->data;
		} else if (g_ascii_strcasecmp (node->name, "author") == 0
			 || g_ascii_strcasecmp (node->name, "itunes:author") == 0
			 || (g_ascii_strcasecmp (node->name, "generator") == 0 && author == NULL)) {
		    	author = node->data;
		} else if (g_ascii_strcasecmp (node->name, "webMaster") == 0) {
			contact = node->data;
		} else if (g_ascii_strcasecmp (node->name, "image") == 0) {
			img = node->data;
		} else if (g_ascii_strcasecmp (node->name, "itunes:image") == 0) {
			const char *href;

			href = xml_parser_get_property (node, "href");
			if (href != NULL)
				img = href;
		} else if (g_ascii_strcasecmp (node->name, "lastBuildDate") == 0
			 || g_ascii_strcasecmp (node->name, "pubDate") == 0) {
		    	pub_date = node->data;
		} else if (g_ascii_strcasecmp (node->name, "copyright") == 0) {
			copyright = node->data;
		}
	}

	/* Send the info we already have about the feed */
	idol_pl_parser_add_uri (parser,
				 IDOL_PL_PARSER_FIELD_IS_PLAYLIST, TRUE,
				 IDOL_PL_PARSER_FIELD_URI, uri,
				 IDOL_PL_PARSER_FIELD_TITLE, title,
				 IDOL_PL_PARSER_FIELD_LANGUAGE, language,
				 IDOL_PL_PARSER_FIELD_DESCRIPTION, description,
				 IDOL_PL_PARSER_FIELD_AUTHOR, author,
				 IDOL_PL_PARSER_FIELD_PUB_DATE, pub_date,
				 IDOL_PL_PARSER_FIELD_COPYRIGHT, copyright,
				 IDOL_PL_PARSER_FIELD_IMAGE_URI, img,
				 IDOL_PL_PARSER_FIELD_CONTACT, contact,
				 NULL);

	for (node = parent->child; node != NULL; node = node->next) {
		if (node->name == NULL)
			continue;

		if (g_ascii_strcasecmp (node->name, "item") == 0)
			parse_rss_item (parser, node);
	}

	idol_pl_parser_playlist_end (parser, uri);

	return IDOL_PL_PARSER_RESULT_SUCCESS;
}

IdolPlParserResult
idol_pl_parser_add_rss (IdolPlParser *parser,
			 GFile *file,
			 GFile *base_file,
			 IdolPlParseData *parse_data,
			 gpointer data)
{
#ifndef HAVE_GMIME
	WARN_NO_GMIME;
#else
	xml_node_t* doc, *channel;
	char *contents;
	gsize size;

	if (g_file_load_contents (file, NULL, &contents, &size, NULL, NULL) == FALSE)
		return IDOL_PL_PARSER_RESULT_ERROR;

	doc = idol_pl_parser_parse_xml_relaxed (contents, size);
	if (doc == NULL) {
		g_free (contents);
		return IDOL_PL_PARSER_RESULT_ERROR;
	}

	/* If the document has no name */
	if (doc->name == NULL
	    || (g_ascii_strcasecmp (doc->name , "rss") != 0
		&& g_ascii_strcasecmp (doc->name , "rss\n") != 0)) {
		g_free (contents);
		xml_parser_free_tree (doc);
		return IDOL_PL_PARSER_RESULT_ERROR;
	}

	for (channel = doc->child; channel != NULL; channel = channel->next) {
		if (g_ascii_strcasecmp (channel->name, "channel") == 0) {
			char *uri;

			uri = g_file_get_uri (file);
			parse_rss_items (parser, uri, channel);
			g_free (uri);

			/* One channel per file */
			break;
		}
	}

	g_free (contents);
	xml_parser_free_tree (doc);

	return IDOL_PL_PARSER_RESULT_SUCCESS;
#endif /* !HAVE_GMIME */
}

/* http://www.apple.com/itunes/store/podcaststechspecs.html */
IdolPlParserResult
idol_pl_parser_add_itpc (IdolPlParser *parser,
			  GFile *file,
			  GFile *base_file,
			  IdolPlParseData *parse_data,
			  gpointer data)
{
#ifndef HAVE_GMIME
	WARN_NO_GMIME;
#else
	IdolPlParserResult ret;
	char *uri, *new_uri, *uri_scheme;
	GFile *new_file;

	uri = g_file_get_uri (file);
	uri_scheme = g_file_get_uri_scheme (file);
	new_uri = g_strdup_printf ("http%s", uri + strlen (uri_scheme));
	g_free (uri);
	g_free (uri_scheme);

	new_file = g_file_new_for_uri (new_uri);
	g_free (new_uri);

	ret = idol_pl_parser_add_rss (parser, new_file, base_file, parse_data, data);

	g_object_unref (new_file);

	return ret;
#endif /* !HAVE_GMIME */
}

IdolPlParserResult
idol_pl_parser_add_zune (IdolPlParser *parser,
			  GFile *file,
			  GFile *base_file,
			  IdolPlParseData *parse_data,
			  gpointer data)
{
#ifndef HAVE_GMIME
	WARN_NO_GMIME;
#else
	IdolPlParserResult ret;
	char *uri, *new_uri;
	GFile *new_file;

	uri = g_file_get_uri (file);
	if (g_str_has_prefix (uri, "zune://subscribe/?") == FALSE) {
		g_free (uri);
		return IDOL_PL_PARSER_RESULT_UNHANDLED;
	}

	new_uri = strchr (uri + strlen ("zune://subscribe/?"), '=');
	if (new_uri == NULL) {
		g_free (uri);
		return IDOL_PL_PARSER_RESULT_UNHANDLED;
	}
	/* Skip over the '=' */
	new_uri++;

	new_file = g_file_new_for_uri (new_uri);
	g_free (uri);

	ret = idol_pl_parser_add_rss (parser, new_file, base_file, parse_data, data);

	g_object_unref (new_file);

	return ret;
#endif /* !HAVE_GMIME */
}

/* Atom docs:
 * http://www.atomenabled.org/developers/syndication/atom-format-spec.php#rfc.section.4.1
 * http://tools.ietf.org/html/rfc4287
 * http://tools.ietf.org/html/rfc4946 */
static IdolPlParserResult
parse_atom_entry (IdolPlParser *parser, xml_node_t *parent)
{
	const char *title, *author, *uri, *filesize;
	const char *copyright, *pub_date, *description;
	xml_node_t *node;

	title = author = uri = filesize = NULL;
	copyright = pub_date = description = NULL;

	for (node = parent->child; node != NULL; node = node->next) {
		if (node->name == NULL)
			continue;

		if (g_ascii_strcasecmp (node->name, "title") == 0) {
			title = node->data;
		} else if (g_ascii_strcasecmp (node->name, "author") == 0) {
			//FIXME
		} else if (g_ascii_strcasecmp (node->name, "link") == 0) {
			const char *rel;

			//FIXME how do we choose the default enclosure type?
			rel = xml_parser_get_property (node, "rel");
			if (g_ascii_strcasecmp (rel, "enclosure") == 0) {
				const char *href;

				//FIXME what's the difference between url and href there?
				href = xml_parser_get_property (node, "href");
				if (href == NULL)
					continue;
				uri = href;
				filesize = xml_parser_get_property (node, "length");
			} else if (g_ascii_strcasecmp (node->name, "license") == 0) {
				const char *href;

				href = xml_parser_get_property (node, "href");
				if (href == NULL)
					continue;
				/* This isn't really a copyright, but what the hey */
				copyright = href;
			}
		} else if (g_ascii_strcasecmp (node->name, "updated") == 0
			   || (g_ascii_strcasecmp (node->name, "modified") == 0 && pub_date == NULL)) {
			pub_date = node->data;
		} else if (g_ascii_strcasecmp (node->name, "summary") == 0
			   || (g_ascii_strcasecmp (node->name, "content") == 0 && description == NULL)) {
			const char *type;

			type = xml_parser_get_property (node, "content");
			if (type != NULL && g_ascii_strcasecmp (type, "text/plain") == 0)
				description = node->data;
		}
		//FIXME handle category
	}

	if (uri != NULL) {
		idol_pl_parser_add_uri (parser,
					 IDOL_PL_PARSER_FIELD_TITLE, title,
					 IDOL_PL_PARSER_FIELD_AUTHOR, author,
					 IDOL_PL_PARSER_FIELD_URI, uri,
					 IDOL_PL_PARSER_FIELD_FILESIZE, filesize,
					 IDOL_PL_PARSER_FIELD_COPYRIGHT, copyright,
					 IDOL_PL_PARSER_FIELD_PUB_DATE, pub_date,
					 IDOL_PL_PARSER_FIELD_DESCRIPTION, description,
					 NULL);
	}

	return IDOL_PL_PARSER_RESULT_SUCCESS;
}

static IdolPlParserResult
parse_atom_entries (IdolPlParser *parser, const char *uri, xml_node_t *parent)
{
	const char *title, *pub_date, *description;
	const char *author, *img;
	xml_node_t *node;
	gboolean started = FALSE;

	title = pub_date = description = NULL;
	author = img = NULL;

	for (node = parent->child; node != NULL; node = node->next) {
		if (node->name == NULL)
			continue;

		if (g_ascii_strcasecmp (node->name, "title") == 0) {
			title = node->data;
		} else if (g_ascii_strcasecmp (node->name, "tagline") == 0) {
		    	description = node->data;
		} else if (g_ascii_strcasecmp (node->name, "modified") == 0
			   || g_ascii_strcasecmp (node->name, "updated") == 0) {
			pub_date = node->data;
		} else if (g_ascii_strcasecmp (node->name, "author") == 0
			 || (g_ascii_strcasecmp (node->name, "generator") == 0 && author == NULL)) {
		    	author = node->data;
		} else if ((g_ascii_strcasecmp (node->name, "icon") == 0 && img == NULL)
			   || g_ascii_strcasecmp (node->name, "logo") == 0) {
			img = node->data;
		}

		if (g_ascii_strcasecmp (node->name, "entry") == 0) {
			if (started == FALSE) {
				/* Send the info we already have about the feed */
				idol_pl_parser_add_uri (parser,
							 IDOL_PL_PARSER_FIELD_IS_PLAYLIST, TRUE,
							 IDOL_PL_PARSER_FIELD_URI, uri,
							 IDOL_PL_PARSER_FIELD_TITLE, title,
							 IDOL_PL_PARSER_FIELD_DESCRIPTION, description,
							 IDOL_PL_PARSER_FIELD_AUTHOR, author,
							 IDOL_PL_PARSER_FIELD_PUB_DATE, pub_date,
							 IDOL_PL_PARSER_FIELD_IMAGE_URI, img,
							 NULL);
				started = TRUE;
			}

			parse_atom_entry (parser, node);
		}
	}

	idol_pl_parser_playlist_end (parser, uri);

	return IDOL_PL_PARSER_RESULT_SUCCESS;
}

IdolPlParserResult
idol_pl_parser_add_atom (IdolPlParser *parser,
			  GFile *file,
			  GFile *base_file,
			  IdolPlParseData *parse_data,
			  gpointer data)
{
#ifndef HAVE_GMIME
	WARN_NO_GMIME;
#else
	xml_node_t* doc;
	char *contents, *uri;
	gsize size;

	if (g_file_load_contents (file, NULL, &contents, &size, NULL, NULL) == FALSE)
		return IDOL_PL_PARSER_RESULT_ERROR;

	doc = idol_pl_parser_parse_xml_relaxed (contents, size);
	if (doc == NULL) {
		g_free (contents);
		return IDOL_PL_PARSER_RESULT_ERROR;
	}

	/* If the document has no name */
	if (doc->name == NULL
	    || g_ascii_strcasecmp (doc->name , "feed") != 0) {
		g_free (contents);
		xml_parser_free_tree (doc);
		return IDOL_PL_PARSER_RESULT_ERROR;
	}

	uri = g_file_get_uri (file);
	parse_atom_entries (parser, uri, doc);
	g_free (uri);

	g_free (contents);
	xml_parser_free_tree (doc);

	return IDOL_PL_PARSER_RESULT_SUCCESS;
#endif /* !HAVE_GMIME */
}

IdolPlParserResult
idol_pl_parser_add_xml_feed (IdolPlParser *parser,
			      GFile *file,
			      GFile *base_file,
			      IdolPlParseData *parse_data,
			      gpointer data)
{
#ifndef HAVE_GMIME
	WARN_NO_GMIME;
#else
	guint len;

	if (data == NULL)
		return IDOL_PL_PARSER_RESULT_UNHANDLED;

	len = strlen (data);

	if (idol_pl_parser_is_rss (data, len) != FALSE)
		return idol_pl_parser_add_rss (parser, file, base_file, parse_data, data);
	if (idol_pl_parser_is_atom (data, len) != FALSE)
		return idol_pl_parser_add_atom (parser, file, base_file, parse_data, data);
	if (idol_pl_parser_is_opml (data, len) != FALSE)
		return idol_pl_parser_add_opml (parser, file, base_file, parse_data, data);

	return IDOL_PL_PARSER_RESULT_UNHANDLED;
#endif /* !HAVE_GMIME */
}

static GByteArray *
idol_pl_parser_load_http_itunes (const char *uri)
{
	SoupMessage *msg;
	SoupSession *session;
	GByteArray *data;

	session = soup_session_sync_new_with_options (
	    SOUP_SESSION_ADD_FEATURE_BY_TYPE, SOUP_TYPE_GNOME_FEATURES_2_26,
	    SOUP_SESSION_ADD_FEATURE_BY_TYPE, SOUP_TYPE_CONTENT_DECODER,
	    SOUP_SESSION_USER_AGENT, "iTunes/7.4.1",
	    SOUP_SESSION_ACCEPT_LANGUAGE_AUTO, TRUE,
	    NULL);

	msg = soup_message_new (SOUP_METHOD_GET, uri);
	soup_session_send_message (session, msg);
	if (SOUP_STATUS_IS_SUCCESSFUL (msg->status_code)) {
		data = g_byte_array_new ();
		g_byte_array_append (data,
				     (guchar *) msg->response_body->data,
				     msg->response_body->length);
	} else {
		return NULL;
	}
	g_object_unref (msg);
	g_object_unref (session);

	return data;
}

static const char *
idol_pl_parser_parse_itms_link_doc (xml_node_t *item)
{
	for (item = item->child; item != NULL; item = item->next) {
		/* What we're looking for looks like:
		 * <key>url</key><string>URL</string> */
		if (g_ascii_strcasecmp (item->name, "key") == 0
		    && g_ascii_strcasecmp (item->data, "url") == 0
		    && item->next != NULL) {
			item = item->next;
			if (g_ascii_strcasecmp (item->name, "string") == 0)
				return item->data;
		} else {
			const char *ret;

			ret = idol_pl_parser_parse_itms_link_doc (item);
			if (ret != NULL)
				return ret;
		}
	}

	return NULL;
}

static const char *
idol_pl_parser_parse_itms_doc (xml_node_t *item)
{
	for (item = item->child; item != NULL; item = item->next) {
		/* What we're looking for looks like:
		 * <key>feedURL</key><string>URL</string> */
		if (g_ascii_strcasecmp (item->name, "key") == 0
		    && g_ascii_strcasecmp (item->data, "feedURL") == 0
		    && item->next != NULL) {
			item = item->next;
			if (g_ascii_strcasecmp (item->name, "string") == 0)
				return item->data;
		} else {
			const char *ret;

			ret = idol_pl_parser_parse_itms_doc (item);
			if (ret != NULL)
				return ret;
		}
	}

	return NULL;
}

static GFile *
idol_pl_parser_get_feed_uri (char *data, gsize len)
{
	xml_node_t* doc;
	const char *uri;
	GFile *ret;

	uri = NULL;

	doc = idol_pl_parser_parse_xml_relaxed (data, len);
	if (doc == NULL)
		return NULL;

	/* If the document has no name */
	if (doc->name == NULL
	    || g_ascii_strcasecmp (doc->name , "Document") != 0) {
		xml_parser_free_tree (doc);
		return NULL;
	}

	uri = idol_pl_parser_parse_itms_doc (doc);
	if (uri == NULL) {
		/* Maybe it's just a link instead */
		const char *link;
		GByteArray *content;
		GFile *feed_file;

		link = idol_pl_parser_parse_itms_link_doc (doc);
		if (link == NULL) {
			xml_parser_free_tree (doc);
			return NULL;
		}

		content = idol_pl_parser_load_http_itunes (link);
		if (content == NULL) {
			xml_parser_free_tree (doc);
			return NULL;
		}
		xml_parser_free_tree (doc);

		feed_file = idol_pl_parser_get_feed_uri ((char *) content->data, content->len);
		g_byte_array_free (content, TRUE);

		return feed_file;
	}

	ret = g_file_new_for_uri (uri);
	xml_parser_free_tree (doc);

	return ret;
}

IdolPlParserResult
idol_pl_parser_add_itms (IdolPlParser *parser,
			  GFile *file,
			  GFile *base_file,
			  IdolPlParseData *parse_data,
			  gpointer data)
{
#ifndef HAVE_GMIME
	WARN_NO_GMIME;
#else
	GByteArray *content;
	char *itms_uri;
	GFile *feed_file;
	IdolPlParserResult ret;

	if (g_file_has_uri_scheme (file, "itms") != FALSE) {
		itms_uri= g_file_get_uri (file);
		memcpy (itms_uri, "http", 4);
	} else if (g_file_has_uri_scheme (file, "http") != FALSE) {
		itms_uri = g_file_get_uri (file);
	} else {
		return IDOL_PL_PARSER_RESULT_ERROR;
	}

	/* Load the file using iTunes user-agent */
	content = idol_pl_parser_load_http_itunes (itms_uri);

	/* And look in the file for the feedURL */
	feed_file = idol_pl_parser_get_feed_uri ((char *) content->data, content->len);
	g_byte_array_free (content, TRUE);
	if (feed_file == NULL)
		return IDOL_PL_PARSER_RESULT_ERROR;

	DEBUG(feed_file, g_print ("Found feed URI: %s\n", uri));

	ret = idol_pl_parser_add_rss (parser, feed_file, NULL, parse_data, NULL);
	g_object_unref (feed_file);

	return ret;
#endif /* !HAVE_GMIME */
}

gboolean
idol_pl_parser_is_itms_feed (GFile *file)
{
	char *uri;

	g_return_val_if_fail (file != NULL, FALSE);

	uri = g_file_get_uri (file);

	if (g_file_has_uri_scheme (file, "itms") != FALSE ||
	    (g_file_has_uri_scheme (file, "http") != FALSE &&
	     strstr (uri, ".apple.com/") != FALSE)) {
		if (strstr (uri, "/podcast/") != NULL ||
		    strstr (uri, "viewPodcast") != NULL) {
			g_free (uri);
			return TRUE;
		}
	}

	g_free (uri);

	return FALSE;
}

static IdolPlParserResult
parse_opml_outline (IdolPlParser *parser, xml_node_t *parent)
{
	xml_node_t* node;

	for (node = parent->child; node != NULL; node = node->next) {
		const char *title, *uri;

		if (node->name == NULL || g_ascii_strcasecmp (node->name, "outline") != 0)
			continue;

		uri = xml_parser_get_property (node, "xmlUrl");
		title = xml_parser_get_property (node, "text");

		if (uri == NULL)
			continue;

		idol_pl_parser_add_uri (parser,
					 IDOL_PL_PARSER_FIELD_TITLE, title,
					 IDOL_PL_PARSER_FIELD_URI, uri,
					 NULL);
	}

	return IDOL_PL_PARSER_RESULT_SUCCESS;
}

static IdolPlParserResult
parse_opml_head_body (IdolPlParser *parser, const char *uri, xml_node_t *parent)
{
	xml_node_t* node;
	gboolean started;

	started = FALSE;

	for (node = parent->child; node != NULL; node = node->next) {
		if (node->name == NULL)
			continue;

		if (g_ascii_strcasecmp (node->name, "body") == 0) {
			if (started == FALSE) {
				/* Send the info we already have about the feed */
				idol_pl_parser_add_uri (parser,
							 IDOL_PL_PARSER_FIELD_IS_PLAYLIST, TRUE,
							 IDOL_PL_PARSER_FIELD_URI, uri,
							 NULL);
				started = TRUE;
			}

			parse_opml_outline (parser, node);
		}
	}

	return IDOL_PL_PARSER_RESULT_SUCCESS;
}

IdolPlParserResult
idol_pl_parser_add_opml (IdolPlParser *parser,
			  GFile *file,
			  GFile *base_file,
			  IdolPlParseData *parse_data,
			  gpointer data)
{
#ifndef HAVE_GMIME
	WARN_NO_GMIME;
#else
	xml_node_t* doc;
	char *contents, *uri;
	gsize size;

	if (g_file_load_contents (file, NULL, &contents, &size, NULL, NULL) == FALSE)
		return IDOL_PL_PARSER_RESULT_ERROR;

	doc = idol_pl_parser_parse_xml_relaxed (contents, size);
	if (doc == NULL) {
		g_free (contents);
		return IDOL_PL_PARSER_RESULT_ERROR;
	}

	/* If the document has no name */
	if (doc->name == NULL
	    || g_ascii_strcasecmp (doc->name , "opml") != 0) {
		g_free (contents);
		xml_parser_free_tree (doc);
		return IDOL_PL_PARSER_RESULT_ERROR;
	}

	uri = g_file_get_uri (file);
	parse_opml_head_body (parser, uri, doc);
	g_free (uri);

	g_free (contents);
	xml_parser_free_tree (doc);

	return IDOL_PL_PARSER_RESULT_SUCCESS;
#endif /* !HAVE_GMIME */
}

#endif /* !IDOL_PL_PARSER_MINI */
