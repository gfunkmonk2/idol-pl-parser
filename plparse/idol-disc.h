/* Idol Disc Content Detection
 * (c) 2004 Ronald Bultje <rbultje@ronald.bitfreak.net>
 *
 * idol-disc.h: media content detection
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301  USA.
 */

#ifndef IDOL_DISC_H
#define IDOL_DISC_H

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

/**
 * IdolDiscMediaType:
 * @MEDIA_TYPE_ERROR: there was an error determining the media's type
 * @MEDIA_TYPE_DATA: data disc
 * @MEDIA_TYPE_CDDA: audio CD
 * @MEDIA_TYPE_VCD: video CD
 * @MEDIA_TYPE_DVD: video DVD
 * @MEDIA_TYPE_DVB: digital television
 *
 * Gives the media type of a disc, or %MEDIA_TYPE_ERROR if the media type
 * could not be determined.
 **/
typedef enum {
  MEDIA_TYPE_ERROR = -1,
  MEDIA_TYPE_DATA = 1,
  MEDIA_TYPE_CDDA,
  MEDIA_TYPE_VCD,
  MEDIA_TYPE_DVD,
  MEDIA_TYPE_DVB
} IdolDiscMediaType;
#define MEDIA_TYPE_NUM_TYPES MEDIA_TYPE_DVB + 1

#define MediaType IdolDiscMediaType

GQuark idol_disc_media_type_quark	(void) G_GNUC_CONST;
#define IDOL_DISC_MEDIA_TYPE		idol_disc_media_type_quark ()

IdolDiscMediaType	idol_cd_detect_type	(const char *device,
						 GError     **error);
IdolDiscMediaType	idol_cd_detect_type_with_url (const char  *device,
						       char       **mrl,
						       GError     **error);
IdolDiscMediaType	idol_cd_detect_type_from_dir (const char *dir,
						       char      **mrl, 
						       GError    **error);
const char *	idol_cd_get_human_readable_name (IdolDiscMediaType type);
char *		idol_cd_mrl_from_type (const char *scheme, const char *dir);
gboolean	idol_cd_has_medium (const char  *device);

G_END_DECLS

#endif /* IDOL_DISC_H */
