#	Makefile : make FTPR_Server or FTPR_Client.

# Label.
.PHONY: clean server client destroy

# Hint message for none specify target.
all:Hint_Message

# Makefile variables
CONFIGUREDIR=/etc/ftpr
C_CONFIGUREDIR=$(CONFIGUREDIR)/client
S_CONFIGUREDIR=$(CONFIGUREDIR)/server

# Translater option.

CC=g++
CFLAGS=-Wall -g -std=c++11 -Iinclude/FTPR -Iinclude/FTPR_server -Iinclude/FTPR_client -DDEBUG

ServerDeps=FTPR_server.o FTPR_server_main.o
ClientDeps=FTPR_client.o FTPR_client_main.o
CommonDeps=FTPR.o FID_class.o THREAD_class.o TCP_SOCK_class.o SYSTEM_SIGNAL_class.o

AsmOBJ=_Copy_String_.o	# Assembly function.

ShareLib=libFTPR_Basic.so libFTPR_THREAD.so	# Shared libs.
ShareLibServer=libFTPR_Server.so
ShareLibClient=libFTPR_Client.so
LIBS=$(ShareLib) $(ShareLibServer) $(ShareLibClient)

ALLOBJS=$(ServerDeps) $(ClientDeps) $(CommonDeps)	# All object target.

# Primary path of build files.
vpath %.cpp src/FTPR
vpath %.cpp src/FTPR_server
vpath %.cpp src/FTPR_client
vpath %.cpp lib/
vpath %.s fobj

# Static rule
$(ALLOBJS):%.o:%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

$(LIBS):%.so:%.cpp
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $<
	cp $@ /usr/lib/

Hint_Message:
	@echo "Please specify target : clean/server/client/destroy"

# Server
server:$(ServerDeps) $(ShareLib) $(AsmOBJ) $(CommonDeps) $(ShareLibServer)
	ldconfig
	$(CC) $(CFLAGS) -o ftprsd $(ServerDeps) -lFTPR_Server -lFTPR_Basic -lFTPR_THREAD $(CommonDeps) -lpthread $(AsmOBJ)
	mv --interactive ftprsd bin/
	@if [ ! -e $(CONFIGUREDIR) ]; \
	then \
		mkdir $(CONFIGUREDIR); \
		mkdir $(S_CONFIGUREDIR); \
	fi
	cp FTPR_server.conf $(S_CONFIGUREDIR)

# Client
client:$(ClientDeps) $(ShareLib) $(AsmOBJ) $(CommonDeps) $(ShareLibClient)
	ldconfig
	$(CC) $(CFLAGS) -o ftprc $(ClientDeps) -lFTPR_Client -lFTPR_Basic -lFTPR_THREAD $(CommonDeps)  -lpthread $(AsmOBJ)
	mv --interactive ftprc bin/
	@if [ ! -e $(CONFIGUREDIR) ]; \
	then \
		mkdir $(CONFIGUREDIR); \
		mkdir $(C_CONFIGUREDIR); \
	fi
	cp FTPR_client.conf $(C_CONFIGUREDIR)

$(AsmOBJ):_Copy_String_.s
	as --gstabs -o $@ $<

clean:
	@rm -f *.o *.so

# Pay a notice on there,do not adding option '-r' for 'rm'.
destroy:
	@rm --interactive bin/ftprsd bin/ftprc /usr/lib/libFTPR_Basic.so /usr/lib/libFTPR_THREAD.so /usr/lib/libFTPR_Client.so /usr/lib/libFTPR_Server.so


