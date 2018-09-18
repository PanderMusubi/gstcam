LABEL:=$(shell sh -c "./label.sh")
GLIB_H:=$(shell sh -c "pkg-config --cflags glib-2.0")
GLIB_L:=$(shell sh -c "pkg-config --libs glib-2.0")
GST_H:=$(shell sh -c "pkg-config --cflags gstreamer-1.0")
GST_L:=$(shell sh -c "pkg-config --libs gstreamer-1.0")

#if [ `lspci|grep VGA|grep -i radeon|wc -l` -ne 0 ]; then
#	VDPAU_DRIVER='radeonsi vainfo'
#fi

all: release
	gcc -g -Wall -Wextra -O0  ${GLIB_H} ${GST_H} -o bin/gstcam-debug src/gstcam.c ${GLIB_L} ${GST_L}

release:
	gcc -Wall -O2 -fno-omit-frame-pointer ${GLIB_H} ${GST_H} -o bin/gstcam-${LABEL} src/gstcam.c ${GLIB_L} ${GST_L}

clean:
	rm -f bin/gstcam-debug
