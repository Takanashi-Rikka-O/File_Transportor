#	Makefile : make FTPR_Server or FTPR_Client.

# Label.
.PHONY: clean server client destroy

# Hint message for none specify target.
all:Hint_Message

# Makefile variables
CONFIGUREDIR=/etc/ftprd
C_CONFIGUREDIR=$(CONFIGUREDIR)/client
S_CONFIGUREDIR=$(CONFIGUREDIR)/server

# Translater option.

CC=g++
CFLAGS=-Wall -g -std=c++11 -Iinclude/FTPR -Iinclude/FTPR_server -Iinclude/FTPR_client

ServerDeps=FTPR_server.o FTPR_server_main.o
ClientDeps=FTPR_client.o FTPR_client_main.o
CommonDeps=FTPR.o FID_class.o THREAD_class.o TCP_SOCK_class.o SYSTEM_SIGNAL_class.o

AsmOBJ=_Copy_String_.o

ShareLib=libFTPR_Basic.so libFTPR_THREAD.so


ALLOBJS=$(ServerDeps) $(ClientDeps) $(CommonDeps)

# Primary path of build files.
vpath %.cpp src/FTPR
vpath %.cpp src/FTPR_server
vpath %.s fobj

# Static rule
$(ALLOBJS):%.o:%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

Hint_Message:
	@echo "Please specify target : clean/server/client/destory"

server:$(ServerDeps) $(ShareLib) $(AsmOBJ) $(CommonDeps)
	$(CC) $(CFLAGS) -o ftprsd $(ServerDeps) $(CommonDeps) $(AsmOBJ) -lFTPR_Basic -lFTPR_THREAD -lpthread
	ldconfig
	@if [ ! -e $(CONFIGUREDIR) ]; \
	then \
		mkdir $(CONFIGUREDIR); \
		mkdir $(C_CONFIGUREDIR); \
		mkdir $(S_CONFIGUREDIR); \
	fi
	cp FTPR_client.conf $(C_CONFIGUREDIR)
	cp FTPR_server.conf $(S_CONFIGUREDIR)

$(AsmOBJ):_Copy_String_.s
	as --gstabs -o $@ $<

libFTPR_Basic.so:libFTPR_Basic.cpp
	$(CC) -fPIC -shared $(CFLAGS) -o $@ $< -std=c++11
	cp $@ /usr/lib/

libFTPR_THREAD.so:libFTPR_THREAD.cpp
	$(CC) -fPIC -shared $(CFLAGS) -o $@ $< -std=c++11
	cp $@ /usr/lib/

clean:
	@rm -f *.o *.so

# Pay a notice on there,do not adding option '-r' for 'rm'.
destroy:
	@rm --interactive /usr/lib/libFTPR_Basic.so /usr/lib/libFTPR_THREAD.so


