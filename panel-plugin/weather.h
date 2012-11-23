/*  Copyright (c) 2003-2012 Xfce Development Team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __WEATHER_H__
#define __WEATHER_H__

#include <libxfce4panel/libxfce4panel.h>
#include <libxfce4util/libxfce4util.h>
#include <libsoup/soup.h>
#include "weather-icon.h"

#define PLUGIN_WEBSITE "http://goodies.xfce.org/projects/panel-plugins/xfce4-weather-plugin"
#define MAX_FORECAST_DAYS 10
#define DEFAULT_FORECAST_DAYS 5

G_BEGIN_DECLS

typedef struct {
    XfcePanelPlugin *plugin;

    SoupSession *session;

    GtkWidget *top_vbox;
    GtkWidget *top_hbox;
    GtkWidget *vbox_center_scrollbox;
    GtkWidget *scrollbox;
    GtkWidget *iconimage;
    GtkWidget *tooltipbox;

    GtkWidget *summary_window;

    GArray *labels;

    gint panel_size;
    gint size;
    GtkOrientation orientation;
    GtkOrientation panel_orientation;
    gint updatetimeout;
    time_t last_astro_update;
    time_t last_data_update;
    time_t last_conditions_update;

    icon_theme *icon_theme;

    gchar *lat;
    gchar *lon;
    gchar *location_name;

    unit_systems unit_system;
    units_config *units;

    xml_weather *weatherdata;
    xml_astro *astrodata;
    gboolean night_time;

    gboolean animation_transitions;
    gint forecast_days;
} xfceweather_data;


extern gboolean debug_mode;

typedef void (*SoupSessionCallback) (SoupSession *session,
                                     SoupMessage *msg,
                                     gpointer user_data);

void weather_http_queue_request(SoupSession *session,
                                const gchar *uri,
                                SoupSessionCallback callback_func,
                                gpointer user_data);

G_END_DECLS

#endif
