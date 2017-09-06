
SERVICES_LIST := echo_via_lct_frame
SERVICES_LIST += circular_buffer_sanity_test
SERVICES_LIST += net_service_performance_test

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
