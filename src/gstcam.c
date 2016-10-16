#include <gst/gst.h>

/* author Pander */

static GMainLoop *loop;

//TODO Not used yet, perhaps not needed at all when width and height are retrieved in another way.
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

//TODO Callback which you are suppose to have. If really not needed, can be removed. Otherwise, should be an implementation which is as minimally as possible.
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
  gboolean silent = FALSE;
  gchar *savefile = NULL;
  GOptionContext *ctx;
  GError *err = NULL;
  GOptionEntry entries[] = {
    { "autovideosink", 'a', 0, G_OPTION_ARG_NONE, &silent,
      "use auto video sink instead of GL Image sink", NULL },
    { "fullscreen", 'f', 0, G_OPTION_ARG_NONE, &silent,
      "use sink in full screen mode", NULL },
    { "device", 'd', 0, G_OPTION_ARG_STRING, &savefile,
      "use specified device as input, e.g. /dev/video1", "FILE" },
    { NULL }
  };
  ctx = g_option_context_new ("Device camera live player");
  g_option_context_add_main_entries (ctx, entries, NULL);
  g_option_context_add_group (ctx, gst_init_get_option_group ());
  if (!g_option_context_parse (ctx, &argc, &argv, &err)) {
    g_print ("Failed to initialize: %s\n", err->message);
    g_clear_error (&err);
    g_option_context_free (ctx);
    return 1;
  }

  //TODO How to retrieve the options and arguments?

  /* initialize gst */
  gst_init(0, NULL);
  
  /* create sink element */
  GstElement *sink;
  sink = gst_element_factory_make ("glimagesink", "sink");//TODO Use autovideosink when indicated in -a option.
  if (!sink) {
    g_print ("Failed to create element of type 'autovideosink'\n");
    g_option_context_free (ctx);
    return -1;
  }
  
  /* get sink dimensions */
  //TODO Get screen width and height via Caps or CallBack?
      
  /* create full-screen scaling filter */
  //TODO Only when -f option is given. Perhaps two elements are needed, goals is to get  "videoscale ! video/x-raw,width=1024,height=768" with max width and max height of the sink 
  /* create source element */
  GstElement *source;
  source = gst_element_factory_make ("v4l2src", "source");
  if (!source) {
    g_print ("Failed to create element of type 'v4l2src'\n");
    gst_object_unref (GST_OBJECT (sink));
    //TODO If filter was created, clean up too!
    g_option_context_free (ctx);
    return -1;
  }
  //TODO Add device=/dev/video1 to the element when indicated in -d agument
  
  /* create pipeline */
  GstElement *pipeline;
  pipeline = gst_pipeline_new ("device-cam");
  gst_bin_add_many (GST_BIN (pipeline), source, sink, NULL);//TODO Include filter when needed.
  if (!gst_element_link_many (source, sink, NULL)) {//TODO Include filter when needed.
    g_warning ("Failed to link elements!");
    //TODO clean up, probably only pipeline and ctx
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
  gst_object_unref (GST_OBJECT (pipeline)); // Unrefs elements too.
  g_source_remove (bus_watch_id);
  g_main_loop_unref (loop);
  g_option_context_free (ctx);

  return 0;
}


//https://gstreamer.freedesktop.org/data/doc/gstreamer/head/manual/html/chapter-helloworld.html
