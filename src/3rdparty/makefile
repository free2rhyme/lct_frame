
SERVICES_LIST := grpc
SERVICES_LIST += mongocxx
SERVICES_LIST += cassandra

all debug release build rebuild:

	@for item in $(SERVICES_LIST); \
	do \
		$(MAKE) -C $$item $@; \
	done

clean:
	@for item in $(SERVICES_LIST); \
	do \
		$(MAKE) -C $$item $@ ; \
	done