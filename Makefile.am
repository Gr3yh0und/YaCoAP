include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

EXTRA_DIST              = \
                          coap.h \
                          $(NULL)

lib_LIBRARIES           = libyacoap.a

libyacoap_a_CPPFLAGS = \
                          -I$(top_srcdir)/third_party/yacoap    \
                          $(CPPFLAGS)                           \
                          $(NULL)

libyacoap_a_SOURCES  = \
                          coap.c                          \
                          coap_parse.c                    \
                          coap_dump.c                     \
                          resources.c                     \
                          $(NULL)
                          
noinst_HEADERS          =  \
                           coap.h          \
                           inet.h          \
                           $(NULL)

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
