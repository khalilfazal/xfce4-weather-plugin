/*  $Id$
 *
 *  Copyright (c) 2003-2007 Xfce Development Team
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

#include <libxfcegui4/libxfcegui4.h>

#include "weather-parsers.h"
#include "weather-data.h"
#include "weather.h"

#include "weather-search.h"
#include "weather-http.h"

#define BORDER 8



static void
append_result (GtkListStore *mdl,
               gchar        *id,
               gchar        *city)
{
  GtkTreeIter iter;

  gtk_list_store_append (mdl, &iter);
  gtk_list_store_set (mdl, &iter, 0, city, 1, id, -1);
}



static gchar *
sanitize_str (const gchar *str)
{
  GString *retstr = g_string_sized_new (strlen (str));
  gchar   *realstr, c = '\0';

  while ((c = *str++))
    {
      if (g_ascii_isspace (c))
        g_string_append (retstr, "%20");
      else if (g_ascii_isalnum (c) == FALSE)
        {
          g_string_free (retstr, TRUE);
          return NULL;
        }
      else
        g_string_append_c (retstr, c);
    }

  realstr = retstr->str;

  g_string_free (retstr, FALSE);

  return realstr;
}



static void
cb_searchdone (gboolean result,
               gpointer user_data)
{
  search_dialog *dialog = (search_dialog *) user_data;
  xmlDoc        *doc;
  xmlNode       *cur_node;
  gchar         *id, *city;

  if (!result || dialog->recv_buffer == NULL)
    return;

  doc = xmlParseMemory (dialog->recv_buffer, strlen (dialog->recv_buffer));
  g_free (dialog->recv_buffer);
  dialog->recv_buffer = NULL;

  if (!doc)
    return;

  cur_node = xmlDocGetRootElement (doc);

  if (cur_node)
    {
      for (cur_node = cur_node->children; cur_node; cur_node = cur_node->next)
        {
          if (NODE_IS_TYPE (cur_node, "loc"))
            {
              id = (gchar *) xmlGetProp (cur_node, (const xmlChar *) "id");

              if (!id)
                continue;

              city = DATA (cur_node);

              if (!city)
                {
                  g_free (id);
                  continue;
                }

              append_result (dialog->result_mdl, id, city);
              g_free (id);
              g_free (city);
            }
        }
    }

  xmlFreeDoc (doc);

  return;
}



static gboolean
search_cb (GtkWidget *widget,
           gpointer   user_data)
{
  search_dialog *dialog = (search_dialog *) user_data;
  gchar         *sane_str, *url;
  const gchar   *str;
  gboolean       result;

  str = gtk_entry_get_text (GTK_ENTRY (dialog->search_entry));

  if (strlen (str) == 0)
    return FALSE;

  gtk_list_store_clear (GTK_LIST_STORE (dialog->result_mdl));

  if ((sane_str = sanitize_str (str)) == NULL)
    return FALSE;

  url = g_strdup_printf ("/search/search?where=%s", sane_str);
  g_free (sane_str);

  result =
    http_get_buffer (url, "xoap.weather.com", dialog->proxy_host,
                     dialog->proxy_port, &dialog->recv_buffer, cb_searchdone,
                     (gpointer) dialog);
  g_free (url);

  return result;
}



static void
pass_search_results (GtkTreeView       *tree_view,
                     GtkTreePath       *path,
                     GtkTreeViewColumn *column,
                     GtkDialog         *dialog)
{
  gtk_dialog_response (dialog, GTK_RESPONSE_ACCEPT);
}



search_dialog *
create_search_dialog (GtkWindow *parent,
                      gchar     *proxy_host,
                      gint       proxy_port)
{
  GtkWidget         *vbox, *button, *hbox, *scroll, *frame;
  GtkTreeViewColumn *column;
  GtkCellRenderer   *renderer = gtk_cell_renderer_text_new ();
  search_dialog     *dialog;

  dialog = panel_slice_new0 (search_dialog);

  dialog->proxy_host = proxy_host;
  dialog->proxy_port = proxy_port;

  if (!dialog)
    return NULL;

  dialog->dialog =
    xfce_titled_dialog_new_with_buttons (_("Search weather location code"), parent,
						GTK_DIALOG_MODAL |
						GTK_DIALOG_DESTROY_WITH_PARENT, 
						GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
						GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, NULL);

  gtk_window_set_icon_name (GTK_WINDOW (dialog->dialog), GTK_STOCK_FIND);

  vbox = gtk_vbox_new (FALSE, BORDER);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 6);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog->dialog)->vbox), vbox,
                      TRUE, TRUE, 0);

  xfce_titled_dialog_set_subtitle (XFCE_TITLED_DIALOG (dialog->dialog), 
				   _("Enter a city name or zip code"));

  hbox = gtk_hbox_new (FALSE, BORDER);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

  dialog->search_entry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (hbox), dialog->search_entry, TRUE, TRUE, 0);
  g_signal_connect (G_OBJECT (dialog->search_entry), "activate",
                    G_CALLBACK (search_cb), dialog);

  button = gtk_button_new_from_stock (GTK_STOCK_FIND);
  gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 0);
  g_signal_connect (G_OBJECT (button), "clicked",
                    G_CALLBACK (search_cb), dialog);

  frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
  gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 0);
  
  scroll = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add (GTK_CONTAINER (frame), scroll);

  dialog->result_mdl = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_STRING);
  dialog->result_list = gtk_tree_view_new_with_model (GTK_TREE_MODEL (dialog->result_mdl));
  column = gtk_tree_view_column_new_with_attributes (_("Results"), renderer, "text", 0, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (dialog->result_list), column);
  g_signal_connect (G_OBJECT (dialog->result_list), "row-activated",
                    G_CALLBACK (pass_search_results), dialog->dialog);
  gtk_container_add (GTK_CONTAINER (scroll), dialog->result_list);

  gtk_widget_set_size_request (dialog->dialog, 425, 250);

  return dialog;
}



gboolean
run_search_dialog (search_dialog *dialog)
{
  GtkTreeIter       iter;
  GtkTreeSelection *selection;
  GValue            value = { 0, };

  gtk_widget_show_all (dialog->dialog);

  if (gtk_dialog_run (GTK_DIALOG (dialog->dialog)) == GTK_RESPONSE_ACCEPT)
    {
      selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (dialog->result_list));

      if (gtk_tree_selection_get_selected (selection, NULL, &iter))
        {
          gtk_tree_model_get_value (GTK_TREE_MODEL (dialog->result_mdl),
                                    &iter, 1, &value);
          dialog->result = g_strdup (g_value_get_string (&value));

          g_value_unset (&value);
          
          return TRUE;
        }
    }

  return FALSE;
}



void
free_search_dialog (search_dialog * dialog)
{
  g_free (dialog->result);

  gtk_widget_destroy (dialog->dialog);

  panel_slice_free (search_dialog, dialog);
}