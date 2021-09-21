BINARYNAME=find_i2c_device

all: $(BINARYNAME)-module $(BINARYNAME)-platform

$(BINARYNAME)-$(LABEL): $(BINARYNAME).o
	$(CC) -o $(BINARYNAME) $(BINARYNAME).o -DRTC_LABEL="$(LABEL)" -lfdt -lreimu -lreimu_fdt $(LDFLAGS)

$(BINARYNAME).o: $(BINARYNAME).c
	$(CC) -o $(BINARYNAME).o -c $(BINARYNAME).c

clean:
	rm -f *.o $(BINARYNAME)
