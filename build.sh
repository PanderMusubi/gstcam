gcc `pkg-config --cflags glib-2.0` `pkg-config --cflags gstreamer-1.0` -o gstcam src/gstcam.c `pkg-config --libs glib-2.0` `pkg-config --libs gstreamer-1.0`
