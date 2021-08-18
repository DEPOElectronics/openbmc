UNITNAME    = reimu
VERSION     = 0.1

PREFIX     ?= /usr/local
DESTDIR    ?=
LIBDIR      = $(DESTDIR)$(PREFIX)/lib
INCLUDEDIR  = $(DESTDIR)$(PREFIX)/include

CFLAGS     ?= -g -O3 -Wall

LIBRARYNAME = lib$(UNITNAME)
HEADERFILE  = $(UNITNAME).h
OBJFILE     = $(UNITNAME).o
STATICLIB   = $(LIBRARYNAME).a
LINKERNAME  = $(LIBRARYNAME).so
SHAREDLIB   = $(LINKERNAME).$(VERSION)
SONAME      = $(LINKERNAME).$(firstword $(subst ., ,$(VERSION)))

.PHONY: all shared static clean install uninstall

V ?= 0
Q = $(if $(filter-out 0,$(V)),,@)
LOG = $(if $(filter-out 0,$(V)),true,echo -e)

all: static shared $(HEADERFILE)

static: $(STATICLIB)

shared: $(SHAREDLIB)

$(STATICLIB): $(OBJFILE)
	@$(LOG) \\tAR\\t$@
	$(Q)$(AR) rcs $@ $^

$(SHAREDLIB): $(OBJFILE)
	@$(LOG) \\tLD\\t$@
	$(Q)$(CC) -shared -Wl,-soname,$(SONAME) $(LDFLAGS) -o $@ $^

$(OBJFILE): $(UNITNAME).c

%.o:
	@$(LOG) \\tCC\\t$@
	$(Q)$(CC) -o $@ -c -fPIC $(CFLAGS) $<

clean:
	rm -f *.o $(SHAREDLIB) $(STATICLIB) $(SONAME) $(LINKERNAME)

install: all
	install -d $(LIBDIR)
	install -d $(INCLUDEDIR)
	install -m 755 $(SHAREDLIB) $(LIBDIR)
	install -m 644 $(STATICLIB) $(LIBDIR)
	install -m 644 $(HEADERFILE) $(INCLUDEDIR)
	ldconfig
	ln -s $(SHAREDLIB) $(LIBDIR)/$(LINKERNAME)

uninstall:
	rm -f $(LIBDIR)/$(SHAREDLIB)
	rm -f $(LIBDIR)/$(LINKERNAME)
	rm -f $(LIBDIR)/$(SONAME)
	rm -f $(LIBDIR)/$(STATICLIB)
	rm -f $(INCLUDEDIR)/$(HEADERFILE)
