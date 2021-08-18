VERSION     = 0.1

PREFIX     ?= /usr/local
DESTDIR    ?=
LIBDIR      = $(DESTDIR)$(PREFIX)/lib

CFLAGS     ?= -g -O3 -Wall

HEADERFILE  = reimu.h

SHAREDLIB   = libreimu.so.$(VERSION)
SONAME      = libreimu.so.$(firstword $(subst ., ,$(VERSION)))

SHAREDLIB_F = libreimu_fdt.so.$(VERSION)
SONAME_F    = libreimu_fdt.so.$(firstword $(subst ., ,$(VERSION)))

.PHONY: all clean

V ?= 0
Q = $(if $(filter-out 0,$(V)),,@)
LOG = $(if $(filter-out 0,$(V)),true,echo -e)

all: $(HEADERFILE) $(SHAREDLIB) $(SHAREDLIB_F)

$(SHAREDLIB): reimu.o
	@$(LOG) \\tLD\\t$@
	$(Q)$(CC) -shared -Wl,-soname,$(SONAME) $(LDFLAGS) -o $@ $^

$(SHAREDLIB_F): reimu_fdt.o
	@$(LOG) \\tLD\\t$@
	$(Q)$(CC) -shared -Wl,-soname,$(SONAME_F) $(LDFLAGS) -lfdt -o $@ $^

%.o: %.c
	@$(LOG) \\tCC\\t$@
	$(Q)$(CC) -o $@ -c -fPIC $(CFLAGS) $<

clean:
	rm -f *.o $(SHAREDLIB) $(SONAME) $(SHAREDLIB_F) $(SONAME_F)
