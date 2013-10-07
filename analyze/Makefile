CC = g++
RM = rm

CFLAGS = -O3 `root-config --cflags` -Wall -c
LIBS =  `root-config --libs` -Wall

channel_to_ns: channel_to_ns.o
	$(CC) -o channel_to_ns $(LIBS) $<

.cpp.o:
	$(CC) $(CFLAGS) $<

clean:
	$(RM) *.o channel_to_ns
