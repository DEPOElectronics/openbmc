BINARYNAME = find_i2c_device
TARGETS = $(BINARYNAME)-module $(BINARYNAME)-platform

.PHONY: all clean

all: $(TARGETS)

%: %.o
	$(CC) -o $@ $^ -lfdt -lreimu -lreimu_fdt $(LDFLAGS)

%.o: $(BINARYNAME).c
	$(CC) -o $@ -DRTC_LABEL=\"$(patsubst $(BINARYNAME)-%.o,%,$@)\" -c $^

clean:
	rm -f *.o $(TARGETS)
