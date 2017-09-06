

GRPC_LIBRARY_PATH    := $(CURDIR)/lib
SYMBOLIC_FLAGS       := -r -s

LIB_CRYPTO_SO_FILE   := libcrypto.so.1.0.1e
LIB_CRYPTO_SO_10     := libcrypto.so.10
LIB_CRYPTO_SO        := libcrypto.so

LIB_SSL_SO_FILE      := libssl.so.1.0.1e
LIB_SSL_SO_10        := libssl.so.10
LIB_SSL_SO           := libssl.so

FIXED_LIB_CRYPTO_SO_FILE  := $(GRPC_LIBRARY_PATH)/$(LIB_CRYPTO_SO_FILE)
FIXED_LIB_CRYPTO_SO_10    := $(GRPC_LIBRARY_PATH)/$(LIB_CRYPTO_SO_10)
FIXED_LIB_CRYPTO_SO       := $(GRPC_LIBRARY_PATH)/$(LIB_CRYPTO_SO)

FIXED_LIB_SSL_SO_FILE   := $(GRPC_LIBRARY_PATH)/$(LIB_SSL_SO_FILE)
FIXED_LIB_SSL_SO_10     := $(GRPC_LIBRARY_PATH)/$(LIB_SSL_SO_10)
FIXED_LIB_SSL_SO        := $(GRPC_LIBRARY_PATH)/$(LIB_SSL_SO)

all debug release build:
	@echo "Start building $@"

ifneq ($(FIXED_LIB_CRYPTO_SO_10), $(wildcard $(FIXED_LIB_CRYPTO_SO_10)))
	ln $(SYMBOLIC_FLAGS) $(FIXED_LIB_CRYPTO_SO_FILE) $(FIXED_LIB_CRYPTO_SO_10)
endif

ifneq ($(FIXED_LIB_CRYPTO_SO), $(wildcard $(FIXED_LIB_CRYPTO_SO)))
	ln $(SYMBOLIC_FLAGS) $(FIXED_LIB_CRYPTO_SO_FILE) $(FIXED_LIB_CRYPTO_SO)
endif

ifneq ($(FIXED_LIB_SSL_SO_10), $(wildcard $(FIXED_LIB_SSL_SO_10)))
	ln $(SYMBOLIC_FLAGS) $(FIXED_LIB_SSL_SO_FILE) $(FIXED_LIB_SSL_SO_10)
endif

ifneq ($(FIXED_LIB_SSL_SO), $(wildcard $(FIXED_LIB_SSL_SO)))
	ln $(SYMBOLIC_FLAGS) $(FIXED_LIB_SSL_SO_FILE) $(FIXED_LIB_SSL_SO)
endif

	@echo "Finished building $@"

clean:
	rm -f $(FIXED_LIB_CRYPTO_SO_10)
	rm -f $(FIXED_LIB_CRYPTO_SO)
	rm -f $(FIXED_LIB_SSL_SO_10)
	rm -f $(FIXED_LIB_SSL_SO)
	@echo " "

rebuild: build

.PHONY:clean rebuild



