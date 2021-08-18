BINARYNAME=vncpasswd

all: $(BINARYNAME)

$(BINARYNAME): $(BINARYNAME).o
	$(CC) -o $(BINARYNAME) $(BINARYNAME).o -lvncserver -lreimu $(LDFLAGS)

$(BINARYNAME).o: $(BINARYNAME).c
	$(CC) -o $(BINARYNAME).o -c $(BINARYNAME).c

clean:
	rm -f *.o $(BINARYNAME)
