lib_LTLIBRARIES = libtestgear.la

libtestgeardir = $(includedir)/testgear
libtestgear_HEADERS = include/testgear/testgear.h

libtestgear_la_SOURCES = \
    message.c            \
    session.c            \
    tcp.c                \
    testgear.c           \
    include/testgear/message.h \
    include/testgear/session.h \
    include/testgear/tcp.h     \
    include/testgear/testgear.h

libtestgear_la_CFLAGS = -I$(top_srcdir)/include

libtestgear_la_LDFLAGS = -version-info $(LIBTESTGEAR_LT_VERSION) -init,init
