# définition des commandes utilisées
SHELL = /bin/sh
CC = g++
RM = rm -f


CPPFLAGS = -I/usr/local/include
CFLAGS = -g -Wall -O3 
LDFLAGS =   -L/usr/local/lib -lopencv_core -lopencv_highgui   -std=c++11 -pthread 

#définition des fichiers et dossiers
PROGNAME = julia
PACKAGE=$(PROGNAME)
VERSION = 1.0
distdir = $(PACKAGE)-$(VERSION)
HEADERS = 
SOURCES = main.cpp
OBJ = $(SOURCES:.c=.o)

all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(PROGNAME)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) -r $(PROGNAME) .o


