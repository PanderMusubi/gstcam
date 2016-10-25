gstcam
======

_Play video from local camera with GStreamer without network streaming_


Description
-----------

Small application using GStreamer to play video stream from a camera that is
locally attached to the system, without using any network streams at all. The
`4vl2src` source from GStreamer is used as input. This will search for video
input, starting at `/dev/video0`. The output is send to GStreamer's sink
`glimagesink`. This will show the video on the default display.

[![Logo GStreamer](images/logo-gstreamer.png?raw=true)](http://gstreamer.freedesktop.org)

The target platform for this application is mainly the Raspberry Pi without X
installed. However, it should also work on any system with or without X
installed. The camera can be a USB camera attached to the system or any other
camera that can be found by GStreamer.

![Component diagram](images/component-diagram.png?raw=true)

Apart from that this application doesn't need a network or storage, it is also
guaranteed that no network or storage will be used.


Usage
-----

The application can be run with the command:

    ./bin/gstcam-MACHINE-DISTRIB-VERSION

Where MACHINE, DISTRIB and VERSION need to be replaced with the name of the
machine's (system's) architecture and the name and version of the distribution
(operating system) installed. Simply see what is available in the `bin`
directory or has been created by the script `build.sh` on your system.

Only a few command-line options and arguments can be passed to this application
in order to keep this application simple and fast.

**-h, --help**

Shows the help options.

**-a, --autovideosink**

Normally, the GL Image sink is used for video output. In case that that is not
available, this option will let GStreamer use `autovideosink` to find a suitable
video sink.

**-f, --fullscreen**

In case the resolution from the camera ... option ...

**-d, --device=DEVICE**

GStreamer will search for the first camera available, starting at `/dev/video0`.
This can be the camera mounted in a laptop's lid. When it is preferred to use
another camera that is attached to the system, the specific device can be given
as an argument via this option.


Building
--------

Prerequisites for building this application can be installed with:

    sudo apt-get install gcc libgstreamer1.0-dev

The application can be build by running:

    ./build.sh

and the resulting binary can be found in the `bin` directory. Note that several
builds are already available.

The source code can be checked with:

    ./check.sh

For checking, one has to have installed:

    sudo apt-get install splint cppcheck


To do
-----

See source code for all comments with TODO. Try to get this application included
in some GStreamer DEB package to get it deployed on Ubuntu and Debian (and
Raspbian).


License
-------

For this application the MIT License applies, see also the file
[LICENSE](LICENSE). The author of this software is
[Stichting z25.org](http://z25.org).

[![Logo z25](images/logo-z25.png?raw=true)](http://z25.org)
