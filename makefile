#	Makefile : make FTPR_Server or FTPR_Client.

# Label.
.PHONY: clean server client

# Hint message for none specify target.
all:Hint_Message

# Primary path of build files.
vpath %.h include/*
vpath %.c src/*
vpath %.cpp src/*

# Suffic rule.
.SUFFIX: .cpp .o .c
*.cpp:*.o
	g++ -g -o $@ -c $^

Hint_Message:
	@echo "Please specify target : clean/server/client"




clean:
	@rm -f *.o


