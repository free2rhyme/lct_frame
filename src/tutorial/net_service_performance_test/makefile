
SERVICES_LIST := async_tcp_client
SERVICES_LIST += async_tcp_server

all debug release build rebuild:

	@for item in $(SERVICES_LIST); \
	do \
		$(MAKE) -C $$item $@ || exit "$$?"; \
	done

clean:
	@for item in $(SERVICES_LIST); \
	do \
		$(MAKE) -C $$item $@ ; \
		rm -fr $(BUILD_DIR) ; \
	done
