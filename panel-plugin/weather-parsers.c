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

#include "weather-parsers.h"
#include <libxfce4panel/xfce-panel-macros.h>



xml_weather *
parse_weather (xmlNode *cur_node)
{
  xml_weather *ret;
  xmlNode     *child_node;
  guint        i = 0;

  if (!NODE_IS_TYPE (cur_node, "weather"))
    {
      return NULL;
    }

  if ((ret = panel_slice_new0 (xml_weather)) == NULL)
    return NULL;

  for (cur_node = cur_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type != XML_ELEMENT_NODE)
        continue;

      if (NODE_IS_TYPE (cur_node, "cc"))
        ret->cc = parse_cc (cur_node);
      else if (NODE_IS_TYPE (cur_node, "loc"))
        ret->loc = parse_loc (cur_node);
      else if (NODE_IS_TYPE (cur_node, "lnks"))
        ret->lnk = parse_lnk (cur_node);
      else if (NODE_IS_TYPE (cur_node, "dayf"))
        {
          for (child_node = cur_node->children; child_node;
               child_node = child_node->next)
            {
              if (NODE_IS_TYPE (child_node, "day"))
                {
                  if (i >= XML_WEATHER_DAYF_N)
                    break;

                  ret->dayf[i] = parse_dayf (child_node);

                  i++;
                }
            }
        }
    }

  return ret;
}



xml_loc *
parse_loc (xmlNode *cur_node)
{
  xml_loc *ret;

  if ((ret = panel_slice_new0 (xml_loc)) == NULL)
    return NULL;


  for (cur_node = cur_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type != XML_ELEMENT_NODE)
        continue;

      if (NODE_IS_TYPE (cur_node, "dnam"))
        ret->dnam = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "sunr"))
        ret->sunr = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "suns"))
        ret->suns = DATA (cur_node);
    }

  return ret;
}

xml_lnk *
parse_lnk (xmlNode *cur_node)
{
  xml_lnk *ret;
  int i = 0;
  if ((ret = panel_slice_new0 (xml_lnk)) == NULL)
    return NULL;


  for (cur_node = cur_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type != XML_ELEMENT_NODE)
        continue;

      if (NODE_IS_TYPE (cur_node, "link")) {
        xmlNode *l_node;
        if (i < 4) {
	  for (l_node = cur_node->children; l_node; l_node = l_node->next) {
              if (NODE_IS_TYPE (l_node, "l"))
		ret->lnk[i] = DATA(l_node);
	      else if (NODE_IS_TYPE (l_node, "t"))
		ret->lnk_txt[i] = DATA(l_node);
	  }
	}
	i++;
      }
    }

  return ret;
}



static xml_uv *
parse_uv (xmlNode *cur_node)
{
  xml_uv *ret;

  if ((ret = panel_slice_new0 (xml_uv)) == NULL)
    return NULL;

  for (cur_node = cur_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type != XML_ELEMENT_NODE)
        continue;

      if (NODE_IS_TYPE (cur_node, "i"))
        ret->i = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "t"))
        ret->t = DATA (cur_node);
    }

  return ret;
}



static xml_bar *
parse_bar (xmlNode *cur_node)
{
  xml_bar *ret;

  if ((ret = panel_slice_new0 (xml_bar)) == NULL)
    return NULL;

  for (cur_node = cur_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type != XML_ELEMENT_NODE)
        continue;

      if (NODE_IS_TYPE (cur_node, "r"))
        ret->r = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "d"))
        ret->d = DATA (cur_node);
    }

  return ret;
}



static xml_wind *
parse_wind (xmlNode *cur_node)
{
  xml_wind *ret;

  if ((ret = panel_slice_new0 (xml_wind)) == NULL)
    return NULL;

  for (cur_node = cur_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type != XML_ELEMENT_NODE)
        continue;

      if (NODE_IS_TYPE (cur_node, "s"))
        ret->s = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "gust"))
        ret->gust = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "d"))
        ret->d = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "t"))
        ret->t = DATA (cur_node);
    }

  return ret;
}



xml_cc *
parse_cc (xmlNode *cur_node)
{
  xml_cc *ret;

  if ((ret = panel_slice_new0 (xml_cc)) == NULL)
    return NULL;

  for (cur_node = cur_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type != XML_ELEMENT_NODE)
        continue;

      if (NODE_IS_TYPE (cur_node, "tmp"))
        ret->tmp = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "icon"))
        ret->icon = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "t"))
        ret->t = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "flik"))
        ret->flik = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "bar"))
        ret->bar = parse_bar (cur_node);
      else if (NODE_IS_TYPE (cur_node, "wind"))
        ret->wind = parse_wind (cur_node);
      else if (NODE_IS_TYPE (cur_node, "hmid"))
        ret->hmid = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "vis"))
        ret->vis = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "uv"))
        ret->uv = parse_uv (cur_node);
      else if (NODE_IS_TYPE (cur_node, "dewp"))
        ret->dewp = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "lsup"))
        ret->lsup = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "obst"))
        ret->obst = DATA (cur_node);
    }

  return ret;
}



