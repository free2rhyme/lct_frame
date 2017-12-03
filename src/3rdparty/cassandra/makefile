
CASS_LIBRARY_PATH    := $(CURDIR)/lib
SYMBOLIC_FLAGS       := -r -s

LIB_CASSANDRA_SO_FILE := libcassandra.so.2.7.0
LIB_CASSANDRA_SO_2    := libcassandra.so.2
LIB_CASSANDRA_SO      := libcassandra.so


FIXED_LIB_CASSANDRA_SO_FILE := $(CASS_LIBRARY_PATH)/$(LIB_CASSANDRA_SO_FILE)
FIXED_LIB_CASSANDRA_SO_2    := $(CASS_LIBRARY_PATH)/$(LIB_CASSANDRA_SO_2)
FIXED_LIB_CASSANDRA_SO      := $(CASS_LIBRARY_PATH)/$(LIB_CASSANDRA_SO)


all debug release build:
	@echo "Start building $@"

ifneq ($(FIXED_LIB_CASSANDRA_SO_2), $(wildcard $(FIXED_LIB_CASSANDRA_SO_2)))
	ln $(SYMBOLIC_FLAGS) $(FIXED_LIB_CASSANDRA_SO_FILE) $(FIXED_LIB_CASSANDRA_SO_2)
endif

ifneq ($(FIXED_LIB_CASSANDRA_SO), $(wildcard $(FIXED_LIB_CASSANDRA_SO)))
	ln $(SYMBOLIC_FLAGS) $(FIXED_LIB_CASSANDRA_SO_FILE) $(FIXED_LIB_CASSANDRA_SO)
endif

	@echo "Finished building $@"

clean:
	rm -f $(FIXED_LIB_CASSANDRA_SO_2)
	rm -f $(FIXED_LIB_CASSANDRA_SO)
	@echo " "

rebuild: build

.PHONY:clean rebuild



