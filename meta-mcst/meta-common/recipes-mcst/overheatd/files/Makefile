BINARYNAME=overheatd

all: $(BINARYNAME)

$(BINARYNAME): $(BINARYNAME).o
	$(CC) -o $(BINARYNAME) $(BINARYNAME).o -lgpiod -li2c -lrt -lfdt -lreimu -lreimu_gpio -lreimu_fdt $(LDFLAGS)

$(BINARYNAME).o: $(BINARYNAME).c
	$(CC) -o $(BINARYNAME).o -c $(BINARYNAME).c

clean:
	rm -f *.o $(BINARYNAME)