static xml_part *
parse_part (xmlNode *cur_node)
{
  xml_part *ret;

  if ((ret = panel_slice_new0 (xml_part)) == NULL)
    return NULL;

  for (cur_node = cur_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type != XML_ELEMENT_NODE)
        continue;

      if (NODE_IS_TYPE (cur_node, "icon"))
        ret->icon = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "t"))
        ret->t = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "wind"))
        ret->wind = parse_wind (cur_node);
      else if (NODE_IS_TYPE (cur_node, "ppcp"))
        ret->ppcp = DATA (cur_node);
      else if (NODE_IS_TYPE (cur_node, "hmid"))
        ret->hmid = DATA (cur_node);
    }

  return ret;
}



xml_dayf *
parse_dayf (xmlNode *cur_node)
{
  xml_dayf *ret;
  gchar    *value;

  if ((ret = panel_slice_new0 (xml_dayf)) == NULL)
    return NULL;

  ret->day = (gchar *) xmlGetProp (cur_node, (const xmlChar *) "t");
  ret->date = (gchar *) xmlGetProp (cur_node, (const xmlChar *) "dt");

  for (cur_node = cur_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type != XML_ELEMENT_NODE)
        continue;

      if (NODE_IS_TYPE (cur_node, "hi"))
        {
          ret->hi = DATA (cur_node);
          g_assert (ret->hi != NULL);
        }
      else if (NODE_IS_TYPE (cur_node, "low"))
        {
          ret->low = DATA (cur_node);
        }
      else if (NODE_IS_TYPE (cur_node, "part"))
        {
          value = (gchar *) xmlGetProp (cur_node, (const xmlChar *) "p");

          if (xmlStrEqual ((const xmlChar *) value, (const xmlChar *) "d"))
            ret->part[0] = parse_part (cur_node);
          else
            if (xmlStrEqual ((const xmlChar *) value, (const xmlChar *) "n"))
            ret->part[1] = parse_part (cur_node);

          g_free (value);
        }
    }

  return ret;
}



static void
xml_uv_free (xml_uv * data)
{
  g_free (data->i);
  g_free (data->t);

  panel_slice_free (xml_uv, data);
}



static void
xml_wind_free (xml_wind * data)
{
  g_free (data->s);
  g_free (data->gust);
  g_free (data->d);
  g_free (data->t);

  panel_slice_free (xml_wind, data);
}



static void
xml_bar_free (xml_bar * data)
{
  g_free (data->r);
  g_free (data->d);

  panel_slice_free (xml_bar, data);
}



static void
xml_cc_free (xml_cc * data)
{
  g_free (data->obst);
  g_free (data->lsup);
  g_free (data->flik);
  g_free (data->t);
  g_free (data->icon);
  g_free (data->tmp);
  g_free (data->hmid);
  g_free (data->vis);
  g_free (data->dewp);

  if (data->uv)
    xml_uv_free (data->uv);

  if (data->wind)
    xml_wind_free (data->wind);

  if (data->bar)
    xml_bar_free (data->bar);

  panel_slice_free (xml_cc, data);
}



static void
xml_loc_free (xml_loc *data)
{
  g_free (data->dnam);
  g_free (data->sunr);
  g_free (data->suns);

  panel_slice_free (xml_loc, data);
}

static void
xml_lnk_free (xml_lnk *data)
{
  int i;
  for (i = 0; i < 4; i++) {
    g_free (data->lnk[i]);
    g_free (data->lnk_txt[i]);
  }
  panel_slice_free (xml_lnk, data);
}

static void
xml_part_free (xml_part *data)
{
  if (!data)
    return;

  g_free (data->icon);
  g_free (data->t);
  g_free (data->ppcp);
  g_free (data->hmid);

  if (data->wind)
    xml_wind_free (data->wind);

  panel_slice_free (xml_part, data);
}



static void
xml_dayf_free (xml_dayf *data)
{
  if (!data)
    return;

  g_free (data->day);
  g_free (data->date);
  g_free (data->hi);
  g_free (data->low);

  if (data->part[0])
    xml_part_free (data->part[0]);

  if (data->part[1])
    xml_part_free (data->part[1]);

  panel_slice_free (xml_dayf, data);
}



void
xml_weather_free (xml_weather *data)
{
  guint i;

  if (data->cc)
    xml_cc_free (data->cc);

  if (data->loc)
    xml_loc_free (data->loc);

  if (data->lnk)
    xml_lnk_free (data->lnk);

  if (data->dayf)
    {
      for (i = 0; i < XML_WEATHER_DAYF_N; i++)
        {
          if (!data->dayf[i])
            break;

          xml_dayf_free (data->dayf[i]);
        }
    }

  panel_slice_free (xml_weather, data);
}