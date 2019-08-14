add_executable(gstcam gstcam.c)
target_include_directories(gstcam PRIVATE ${GSTREAMER_INCLUDE_DIRS})
target_link_libraries(gstcam ${GSTREAMER_LIBRARIES})
