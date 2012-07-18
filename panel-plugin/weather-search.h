/*  Copyright (c) 2003-2007 Xfce Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <glib.h>
#include <gmodule.h>
#include <gtk/gtk.h>

#ifndef __WEATHER_SEARCH_H__
#define __WEATHER_SEARCH_H__

G_BEGIN_DECLS

typedef struct
{
  GtkWidget    *dialog;
  GtkWidget    *search_entry;
  GtkWidget    *result_list;
  GtkWidget    *find_button;
  GtkListStore *result_mdl;
  GtkTreeViewColumn *column;

  gchar        *result_lat;
  gchar        *result_lon;
  gchar        *result_name;

  gchar        *proxy_host;
  gint          proxy_port;
  
  gchar        *last_search;
}
search_dialog;

search_dialog *create_search_dialog (GtkWindow *, gchar *, gint);

gboolean run_search_dialog (search_dialog * dialog);

void weather_search_by_ip(const gchar *proxy_host, gint proxy_port,
    void (*gui_cb)(const gchar *loc_name, const gchar *lat, const gchar *lon,
                   const unit_systems unit_system, gpointer user_data),
    gpointer user_data);

void free_search_dialog (search_dialog * dialog);

G_END_DECLS

#endif
