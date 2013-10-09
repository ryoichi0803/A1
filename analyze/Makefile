CC = g++
RM = rm
CP = cp

CFLAGS = -O3 `root-config --cflags` -Wall -c
LIBS =  `root-config --libs` -Wall

all: channel_to_ns muon_g muon_lifetime

channel_to_ns: channel_to_ns.o
	$(CC) -o $@ $(LIBS) $<

muon_g: muon_g.o convert_tdc_to_ns.o
	$(CC) -o $@ $(LIBS) $< convert_tdc_to_ns.o

muon_lifetime: muon_lifetime.o convert_tdc_to_ns.o
	$(CC) -o $@ $(LIBS) $< convert_tdc_to_ns.o

.cpp.o: muon_g.cpp channel_to_ns.cpp convert_tdc_to_ns.cpp
	$(CC) $(CFLAGS) $<

clean:
	$(RM) *.o channel_to_ns muon_lifetime muon_g
	$(RM) ../bin/channel_to_ns ../bin/muon_g ../bin/muon_lifetime

install:
	mkdir -p ../bin/
	$(CP) channel_to_ns ../bin/
	$(CP) muon_lifetime ../bin/
	$(CP) muon_g  ../bin/
