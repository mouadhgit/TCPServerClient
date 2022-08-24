#******************************************************************************
# Copyright (C) 2022 by Mouadh Dahech - Focus Corporation 
#
#*****************************************************************************

BUILD_DIR = build
CC = gcc
CFLAGS = -Wall
BuildFolder = ./$(BUILD_DIR)
EXE_Server = Server
EXE_Client = Client

$(shell   mkdir -p $(BUILD_DIR))

all:Server Client

Server:Server.c
	$(CC) $(CFLAGS) Server.c -o $(BuildFolder)/$(EXE_Server)
Client:Client.c
	$(CC) $(CFLAGS) Client.c -o $(BuildFolder)/$(EXE_Client)
clean:
	rm -rf $(BuildFolder)/$(EXE_Server) $(BuildFolder)/$(EXE_Client)
mrproper: clean
	rm -r $(BuildFolder)
	