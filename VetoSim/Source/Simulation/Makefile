CPPFLAGS:=
CFLAGS:=$(shell root-config --cflags)
LDFLAGS:=
LIBS:=$(shell root-config --libs) $(shell pkg-config freetype2 --libs)

######################################################################
MACRO:=Create_ASCII_File

.PHONY: all
all: $(MACRO)

$(MACRO): $(MACRO).o
	g++ -o $@ $^ $(LIBS)

$(MACRO).o: $(MACRO).cpp Makefile
	g++ -c -o $@ $< $(CPPFLAGS) $(CFLAGS)
	
MACRO:=Create_ASCII_File_Uniform

.PHONY: all
all: $(MACRO)

$(MACRO): $(MACRO).o
	g++ -o $@ $^ $(LIBS)

$(MACRO).o: $(MACRO).cpp Makefile
	g++ -c -o $@ $< $(CPPFLAGS) $(CFLAGS)
	
MACRO:=Boosting

.PHONY: all
all: $(MACRO)

$(MACRO): $(MACRO).o
	g++ -o $@ $^ $(LIBS)

$(MACRO).o: $(MACRO).cpp Makefile
	g++ -c -o $@ $< $(CPPFLAGS) $(CFLAGS)
	
MACRO:=G4Secondaries

.PHONY: all
all: $(MACRO)

$(MACRO): $(MACRO).o
	g++ -o $@ $^ $(LIBS)

$(MACRO).o: $(MACRO).cpp Makefile
	g++ -c -o $@ $< $(CPPFLAGS) $(CFLAGS)
	
MACRO:=CheckG4Engine

.PHONY: all
all: $(MACRO)

$(MACRO): $(MACRO).o
	g++ -o $@ $^ $(LIBS)

$(MACRO).o: $(MACRO).cpp Makefile
	g++ -c -o $@ $< $(CPPFLAGS) $(CFLAGS)

MACRO:=SetG3

.PHONY: all
all: $(MACRO)

$(MACRO): $(MACRO).o
	g++ -o $@ $^ $(LIBS)

$(MACRO).o: $(MACRO).cpp Makefile
	g++ -c -o $@ $< $(CPPFLAGS) $(CFLAGS)
	
MACRO:=SetG4

.PHONY: all
all: $(MACRO)

$(MACRO): $(MACRO).o
	g++ -o $@ $^ $(LIBS)

$(MACRO).o: $(MACRO).cpp Makefile
	g++ -c -o $@ $< $(CPPFLAGS) $(CFLAGS)
	
MACRO:=SetG34

.PHONY: all
all: $(MACRO)

$(MACRO): $(MACRO).o
	g++ -o $@ $^ $(LIBS)

$(MACRO).o: $(MACRO).cpp Makefile
	g++ -c -o $@ $< $(CPPFLAGS) $(CFLAGS)

MACRO:=STaRTrackReconstructor

.PHONY: all
all: $(MACRO)

$(MACRO): $(MACRO).o
	g++ -o $@ $^ $(LIBS)

$(MACRO).o: $(MACRO).cpp Makefile
	g++ -c -o $@ $< $(CPPFLAGS) $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o *_C.* $(MACRO)