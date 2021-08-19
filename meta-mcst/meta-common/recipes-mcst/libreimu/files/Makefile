VERSION  = 0.1
CFLAGS  ?= -g -O3 -Wall
MODULES  = reimu reimu_gpio reimu_dbus reimu_fdt

reimu_CFLAGS      =
reimu_gpio_CFLAGS =
reimu_dbus_CFLAGS = $(shell pkg-config --cflags dbus-1)
reimu_fdt_CFLAGS  =
reimu_LIBS        =
reimu_gpio_LIBS   = -lgpiod
reimu_dbus_LIBS   = $(shell pkg-config --libs dbus-1)
reimu_fdt_LIBS    = -lfdt

.PHONY: all clean

V   ?= 0
Q    = $(if $(filter-out 0,$(V)),,@)
LOG  = $(if $(filter-out 0,$(V)),true,echo -e)

all: reimu.h $(addprefix lib,$(addsuffix .so.$(VERSION),$(MODULES)))

lib%.so.$(VERSION): %.o
	@$(LOG) \\tLD\\t$@
	$(Q)$(CC) -shared -Wl,-soname,$(firstword $(subst ., ,$@)).so.$(firstword $(subst ., ,$(VERSION))) $(LDFLAGS) $($(subst lib,,$(firstword $(subst ., ,$@)))_LIBS) -o $@ $^

.SECONDARY:
%.o: %.c
	@$(LOG) \\tCC\\t$@
	$(Q)$(CC) -o $@ -c -fPIC $(CFLAGS) $($(firstword $(subst ., ,$@))_CFLAGS) $<

clean:
	rm -f reimu*.o libreimu*.so.*
