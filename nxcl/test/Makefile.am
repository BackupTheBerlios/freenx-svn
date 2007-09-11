AM_CPPFLAGS = @PACKAGE_CFLAGS@ -DPACKAGE_DATA_DIR=\""$(datadir)"\" -DLOCALEDIR=\"$(localedir)\" -DPACKAGE_BIN_DIR=\""$(bindir)"\"
INCLUDES = -I../lib
bin_PROGRAMS = libtest nxcmd notQttest
libtest_SOURCES = libtest.cpp
libtest_LDADD = @PACKAGE_LIBS@ $(LIBINTL) -L../lib -lnxcl
notQttest_SOURCES = notQttest.cpp
notQttest_LDADD = @PACKAGE_LIBS@ $(LIBINTL) -L../lib -lnxcl
nxcmd_SOURCES = nxcmd.cpp
nxcmd_LDADD = @PACKAGE_LIBS@ $(LIBINTL)
#pkginclude_HEADERS = header.h

