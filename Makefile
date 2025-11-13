CC=gcc
UNAME_S := $(shell uname -s)

# Default flags
CFLAGS=-I./common
LDFLAGS=
LDLIBS=-lssl -lcrypto

# macOS specific flags
ifeq ($(UNAME_S), Darwin)
    OPENSSL_PATH := $(shell brew --prefix openssl@3 2>/dev/null)
    ifneq ($(OPENSSL_PATH),)
        CFLAGS += -I$(OPENSSL_PATH)/include
        LDFLAGS += -L$(OPENSSL_PATH)/lib
    else
        OPENSSL_PATH_1_1 := $(shell brew --prefix openssl@1.1 2>/dev/null)
        ifneq ($(OPENSSL_PATH_1_1),)
            CFLAGS += -I$(OPENSSL_PATH_1_1)/include
            LDFLAGS += -L$(OPENSSL_PATH_1_1)/lib
        else
            $(warning "OpenSSL not found via brew. Assuming it's in a standard location.")
        endif
    endif
endif

all: client/totp verifier/verifier

# Rule for the client
client/totp: client/main.o common/util.o
	$(CC) $(LDFLAGS) -o client/totp client/main.o common/util.o $(LDLIBS)

client/main.o: client/main.c common/util.h
	$(CC) $(CFLAGS) -c client/main.c -o client/main.o

# Rule for the verifier
verifier/verifier: verifier/main.o common/util.o
	$(CC) $(LDFLAGS) -o verifier/verifier verifier/main.o common/util.o $(LDLIBS)

verifier/main.o: verifier/main.c common/util.h
	$(CC) $(CFLAGS) -c verifier/main.c -o verifier/main.o

# Rule for the common object file
common/util.o: common/util.c common/util.h
	$(CC) $(CFLAGS) -c common/util.c -o common/util.o

clean:
	rm -f client/totp client/main.o verifier/verifier verifier/main.o common/util.o
