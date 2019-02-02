#include <stdio.h>
#include <string.h>
#include <gst/gst.h>

/* author Pander */

static GMainLoop *loop;

//TODO Not used yet, perhaps not needed at all when width and height are
// retrieved in another way.
/*static void
read_video_props (GstCaps *caps)
{
  gint width, height;
  const GstStructure *str;

  g_return_if_fail (gst_caps_is_fixed (caps));

  str = gst_caps_get_structure (caps, 0);
  if (!gst_structure_get_int (str, "width", &width) ||
      !gst_structure_get_int (str, "height", &height)) {
    g_print ("No width/height available\n");
    return;
  }

  g_print ("The video size of this set of capabilities is %dx%d\n",
	   width, height);
}*/

//TODO Callback which you are suppose to have. If really not needed, can be
// removed. Otherwise, should be an implementation which is as minimally as
// possible.
static gboolean
my_bus_callback (GstBus     *bus,
		 GstMessage *message,
		 gpointer    data)
{
//  g_print ("Got %s message\n", GST_MESSAGE_TYPE_NAME (message));

  switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_ERROR: {
      GError *err;
      gchar *debug;

      gst_message_parse_error (message, &err, &debug);
      g_print ("Error: %s\n", err->message);
      g_error_free (err);
      g_free (debug);

      g_main_loop_quit (loop);
      break;
    }
    case GST_MESSAGE_EOS:
      /* end-of-stream */
      g_main_loop_quit (loop);
      break;
    default:
      /* unhandled message */
      break;
  }

  /* we want to be notified again the next time there is a message
   * on the bus, so returning TRUE (FALSE means we want to stop watching
   * for messages on the bus and our callback should not be called again)
   */
  return TRUE;
}

gint
main (int   argc,
      char *argv[])
{
  /* process command-line arguments */
  gboolean autovideosink = FALSE;
  gboolean fullscreen = FALSE;
  gchar *device = NULL;
  gchar *resolution = NULL;
  GOptionContext *ctx;
  GError *err = NULL;
  GOptionEntry entries[] = {
    { "autovideosink", 'a', 0, G_OPTION_ARG_NONE, &autovideosink,
      "use auto video sink instead of GL Image sink", NULL },
    { "fullscreen", 'f', 0, G_OPTION_ARG_NONE, &fullscreen,
      "use sink in full screen mode, is overridden by --resolution", NULL },
    { "device", 'd', 0, G_OPTION_ARG_STRING, &device,
      "use specified device as input, e.g. /dev/video1", "FILE" },
    { "resolution", 'r', 0, G_OPTION_ARG_STRING, &resolution,
      "use specified resolution, e.g. 800x600 or 640x480", "WIDTHxHEIGHT" },
    { NULL }
  };
  //TODO Add option with argument for  
  ctx = g_option_context_new ("Device camera live player");
  g_option_context_add_main_entries (ctx, entries, NULL);
  g_option_context_add_group (ctx, gst_init_get_option_group ());
  if (!g_option_context_parse (ctx, &argc, &argv, &err)) {
    g_print ("Failed to initialize: %s\n", err->message);
    g_clear_error (&err);
    g_option_context_free (ctx);
    return 1;
  }

  /* initialize gst */
  gst_init(0, NULL);
  
  /* create sink element */
  GstElement *sink;
  sink = NULL;
  if (autovideosink) {
    /* use autovideosink option */
    sink = gst_element_factory_make ("autovideosink", "sink");
    if (!sink) {
      g_error ("Failed to create element of type 'autovideosink'\n");
      g_option_context_free (ctx);
      return -1;
    }
  } else {
    /* use glimagesink as default */
    sink = gst_element_factory_make ("glimagesink", "sink");
    if (!sink) {
      /* try autovideosink as fallback*/
      sink = gst_element_factory_make ("autovideosink", "sink");
      if (!sink) {
        g_error ("Failed to create element of type 'glimagesink' and "
                 "fallback 'autovideosink' also failed\n");
        g_option_context_free (ctx);
        return -1;
      }
    }
  }
  
  /* create source element */
  GstElement *src;
  src = gst_element_factory_make ("v4l2src", "source");
  if (!src) {
    g_error ("Failed to create element of type 'v4l2src'\n");
    gst_object_unref (GST_OBJECT (sink));
    g_option_context_free (ctx);
    return -1;
  }
  /* use device argument for source */
  if (device) {
    g_object_set (G_OBJECT (src), "device", device, NULL);
  }
  //TODO also set resolution at source as default is max.
  
  /* create pipeline */
  GstElement *pipeline;
  pipeline = gst_pipeline_new ("gstcam");
  if (fullscreen || resolution) {
    /* get sink dimensions */
    //TODO Get screen width and height via Caps or CallBack?
      
    /* create full-screen scaling filter */
    GstElement *vscale;
    GstElement *cfilter;
    vscale = gst_element_factory_make ("videoscale", "videoscale");
    cfilter = gst_element_factory_make ("capsfilter", "capsfilter");
    //TODO make the following more robust, also for width of four or two chars
    gchar width[5];
    gchar height[5];
    gchar scaling[34] = "video/x-raw,width=";
    if (resolution) {
      memcpy(width, resolution, 3);
      width[3] = '\0';
      memcpy(height, resolution+4, 3);
      height[3] = '\0';
      memcpy(scaling+18, width, 3);
      memcpy(scaling+21, ",height=", 8);
      memcpy(scaling+29, height, 3);
    } else {
      memcpy(width, "1280", 4);//TODO use screen width
      width[4] = '\0';
      memcpy(height, "768", 3);//TODO use screen height
      height[3] = '\0';
      memcpy(scaling+18, width, 4);
      memcpy(scaling+22, ",height=", 8);
      memcpy(scaling+30, height, 3);
    }
    GstCaps *caps = gst_caps_from_string (scaling);
    g_object_set (G_OBJECT (cfilter), "caps", caps, NULL);

    gst_bin_add_many (GST_BIN (pipeline), src, vscale, cfilter, sink, NULL);
    if (!gst_element_link_many (src, vscale, cfilter, sink, NULL)) {
      g_error ("Failed to link elements");
      gst_object_unref (GST_OBJECT (pipeline)); // unrefs also its elements
      g_option_context_free (ctx);
      return -1;
    }
  } else {
    gst_bin_add_many (GST_BIN (pipeline), src, sink, NULL);
    if (!gst_element_link_many (src, sink, NULL)) {
      g_error ("Failed to link elements");
      gst_object_unref (GST_OBJECT (pipeline)); // unrefs also its elements
      g_option_context_free (ctx);
      return -1;
    }
  }
  
  /* add watch to pipeline message bus */
  GstBus *bus;
  guint bus_watch_id;
  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  bus_watch_id = gst_bus_add_watch (bus, my_bus_callback, NULL);
  gst_object_unref (bus);

  /* play */
  gst_element_set_state (pipeline, GST_STATE_PLAYING);
  
  /* main loop */
  loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (loop);
  
  /* clean up */
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (GST_OBJECT (pipeline)); // unrefs also its elements
  g_source_remove (bus_watch_id);
  g_main_loop_unref (loop);
  g_option_context_free (ctx);

  return 0;
}


//https://gstreamer.freedesktop.org/data/doc/gstreamer/head/manual/html/chapter-helloworld.html

