MACHINE=`uname -m`
DISTRIB=`lsb_release -i|awk -F ':\t' '{print $2}'`
VERSION=`lsb_release -r|awk -F ':\t' '{print $2}'`
GSTREAM=`gst-inspect-1.0 --version|grep ^GStreamer|awk '{print $2}'`

bin/gstcam-$MACHINE-$DISTRIB-$VERSION-gst-$GSTREAM
