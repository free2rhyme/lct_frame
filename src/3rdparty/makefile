
SERVICES_LIST := openssl
SERVICES_LIST += grpc
SERVICES_LIST += mongocxx

all debug release build rebuild:

	@for item in $(SERVICES_LIST); \
	do \
		$(MAKE) -C $$item $@; \
	done

clean:
	@for item in $(SERVICES_LIST); \
	do \
		$(MAKE) -C $$item $@ ; \
		rm -fr $(BUILD_DIR) ; \
	done