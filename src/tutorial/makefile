
SERVICES_LIST := circular_buffer_sanity_test
SERVICES_LIST += cassandra_native_client
SERVICES_LIST += lct_grpc_client_helper
SERVICES_LIST += lct_grpc_service

all debug release build rebuild:

	@for item in $(SERVICES_LIST); \
	do \
		$(MAKE) -C $$item $@ || exit "$$?"; \
	done

clean:
	@for item in $(SERVICES_LIST); \
	do \
		$(MAKE) -C $$item $@ ; \
	done
