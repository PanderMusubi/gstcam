# gstcam

_Play video from local camera with GStreamer without network streaming_


## 1 Description

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


## 2 Usage

The application can be run with the command:

    ./bin/gstcam-MACHINE-DISTRIB-VERSION-gst-GSTREM

Where MACHINE, DISTRIB, VERSION and GSTREAM need to be replaced with the name
of the machine's (system's) architecture, the name and version of the
distribution (operating system) and the version of GStreamer installed. Simply
see what is available in the `bin` directory or has been created by the script
`build.sh` on your system and choose the most likely to match your system.

Examples of available binaries are:
* `./bin/gstcam-x86_64-Ubuntu-16.10-gst-1.8.3`
* `./bin/gstcam-armv7l-Raspbian-8.0-gst-1.4.4`
* `./bin/gstcam-aarch64-Ubuntu-16.04-gst-1.8.3`

Only a few command-line options and arguments can be passed to this application
in order to keep this application simple and fast.

**-h, --help**

Shows the help options.

**-a, --autovideosink**

Normally, the GL Image sink is used for video output. In case that that is not
available, this option will let GStreamer use `autovideosink` to find a
suitable video sink.

**-f, --fullscreen**

TODO Uses the full resolution of the screen ... option ... Is overridden by
--resolution.

**-d, --device=FILE**

GStreamer will search for the first camera available, starting at
`/dev/video0`. This can be the camera mounted in a laptop's lid. When it is
preferred to use another camera that is attached to the system, the specific
device can be given as an argument via this option.

**-r, --resolution=WIDTHxHEIGTH**

TODO Sets the resolution of the screen. The fullscreen option will be ignored.


## 3 Alternatives

The following application provide alternatives to gstcam but might have more
requirements or dependencies for installation.


### 3.1 GStreamer Launcher (gst-launch)

[GStreamer](http://gstreamer.freedesktop.org) Launcher can be run with:

    gst-launch-1.0 -q v4l2src ! glimagesink


### 3.2 Libav

[Libav](https://libav.org) can be run with:

    TODO


### 3.3 FFmpeg

[FFmpeg](https://ffmpeg.org) can be run with:

    TODO


### 3.4 GTK+ UVC Viewer (guvcview)

[GTK+ UVC Viewer](http://guvcview.sourceforge.net) (guvcview) can be run with:

    guvcview -d /dev/video0 -x 1280x768 -m max -g none


### 3.5 Cheese

[Cheese](https://wiki.gnome.org/Apps/Cheese) can be run with:

    cheese -f -d /dev/video0

See https://bugzilla.gnome.org/show_bug.cgi?id=780849 for the -d option.


### 3.6 Webcamoid

[Webcamoid](https://webcamoid.github.io) can be run with:

    webcamoid

Note that this application does not yet have command line arguments.


### 3.7 VLC media player

[VLC](https://videolan.org/vlc) can be run with:

    vlc TODO


### 3.8 MPV

[MPV](https://mpv.io) can be run with:

    TODO


### 3.9 Xine

[Xine](https://xine-project.org) can be run with:

    TODO


## 4 Building

Prerequisites for building this application can be installed with:

    sudo apt-get install gcc libgstreamer1.0-dev gstreamer1.0-tools

The application can be build by running:

    ./build.sh

and the resulting binary can be found in the `bin` directory. Note that several
builds are already available.

The source code can be checked with:

    ./check.sh

For checking, one has to have installed:

    sudo apt-get install splint cppcheck


## 5 To do

See source code for all comments with TODO. Try to get this application
included in some GStreamer DEB package to get it deployed on Ubuntu and Debian
(and Raspbian).


## 6 License

For this application the MIT License applies, see also the file
[LICENSE](LICENSE). The author of this software is
[Stichting z25.org](http://z25.org).

[![Logo z25](images/logo-z25.png?raw=true)](http://z25.org)
