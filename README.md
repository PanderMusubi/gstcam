gstcam
======

_Play video from local camera with GStreamer without network streaming_


Description
-----------

Small application using GStreamer to play video stream from a camera that is locally attached to the system, without using any network streams at all. The `4vl2src` source from GStreamer is used as input. This will search for video input, starting at `/dev/video0`. The output is send to GStreamer's sink `glimagesink`. This will show the video on the default display.

The target platorm for this application is mainly the Raspberry Pi without X installed. However, it should also work on any system with X installed. The camera can be a USB camera attached to the system or any other camera that can be found by GStreamer.


Usage
-----

The application can be run with the command:

    ./gstcam

Only a few command-line options and arguments can be passed to this application in order to keep this application simple and fast.

**-h, --help**

Shows the help options.

**-a, --autovideosink**

Normally, the GL Image sink is used for video output. In case that that is not available, this option will let GStreamer use `autovideosink` to find a suitable video sink.

**-f, --fullscreen**

In case the resolution from the camera ... option ...

**-d, --device=DEVICE**

GStreamer will search for the first camera available, starting at `/dev/video0`. This can be the camera mounted in a laptop's lid. When it is preferred to use another camera that is attached to the system, the specific device can be given as an argument via this option.


Building
--------

Prerequisites for building this application can be installed with:

    sudo apt-get install gcc libgstreamer1.0-dev

The application can be build by running:

    ./build.sh


To do
-----

See source code for all comments with TODO. Try to get this application included in some GStreamer DEB package to get it deployed on Ubuntu and Debian (and Raspbian).


License
-------

For this application the MIT License applies, see also the file [LICENSE].
