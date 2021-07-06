BINARYNAME=hwmon-instantiator

all: $(BINARYNAME)

$(BINARYNAME): $(BINARYNAME).o
	$(CC) -o $(BINARYNAME) $(BINARYNAME).o -lfdt $(LDFLAGS)

$(BINARYNAME).o: $(BINARYNAME).c
	$(CC) -o $(BINARYNAME).o -c $(BINARYNAME).c

clean:
	rm -f *.o $(BINARYNAME)
