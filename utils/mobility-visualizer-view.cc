/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <map>

#include <goocanvas.h>
#include <glib/gthread.h>

#include "mobility-visualizer.h"
#include <map>
#include "ns3/simulator.h"

#define MAX_QUEUE_LENGTH 100
#define MAX_EVENTS 20
#define LOOKAHEAD_SECONDS 10

GtkWidget *g_canvas;

int model_init (int argc, char *argv[]);

struct Node
{
  GooCanvasItem *m_item;
  GooCanvasItem *m_vector;
  void create ()
  {
    GooCanvasItem *root = goo_canvas_get_root_item (GOO_CANVAS (g_canvas));
    m_item = goo_canvas_ellipse_new (root, 0, 0, 2.0, 2.0,
                                     "line_width", 0.5,
                                     "stroke_color", "black",
                                     "fill_color", "red",
                                     NULL);

    m_vector = goo_canvas_polyline_new (root, FALSE, 0,
                                        "line_width", 0.3,
                                        "stroke_color", "black",
                                        "end-arrow", TRUE,
                                        "arrow-length", 10.0,
                                        "arrow-width", 10.0,
                                        NULL);
    
  }


  void update (double x, double y, double vx, double vy)
  {
    g_object_set (m_item, "center_x", x, "center_y", y, NULL);
    
    if (vx == 0 && vy == 0)
      {
        GooCanvasPoints *points = goo_canvas_points_new (0);
        g_object_set (m_vector, "points", points, NULL);
        goo_canvas_points_unref (points);
      }
    else
      {
        GooCanvasPoints *points = goo_canvas_points_new (2);

        points->coords[0] = x;
        points->coords[1] = y;
        points->coords[2] = x + vx;
        points->coords[3] = y + vy;

        g_object_set (m_vector, "points", points, NULL);
        goo_canvas_points_unref (points);
      }
  }
};

std::map<void*, Node> g_nodes;

GTimeVal initialTime = {-1, -1};
gboolean firstTime = TRUE;
double g_lookaheadTime = 0;
G_LOCK_DEFINE (g_lookaheadTimeMux);
ViewUpdateData *g_nextData = NULL;


GAsyncQueue *queue;

#define TIME_SCALE 1

double get_current_time ()
{
  GTimeVal currTime;
  g_get_current_time (&currTime);
  GTimeVal relativeTime;
  relativeTime.tv_sec = currTime.tv_sec - initialTime.tv_sec + LOOKAHEAD_SECONDS;
  relativeTime.tv_usec = currTime.tv_usec;
  g_time_val_add (&relativeTime, -initialTime.tv_usec);
  return (relativeTime.tv_sec + 1.0e-6*relativeTime.tv_usec)*TIME_SCALE;
}

// called from the simulation thread
void view_update (ViewUpdateData *updateData)
{
  while ((G_LOCK (g_lookaheadTimeMux), g_lookaheadTime) != 0 and updateData->time >= g_lookaheadTime)
    {
      G_UNLOCK (g_lookaheadTimeMux);
      g_usleep ((gulong) 10e3);
    }
  G_UNLOCK (g_lookaheadTimeMux);
  g_async_queue_push (queue, updateData);
}

void view_update_process (ViewUpdateData *updateData)
{
  if (firstTime)
    {
      firstTime = FALSE;
      g_get_current_time (&initialTime);

      for (std::vector<NodeUpdate>::const_iterator update
             = updateData->updateList.begin ();
           update != updateData->updateList.end ();
           update++)
        {
          g_nodes[update->node].create ();
          g_nodes[update->node].update (update->x, update->y, update->vx, update->vy);
        }
      delete updateData;
    }
  else
    {
      for (std::vector<NodeUpdate>::const_iterator update
             = updateData->updateList.begin ();
           update != updateData->updateList.end ();
           update++)
        {
          g_nodes[update->node].update (update->x, update->y, update->vx, update->vy);
        }
      delete updateData;

      G_LOCK (g_lookaheadTimeMux);
      g_lookaheadTime = get_current_time () + LOOKAHEAD_SECONDS;
      G_UNLOCK (g_lookaheadTimeMux);
    }
}

gboolean view_update_consumer ()
{
  if (!g_nextData)
    g_nextData = (ViewUpdateData *) g_async_queue_try_pop (queue);

  if (!g_nextData)
    return TRUE;
  
  double time = get_current_time ();
  if (g_nextData->time > time)
    return TRUE;

  do
    {
      view_update_process (g_nextData);
      g_nextData = (ViewUpdateData *) g_async_queue_try_pop (queue);
    }
  while (g_nextData && g_nextData->time <= time);

  return TRUE;
}

void zoom_changed (GtkAdjustment *adj)
{
  goo_canvas_set_scale (GOO_CANVAS (g_canvas), gtk_adjustment_get_value (adj));
}

int main (int argc, char *argv[])
{
  g_thread_init (NULL);
  gtk_init (&argc, &argv);
  double x1 = 0, y1 = 0, x2 = 0, y2 = 0;

  model_init (argc, argv, &x1, &y1, &x2, &y2);

  GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 640, 600);
  gtk_widget_show (window);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  GtkWidget *scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_win), GTK_SHADOW_IN);
  gtk_widget_show (scrolled_win);

  GtkWidget *vbox = gtk_vbox_new (FALSE, 4);
  gtk_widget_show (vbox);
  gtk_box_pack_start (GTK_BOX (vbox), scrolled_win, 1, 1, 4);
  gtk_container_add (GTK_CONTAINER (window), vbox);

  GtkWidget *hbox = gtk_hbox_new (FALSE, 4);
  gtk_widget_show (hbox);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, false, false, 4);

  GtkObject *zoom = gtk_adjustment_new (3.0, 0.1, 10.0, 0.2, 1.0, 1.0);
  gtk_box_pack_start(GTK_BOX (hbox),
                     GTK_WIDGET (g_object_new (GTK_TYPE_SPIN_BUTTON, "adjustment", zoom,
                                               "visible", true, "digits", 2, NULL)),
                     false, false, 4);

  g_canvas = goo_canvas_new ();
  gtk_widget_set_size_request (GTK_WIDGET (g_canvas), 600, 450);
  goo_canvas_set_bounds (GOO_CANVAS (g_canvas), -500, -500, 500, 500);
  g_signal_connect (zoom, "value-changed", G_CALLBACK (zoom_changed), NULL);
  gtk_adjustment_value_changed (GTK_ADJUSTMENT (zoom));
  gtk_widget_show (g_canvas);
  gtk_container_add (GTK_CONTAINER (scrolled_win), g_canvas);

  goo_canvas_scroll_to (GOO_CANVAS (g_canvas), 0, 0);

  // create the bounds rectangle
  if (x1 != x2)
    {
      GooCanvasItem *item = 
        goo_canvas_rect_new (goo_canvas_get_root_item (GOO_CANVAS (g_canvas)),
                                                 x1, y1, x2-x1, y2-y1, NULL);
        g_object_set (item, "line-width", 1.0, "stroke-color", "grey", NULL);
    }

  queue = g_async_queue_new ();

  g_timeout_add ((guint) (SAMPLE_INTERVAL*1000), (GSourceFunc) view_update_consumer, NULL);

  g_thread_create (GThreadFunc (ns3::Simulator::Run), NULL, FALSE, NULL);

  gtk_main ();

  return 0;
}
