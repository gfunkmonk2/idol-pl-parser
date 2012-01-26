/*
   Copyright (C) 2009, Nokia

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

   Author: Carlos Garnacho <carlos@lanedo.com>
 */

#ifndef __IDOL_PL_PLAYLIST_H__
#define __IDOL_PL_PLAYLIST_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define IDOL_TYPE_PL_PLAYLIST            (idol_pl_playlist_get_type ())
#define IDOL_PL_PLAYLIST(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), IDOL_TYPE_PL_PLAYLIST, IdolPlPlaylist))
#define IDOL_PL_PLAYLIST_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), IDOL_TYPE_PL_PLAYLIST, IdolPlPlaylistClass))
#define IDOL_IS_PL_PLAYLIST(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IDOL_TYPE_PL_PLAYLIST))
#define IDOL_IS_PL_PLAYLIST_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), IDOL_TYPE_PL_PLAYLIST))

typedef struct IdolPlPlaylist IdolPlPlaylist;
typedef struct IdolPlPlaylistClass IdolPlPlaylistClass;
typedef struct IdolPlPlaylistIter IdolPlPlaylistIter;

/**
 * IdolPlPlaylist:
 *
 * All the fields in the #IdolPlPlaylist structure are private and should never be accessed directly.
 **/
struct IdolPlPlaylist {
        GObject parent_instance;
};

/**
 * IdolPlPlaylistClass:
 *
 * All the fields in the #IdolPlPlaylistClass structure are private and should never be accessed directly.
 **/
struct IdolPlPlaylistClass {
        GObjectClass parent_class;
};

/**
 * IdolPlPlaylistIter:
 *
 * All the fields in the #IdolPlPlaylistIter structure are private and should never be accessed directly.
 **/
struct IdolPlPlaylistIter {
        gpointer data1;
        gpointer data2;
};

GType idol_pl_playlist_get_type (void) G_GNUC_CONST;

IdolPlPlaylist * idol_pl_playlist_new (void);

guint idol_pl_playlist_size   (IdolPlPlaylist     *playlist);

/* Item insertion methods */
void idol_pl_playlist_prepend (IdolPlPlaylist     *playlist,
                                IdolPlPlaylistIter *iter);
void idol_pl_playlist_append  (IdolPlPlaylist     *playlist,
                                IdolPlPlaylistIter *iter);
void idol_pl_playlist_insert  (IdolPlPlaylist     *playlist,
                                gint                 position,
                                IdolPlPlaylistIter *iter);

/* Navigation methods */
gboolean idol_pl_playlist_iter_first (IdolPlPlaylist     *playlist,
                                       IdolPlPlaylistIter *iter);
gboolean idol_pl_playlist_iter_next  (IdolPlPlaylist     *playlist,
                                       IdolPlPlaylistIter *iter);
gboolean idol_pl_playlist_iter_prev  (IdolPlPlaylist     *playlist,
                                       IdolPlPlaylistIter *iter);

/* Item edition methods */
gboolean idol_pl_playlist_get_value (IdolPlPlaylist     *playlist,
                                      IdolPlPlaylistIter *iter,
                                      const gchar         *key,
                                      GValue              *value);
void idol_pl_playlist_get_valist    (IdolPlPlaylist     *playlist,
                                      IdolPlPlaylistIter *iter,
                                      va_list              args);
void idol_pl_playlist_get           (IdolPlPlaylist     *playlist,
                                      IdolPlPlaylistIter *iter,
                                      ...) G_GNUC_NULL_TERMINATED;

gboolean idol_pl_playlist_set_value (IdolPlPlaylist     *playlist,
                                      IdolPlPlaylistIter *iter,
                                      const gchar         *key,
                                      GValue              *value);
void idol_pl_playlist_set_valist    (IdolPlPlaylist     *playlist,
                                      IdolPlPlaylistIter *iter,
                                      va_list              args);
void idol_pl_playlist_set           (IdolPlPlaylist     *playlist,
                                      IdolPlPlaylistIter *iter,
                                      ...) G_GNUC_NULL_TERMINATED;

G_END_DECLS

#endif /* __IDOL_PL_PLAYLIST_H__ */
