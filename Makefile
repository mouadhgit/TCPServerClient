#******************************************************************************
# Copyright (C) 2022 by Mouadh Dahech 
#
#*****************************************************************************

include sources.mk

TARGET1 = Server
TARGET2 = Client
OUTPUT1 = $(TARGET1).out
OUTPUT2 = $(TARGET2).out

BUILDIR = build

CC = gcc
CFLAGS = -Wall -Werror -g -O0 -std=c99 
LDFLAGS = -l pthread
SIZE = size
BuildFolder = ./$(BUILDIR)

OBJECTS1 = $(SOURCES1:.c=.o)
OBJECTS2 = $(SOURCES2:.c=.o)

$(shell   mkdir -p $(BUILDIR))

%.o : %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $< 

.PHONY: all
all: Server Client
	
Server: $(OBJECTS1)
	$(CC) $(OBJECTS1) $(CFLAGS) $(LDFLAGS) $(INCLUDES) -o $(BuildFolder)/$(OUTPUT1)
	$(SIZE) $(BuildFolder)/$(OUTPUT1)

Server.o: compile-all
compile-all: $(OBJECTS1)

Client:	$(OBJECTS2)
	$(CC) $(OBJECTS2) $(CFLAGS) $(INCLUDES) -o $(BuildFolder)/$(OUTPUT2)
	$(SIZE) $(BuildFolder)/$(OUTPUT2)

Client.o: compile-all
compile-all: $(OBJECTS2)

.PHONY: clean

run: all
	gnome-terminal -- $(BuildFolder)/$(OUTPUT2)
	gnome-terminal -- $(BuildFolder)/$(OUTPUT2)
	gnome-terminal -- $(BuildFolder)/$(OUTPUT2)
	sleep 5
	gnome-terminal -- $(BuildFolder)/$(OUTPUT1) 
	
clean:
	rm -f $(OBJECTS1) $(OBJECTS2) $(BuildFolder)/$(OUTPUT1) $(BuildFolder)/$(OUTPUT2)
	
mrproper: clean
	rm -r $(BuildFolder)
	