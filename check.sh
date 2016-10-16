 splint -linelen 320 -predboolint -maintype -boolops \
 -I/usr/include \
 -I/usr/include/x86_64-linux-gnu \
 `pkg-config --cflags glib-2.0` \
 `pkg-config --cflags gstreamer-1.0|awk -F 'thread ' '{print $2}'` \
 src/gstcam.c
 
 cppcheck --language=c --enable=all --force -U GST_CAT_DEFAULT \
-I/usr/include \
-I/usr/include/linux \
-I/usr/include/x86_64-linux-gnu \
-I/usr/lib/gcc/x86_64-linux-gnu/6/include \
`pkg-config --cflags glib-2.0` \
`pkg-config --cflags gstreamer-1.0|awk -F 'thread ' '{print $2}'` \
src/gstcam.c
